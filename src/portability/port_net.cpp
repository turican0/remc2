// port_net.cpp
// NetBIOS emulation layer over TCP.
//
// ARCHITECTURE
// ============
// All communication runs over TCP — both the control channel (name registration,
// CALL/LISTEN handshake) and the data channel (SEND/RECEIVE game data).
//
// TCP gives us guaranteed, in-order delivery for free, so there is no need for
// sequence numbers, ACKs, NACKs, reorder buffers, or retransmit timers.
//
// TOPOLOGY
// ========
//
//   Control channel (every node → server):
//     Each node maintains one persistent TCP connection to the server.
//     Commands: ADD_NAME, CALL, LISTEN, CANCEL, DELETE, GET_IP.
//     Replies:  TESTADDNAME_OK/REJECT, CALL_ACCEPT/REJECT, LISTEN_ACCEPT/REJECT.
//
//   Data channel (peer ↔ peer, after CALL/LISTEN succeeds):
//     The caller opens a fresh TCP socket directly to the listener's data port
//     (which equals the listener's clPort).  The listener accepts it from a
//     per-node TCP server socket also bound to clPort.
//     After setup both sides exchange MESS_DIRECT_SEND packets over that socket.
//     The channel is torn down on HANG_UP or peer disconnect.
//
// HEARTBEAT
// =========
//   TCP itself signals a broken link (recv returns 0 / error).
//   If no data is received for HEARTBEAT_TIMEOUT_MS the session is declared
//   dead and ncb_cmd_cplt_49 is set to NRC_SCLOSED so Network.cpp can
//   perform CANCEL / HANG_UP as on a real NetBIOS adapter.
//   After HEARTBEAT_PROBE_MS of silence a lightweight MESS_HEARTBEAT is sent.
//
// NCB COMMAND DISPATCH  (simulateInterupt → handleConnections[])
// =============================================================
//   Pass 1 – deliver pending RECEIVE data from per-session queues.
//   Pass 2 – timeout / retry per NCB command.
//   Pass 4 – heartbeat / dead-peer detection on established sessions.
//   (Pass 3 — UDP retransmit — is gone; TCP is reliable.)
//
// API COMPATIBILITY
// =================
//   simulateInterupt(), InitMyNetLib(), EndMyNetLib(), CleanMessages(),
//   printState(), printState2(), timeState(), IsRemoteAlive(),
//   ReceiveServerAddName() — all signatures unchanged from the UDP version.
#define _CRT_SECURE_NO_WARNINGS
#include "port_net.h"
#include "engine/CommandLineParser.h"
#include <map>
#include <set>
#include <queue>
#include <thread>
#include <atomic>
#include <mutex>
#include <memory>
#include <chrono>
#include <cassert>
#include <algorithm>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
typedef SOCKET socket_t;
#define CLOSE_SOCKET   closesocket
#define SOCK_INVALID   INVALID_SOCKET
static int sock_errno() { return WSAGetLastError(); }
static bool would_block(int e) { return e == WSAEWOULDBLOCK || e == WSAEINPROGRESS; }
static void set_nonblocking(socket_t s) {
	unsigned long m = 1; ioctlsocket(s, FIONBIO, &m);
}
static void set_nodelay(socket_t s) {
	int one = 1; setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (char*)&one, sizeof(one));
}
#else
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
typedef int socket_t;
#define CLOSE_SOCKET   close
#define SOCK_INVALID   (-1)
static int sock_errno() { return errno; }
static bool would_block(int e) { return e == EAGAIN || e == EWOULDBLOCK || e == EINPROGRESS; }
static void set_nonblocking(socket_t s) {
	int f = fcntl(s, F_GETFL, 0); fcntl(s, F_SETFL, f | O_NONBLOCK);
}
static void set_nodelay(socket_t s) {
	int one = 1; setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
}
#endif

using namespace std;

// ---------------------------------------------------------------------------
// Timing constants
// ---------------------------------------------------------------------------
static const long HEARTBEAT_PROBE_MS = 1000; // silence (ms) before sending a probe
static const long HEARTBEAT_TIMEOUT_MS = 5000; // silence (ms) → peer dead → CANCEL
static const int  MAX_RETRIES = 8;    // CALL / ADD_NAME retries

// NetBIOS status codes
#define NRC_GOODRET  0x00
#define NRC_CMDTMO   0x05
#define NRC_SCLOSED  0x0a
#define NRC_CMDCAN   0x0b
#define NRC_INUSE    0x16
#define NRC_PENDING  0xff

// ---------------------------------------------------------------------------
// Globals required by Network.cpp / port_net.h
// ---------------------------------------------------------------------------
uint8_t* Zero_pointer_net = nullptr;
int         timest_index = 0;
clock_t     timest_timer = 0;
const int   timest_max_mess = 400;
std::string timest_buffer[timest_max_mess];
bool m_network_debug = false;

// ---------------------------------------------------------------------------
// Protocol message type constants  (wire-compatible with the UDP version)
// ---------------------------------------------------------------------------
const int32_t MESS_UNKNOWN = -1;
const int32_t MESS_CLIENT_TESTADDNAME = 4;
const int32_t MESS_SERVER_TESTADDNAME_OK = 5;
const int32_t MESS_SERVER_TESTADDNAME_REJECT = 6;
const int32_t MESS_CLIENT_MESSAGE_LISTEN = 7;
const int32_t MESS_CLIENT_MESSAGE_CALL = 8;
const int32_t MESS_SERVER_CALL_ACCEPT = 9;
const int32_t MESS_SERVER_LISTEN_ACCEPT = 10;
const int32_t MESS_SERVER_CALL_REJECT = 11;
const int32_t MESS_SERVER_LISTEN_REJECT = 12;
const int32_t MESS_CLIENT_CANCEL = 13;
const int32_t MESS_CLIENT_DELETE = 14;
const int32_t MESS_DIRECT_SEND = 15; // peer → peer game data
const int32_t MESS_HEARTBEAT = 17; // keep-alive probe
const int32_t MESS_HEARTBEAT_ACK = 20; // keep-alive reply
const int32_t MESS_CLIENT_GET_IP = 18;
const int32_t MESS_SERVER_GIVE_IP = 19;
// Marker put in the 'index' field of a GIVE_IP that is an announcement rather than an
// address reply: the session's first NetBIOS name (…0) has been registered.
const int32_t SERVER_NAME_REGISTERED = -777;

char* MessageIndexToText(int32_t index)
{
	switch (index) {
	case MESS_CLIENT_TESTADDNAME:        return (char*)"MESS_CLIENT_TESTADDNAME";
	case MESS_SERVER_TESTADDNAME_OK:     return (char*)"MESS_SERVER_TESTADDNAME_OK";
	case MESS_SERVER_TESTADDNAME_REJECT: return (char*)"MESS_SERVER_TESTADDNAME_REJECT";
	case MESS_CLIENT_MESSAGE_LISTEN:     return (char*)"MESS_CLIENT_MESSAGE_LISTEN";
	case MESS_CLIENT_MESSAGE_CALL:       return (char*)"MESS_CLIENT_MESSAGE_CALL";
	case MESS_SERVER_CALL_ACCEPT:        return (char*)"MESS_SERVER_CALL_ACCEPT";
	case MESS_SERVER_LISTEN_ACCEPT:      return (char*)"MESS_SERVER_LISTEN_ACCEPT";
	case MESS_SERVER_CALL_REJECT:        return (char*)"MESS_SERVER_CALL_REJECT";
	case MESS_SERVER_LISTEN_REJECT:      return (char*)"MESS_SERVER_LISTEN_REJECT";
	case MESS_CLIENT_CANCEL:             return (char*)"MESS_CLIENT_CANCEL";
	case MESS_CLIENT_DELETE:             return (char*)"MESS_CLIENT_DELETE";
	case MESS_DIRECT_SEND:               return (char*)"MESS_DIRECT_SEND";
	case MESS_HEARTBEAT:                 return (char*)"MESS_HEARTBEAT";
	case MESS_HEARTBEAT_ACK:             return (char*)"MESS_HEARTBEAT_ACK";
	case MESS_CLIENT_GET_IP:             return (char*)"MESS_CLIENT_GET_IP";
	case MESS_SERVER_GIVE_IP:            return (char*)"MESS_SERVER_GIVE_IP";
	}
	return (char*)"MESS_UNKNOWN";
}

// ---------------------------------------------------------------------------
// shadow_myNCB helpers  (unchanged)
// ---------------------------------------------------------------------------
shadow_myNCB myNCBtoShadow(myNCB from)
{
	shadow_myNCB to;
	to.ncb_command_0 = from.ncb_command_0;
	to.ncb_retcode_1 = from.ncb_retcode_1;
	to.ncb_lsn_2 = from.ncb_lsn_2;
	to.ncb_num_3 = from.ncb_num_3;
	to.ncb_buffer_4 = (from.ncb_buffer_4.p - Zero_pointer_net);
	to.ncb_bufferLength_8 = from.ncb_bufferLength_8;
	for (int i = 0; i < 16; i++) to.ncb_callName_10[i] = from.ncb_callName_10[i];
	for (int i = 0; i < 16; i++) to.ncb_name_26[i] = from.ncb_name_26[i];
	to.ncb_rto_42 = from.ncb_rto_42;
	to.ncb_sto_43 = from.ncb_sto_43;
	to.ncb_cmd_cplt_49 = from.ncb_cmd_cplt_49;
	return to;
}

myNCB myNCBfromShadow(shadow_myNCB from)
{
	myNCB to;
	to.ncb_command_0 = from.ncb_command_0;
	to.ncb_retcode_1 = from.ncb_retcode_1;
	to.ncb_lsn_2 = from.ncb_lsn_2;
	to.ncb_num_3 = from.ncb_num_3;
	to.ncb_buffer_4.p = (from.ncb_buffer_4 + Zero_pointer_net);
	to.ncb_bufferLength_8 = from.ncb_bufferLength_8;
	for (int i = 0; i < 16; i++) to.ncb_callName_10[i] = from.ncb_callName_10[i];
	for (int i = 0; i < 16; i++) to.ncb_name_26[i] = from.ncb_name_26[i];
	to.ncb_rto_42 = from.ncb_rto_42;
	to.ncb_sto_43 = from.ncb_sto_43;
	to.ncb_cmd_cplt_49 = from.ncb_cmd_cplt_49;
	return to;
}

// ---------------------------------------------------------------------------
// Wire format
// Every message is framed as:
//   [uint32_t big-endian body_length][body bytes]
// where body = fixed message_info header + variable payload.
// The 4-byte length prefix is NOT counted in body_length.
// ---------------------------------------------------------------------------
static constexpr size_t MSG_WIRE_HEADER =
sizeof(uint32_t)       // size  (payload bytes)
+ sizeof(int32_t)        // message type
+ sizeof(int32_t)        // index
+ sizeof(int32_t)        // port
+ sizeof(uint32_t)       // seq_num  (always 0 over TCP)
+ sizeof(uint32_t)       // ack_num  (always 0 over TCP)
+ sizeof(shadow_myNCB);  // NCB snapshot

std::string DataToString(const message_info& m)
{
	size_t total = MSG_WIRE_HEADER + m.size;
	return std::string(reinterpret_cast<const char*>(&m), total);
}

std::string Pack_Message(int32_t      message,
	shadow_myNCB locNCB,
	int32_t      locIndex,
	int32_t      port = -1,
	const char* data = nullptr,
	int          size_of_data = 0,
	uint32_t     seq_num = 0,
	uint32_t     ack_num = 0)
{
	message_info m;
	memset(&m, 0, sizeof(m));
	m.message = message;
	m.size = size_of_data;
	m.messNCB = locNCB;
	m.index = locIndex;
	m.port = port;
	m.seq_num = seq_num;
	m.ack_num = ack_num;
	if (data && size_of_data > 0)
		memcpy(m.data, data, size_of_data);
	return DataToString(m);
}

void debug_net_printf(const char* format, ...);   // forward

message_info Unpack_Message(const std::string& data)
{
	message_info out;
	memset(&out, 0, sizeof(out));
	if (data.size() < MSG_WIRE_HEADER) {
		if (m_network_debug)
			debug_net_printf("Unpack: too short %zu/%zu\n", data.size(), MSG_WIRE_HEADER);

		out.message = MESS_UNKNOWN;
		return out;
	}
	memcpy(&out, data.data(), MSG_WIRE_HEADER);
	if (out.size > sizeof(out.data) || MSG_WIRE_HEADER + out.size > data.size()) {
		if (m_network_debug)
			debug_net_printf("Unpack: bad payload size %u\n", out.size);

		out.message = MESS_UNKNOWN;

		return out;
	}
	if (out.size > 0)
		memcpy(out.data, data.data() + MSG_WIRE_HEADER, out.size);
	return out;
}

// ---------------------------------------------------------------------------
// Debug logging
// ---------------------------------------------------------------------------
FILE* debug_net_output = nullptr;
std::string debug_net_filename1 = "net_messages_log1.txt";
bool        debug_net_first = true;
bool        debug_net = true;
std::mutex  debug_net_mutex;

void debug_net_printf(const char* format, ...)
{
	char buf[1024 * 40];
	va_list arg;
	va_start(arg, format);
	vsprintf(buf, format, arg);
	va_end(arg);
	std::lock_guard<std::mutex> lk(debug_net_mutex);
	debug_net_output = fopen(debug_net_filename1.c_str(), debug_net_first ? "wt" : "at");
	debug_net_first = false;
	if (debug_net_output) {
		fprintf(debug_net_output, "%ld|%s", (long)clock(), buf);
		fclose(debug_net_output);
	}
}

static std::string PayloadPreview(const char* d, uint32_t sz)
{
	if (!sz) return "";
	std::string h, a;
	int show = sz < 16 ? (int)sz : 16;
	char tmp[8];
	for (int i = 0; i < show; i++) {
		snprintf(tmp, sizeof(tmp), "%02X ", (unsigned char)d[i]);
		h += tmp;
		a += (d[i] >= 32 && d[i] < 127) ? d[i] : '.';
	}
	if ((int)sz > 16) { h += "..."; a += "..."; }
	return "hex=[" + h + "] asc=[" + a + "]";
}

static void LogPkt(const char* dir, const std::string& addr, int port,
	const message_info& u)
{
	std::string p = PayloadPreview(u.data, u.size);
	if (m_network_debug)
		debug_net_printf("%s %s:%d  msg=%-30s size=%u name=[%.16s] call=[%.16s] idx=%d%s%s\n",
			dir, addr.c_str(), port, MessageIndexToText(u.message),
			u.size, u.messNCB.ncb_name_26, u.messNCB.ncb_callName_10, u.index,
			p.empty() ? "" : " ", p.c_str());

}

// ---------------------------------------------------------------------------
// TCP helpers — length-prefixed framing
// ---------------------------------------------------------------------------

// Send all bytes of msg on socket s.
//
// Every socket here is non-blocking, so a large frame (a full game-state packet is
// ~17 KB and the packet limit is 60 KB, against a default ~64 KB socket buffer) can
// easily fill the send buffer and make send() fail with EWOULDBLOCK part way through.
// Treating that as a fatal error - as this used to - left the 4-byte length prefix and
// a partial body on the wire: the receiver then waited for a remainder that never came
// and glued the *next* message onto the truncated one, so both sides silently decoded
// garbage game state.  Instead wait for the socket to become writable and continue, so
// a frame is either delivered whole or the connection is reported as broken.
static const long TCP_SEND_STALL_TIMEOUT_MS = 5000;

static bool TcpSendFull(socket_t s, const std::string& msg)
{
	if (s == SOCK_INVALID) return false;
	uint32_t len = (uint32_t)msg.size();
	uint8_t hdr[4] = {
		(uint8_t)(len >> 24), (uint8_t)(len >> 16),
		(uint8_t)(len >> 8), (uint8_t)(len) };
	auto send_all = [s](const void* b, int n) -> bool {
		const char* p = (const char*)b; int left = n;
		long waited = 0;
		while (left > 0) {
			int r = send(s, p, left, 0);
			if (r > 0) { p += r; left -= r; waited = 0; continue; }
			if (r == 0) return false;
			int e = sock_errno();
			if (!would_block(e)) return false;
			// Buffer full: wait until the peer drains it, then resume this frame.
			fd_set wf; FD_ZERO(&wf); FD_SET(s, &wf);
			timeval tv; tv.tv_sec = 0; tv.tv_usec = 50 * 1000;
			int sel = select((int)s + 1, nullptr, &wf, nullptr, &tv);
			if (sel < 0) return false;
			if (sel == 0) {
				waited += 50;
				if (waited >= TCP_SEND_STALL_TIMEOUT_MS) return false; // peer is gone/stuck
			}
		}
		return true;
		};
	return send_all(hdr, 4) && send_all(msg.data(), (int)msg.size());
}

// Incremental receive buffer — appends data, extracts complete frames.
struct TcpRecvBuf {
	std::string              raw;
	std::vector<std::string> ready; // complete body strings

	// Max sane frame size: header + max payload
	static const uint32_t MAX_FRAME = (uint32_t)(sizeof(message_info) + 64);

	// Returns false if the socket is closed / errored.
	bool Poll(socket_t s) {
		char tmp[65536];
		while (true) {
			int n = recv(s, tmp, sizeof(tmp), 0);
			if (n > 0) { raw.append(tmp, n); }
			else if (n == 0) { return false; } // peer closed
			else { int e = sock_errno(); if (would_block(e)) break; return false; }
		}
		while (raw.size() >= 4) {
			uint32_t len = ((uint8_t)raw[0] << 24) | ((uint8_t)raw[1] << 16)
				| ((uint8_t)raw[2] << 8) | (uint8_t)raw[3];
			// Sanity check: reject frames that are impossibly large.
			// This catches framing de-sync (e.g. if a send was interrupted
			// mid-frame and the socket was re-used — should not happen on
			// a single TCP connection but defends against bugs).
			if (len > MAX_FRAME) {
				if (m_network_debug)
					debug_net_printf("TcpRecvBuf: FATAL bad frame len=%u (max=%u), discarding %zu bytes — connection likely corrupted\n", len, MAX_FRAME, raw.size());

				raw.clear();
				return false; // treat as disconnect — upper layer will reconnect
			}
			if (raw.size() < 4 + len) break; // incomplete frame, wait for more data
			ready.push_back(raw.substr(4, len));
			raw.erase(0, 4 + len);
		}
		return true;
	}
};

// ---------------------------------------------------------------------------
// Fault injection for automated testing (--net_delay / --net_jitter / --net_stall).
//
// Deliberately confined to the peer-to-peer game data: the control channel that
// registers names and sets up sessions is never touched, so a test disturbs a running
// game rather than preventing it from starting.  All three are off unless asked for on
// the command line, so a normal game runs through the untouched path.
// ---------------------------------------------------------------------------
namespace NetFaults {
	static int  s_delayMs = 0;
	static int  s_jitterMs = 0;
	static int  s_stallMs = 0;
	static int  s_stallEvery = 0;
	static bool s_configured = false;

	void Configure(int delayMs, int jitterMs, int stallMs, int stallEvery)
	{
		s_delayMs = delayMs;
		s_jitterMs = jitterMs;
		s_stallMs = stallMs;
		s_stallEvery = stallEvery;
		s_configured = (delayMs > 0 || jitterMs > 0 || (stallMs > 0 && stallEvery > 0));
		if (s_configured)
			debug_net_printf("NETFAULT: delay=%dms jitter=%dms stall=%dms every %d messages\n",
				delayMs, jitterMs, stallMs, stallEvery);
	}

	bool Enabled() { return s_configured; }

	// Deterministic per-process pseudo-random, so a failing run can be repeated.
	static unsigned NextRand()
	{
		static unsigned state = 12345;
		state = state * 1103515245u + 12345u;
		return (state >> 16) & 0x7FFF;
	}

	// Hold a freshly arrived message back for a while before handing it on.
	//
	// Only ever delays - never discards or reorders.  Over TCP a message that was sent
	// does arrive, and in order; what an unreliable line actually costs is time, because
	// the lost segment is retransmitted and everything queued behind it waits with it.
	// A stall reproduces exactly that, so the test stays faithful to what the game can
	// really meet on the wire.
	int NextHoldMs()
	{
		int ms = s_delayMs;
		if (s_jitterMs > 0) ms += (int)(NextRand() % (unsigned)(s_jitterMs + 1));
		if (s_stallMs > 0 && s_stallEvery > 0) {
			static int counter = 0;
			if (++counter >= s_stallEvery) {
				counter = 0;
				ms += s_stallMs;
				debug_net_printf("NETFAULT: stalling a message by %dms (retransmit-like)\n", s_stallMs);
			}
		}
		return ms;
	}
}

// ---------------------------------------------------------------------------
// Name / connection registry  (unchanged API surface)
// ---------------------------------------------------------------------------
std::vector<std::string> NetworkName;
std::vector<TypeIpPort>  clientIpPort;
std::vector<std::string> ListenName;
std::vector<TypeIpPort>  clientListenID;
std::vector<std::string> ListenName2;
std::vector<TypeIpPort>  clientListenID2;
std::vector<myNCB*>      clientConnection; // established NCBs, for Pass 4

bool receiveServerAddName = false;
bool ReceiveServerAddName() { return receiveServerAddName; }

static std::string TrimName(const char* s, int maxLen = 16)
{
	int e = maxLen;
	while (e > 0 && (s[e - 1] == ' ' || s[e - 1] == '\0')) --e;
	return std::string(s, e);
}

TypeIpPort GetIpPortFromName(std::string name)
{
	std::string t = TrimName(name.c_str(), (int)name.size());
	for (int i = 0; i < (int)NetworkName.size(); i++)
		if (TrimName(NetworkName[i].c_str(), (int)NetworkName[i].size()) == t)
			return clientIpPort[i];
	TypeIpPort r; r.adress = "x999"; r.port = -1; return r;
}

std::string GetNameNetworkFromId(TypeIpPort ip)
{
	for (int i = 0; i < (int)NetworkName.size(); i++)
		if (ip.adress == clientIpPort[i].adress && ip.port == clientIpPort[i].port)
			return NetworkName[i];
	return "";
}

std::string GetNameNetwork(std::string name)
{
	std::string t = TrimName(name.c_str(), (int)name.size());
	for (auto& n : NetworkName)
		if (TrimName(n.c_str(), (int)n.size()) == t) return n;
	return "";
}

int GetNameNetworkIndex(std::string name)
{
	std::string t = TrimName(name.c_str(), (int)name.size());
	for (int i = 0; i < (int)NetworkName.size(); i++)
		if (TrimName(NetworkName[i].c_str(), (int)NetworkName[i].size()) == t) return i;
	return -1;
}

void AddNetworkName(std::string name, TypeIpPort ip)
{
	if (GetNameNetwork(name).empty()) { NetworkName.push_back(name); clientIpPort.push_back(ip); }
}

bool ExistNetworkName(std::string name, TypeIpPort ip)
{
	std::string t = TrimName(name.c_str(), (int)name.size());
	for (int i = 0; i < (int)NetworkName.size(); i++)
		if (TrimName(NetworkName[i].c_str(), (int)NetworkName[i].size()) == t &&
			clientIpPort[i].adress == ip.adress && clientIpPort[i].port == ip.port)
			return true;
	return false;
}

void RemoveNetworkName(std::string name)
{
	int idx = GetNameNetworkIndex(name);
	if (idx < 0) return;
	NetworkName.erase(NetworkName.begin() + idx);
	clientIpPort.erase(clientIpPort.begin() + idx);
}

TypeIpPort GetOtherSide(TypeIpPort ip)
{
	for (int i = 0; i < (int)clientListenID.size(); i++)
		if (clientListenID[i].adress == ip.adress && clientListenID[i].port == ip.port)
			return clientListenID2[i];
	for (int i = 0; i < (int)clientListenID2.size(); i++)
		if (clientListenID2[i].adress == ip.adress && clientListenID2[i].port == ip.port)
			return clientListenID[i];
	TypeIpPort r; r.adress = "x1000"; r.port = -1; return r;
}

std::string GetListenNetwork(std::string name)
{
	for (auto& n : ListenName) if (!name.compare(n)) return n;
	return "";
}

int GetNameListenIndex(std::string name)
{
	for (int i = 0; i < (int)ListenName.size(); i++)
		if (!name.compare(ListenName[i])) return i;
	return -1;
}

int GetName2ListenIndex(std::string name)
{
	for (int i = 0; i < (int)ListenName2.size(); i++)
		if (!name.compare(ListenName2[i])) return i;
	return -1;
}

void AddListenName(myNCB* c)
{
	if (!GetListenNetwork(c->ncb_callName_10).empty()) return;
	ListenName.push_back(c->ncb_callName_10);
	clientListenID.push_back(GetIpPortFromName(c->ncb_callName_10));
	ListenName2.push_back(c->ncb_name_26);
	clientListenID2.push_back(GetIpPortFromName(c->ncb_name_26));
}

bool AddListenName2(const shadow_myNCB* c)
{
	TypeIpPort id1 = GetIpPortFromName(c->ncb_callName_10);
	if (id1.adress == "x999") return false;
	TypeIpPort id2 = GetIpPortFromName(c->ncb_name_26);
	if (id2.adress == "x999") return false;
	int idx = GetNameListenIndex(c->ncb_name_26);
	if (idx < 0) return false;
	clientListenID[idx] = id1;
	clientListenID2[idx] = id2;
	return true;
}

void RemoveListenName(std::string name)
{
	int idx = GetNameListenIndex(name);
	if (idx >= 0) {
		ListenName.erase(ListenName.begin() + idx);
		clientListenID.erase(clientListenID.begin() + idx);
		ListenName2.erase(ListenName2.begin() + idx);
		clientListenID2.erase(clientListenID2.begin() + idx);
	}
	int idx2 = GetName2ListenIndex(name);
	if (idx2 >= 0) {
		ListenName.erase(ListenName.begin() + idx2);
		clientListenID.erase(clientListenID.begin() + idx2);
		ListenName2.erase(ListenName2.begin() + idx2);
		clientListenID2.erase(clientListenID2.begin() + idx2);
	}
}

// ---------------------------------------------------------------------------
// NCB state machine entry
// ---------------------------------------------------------------------------
enum Neti_type {
	NETI_NOT_SET, NETI_ADD_NAME, NETI_ADD_NAME_REJECT, NETI_ADD_NAME_OK,
	NETI_CALL, NETI_CALL_ACCEPT, NETI_CALL_REJECT,
	NETI_LISTEN, NETI_LISTEN_REJECT, NETI_RECEIVE
};

struct connectionTime {
	myNCB* connection;
	long      startTime;
	int       timeout;
	int32_t   index;
	Neti_type state;
	int       retryCount;
};

std::vector<connectionTime> handleConnections;
std::mutex                  connections_mutex;  // guards handleConnections
std::mutex                  clientConnMutex;    // guards clientConnection (separate!)

static connectionTime* GetConnection(int32_t index)
{
	for (auto& c : handleConnections) if (c.index == index) return &c;
	return nullptr;
}

int32_t lastIndex = 0;
int32_t GetNextIndex()
{
	if (++lastIndex > 1000000) lastIndex = 0;
	return lastIndex;
}

static void setListenConnection(myNCB* c)
{
	std::lock_guard<std::mutex> lk(clientConnMutex);
	for (auto* p : clientConnection) if (p == c) return;
	clientConnection.push_back(c);
}

static void deleteListenConnection(myNCB* c)
{
	std::lock_guard<std::mutex> lk(clientConnMutex);
	for (int i = 0; i < (int)clientConnection.size(); i++)
		if (clientConnection[i] == c) { clientConnection.erase(clientConnection.begin() + i); return; }
}

// Mark the LISTEN NCB that is waiting for the caller identified by tmp->ncb_name_26.
//
// LISTEN NCB layout:
//   ncb_callName_10 = name we listen FOR  (= the caller's name, e.g. "NETH201")
//   ncb_name_26     = our own name        (= the listener's name, e.g. "NETH200")
//
// The server sends LISTEN_ACCEPT with:
//   messNCB.ncb_name_26     = caller's name    ("NETH201") ← what we match against
//   messNCB.ncb_callName_10 = listener's name  ("NETH200") ← NOT used for matching
//
// So: tmp->ncb_name_26 (caller's name) == c->ncb_callName_10 (who we waited for).
static bool setListen(myNCB* tmp)
{
	if (m_network_debug)
		debug_net_printf("setListen: looking for NCB whose callName=[%.16s]\n", tmp->ncb_name_26);

	std::lock_guard<std::mutex> lk(clientConnMutex);
	for (auto* c : clientConnection) {
		if (memcmp(tmp->ncb_name_26, c->ncb_callName_10, 16) == 0) {
			c->ncb_lsn_2 = 20;
			//c->ncb_cmd_cplt_49 = NRC_GOODRET;
			if (m_network_debug)
				debug_net_printf("setListen: MATCH name=[%.16s] call=[%.16s] lsn=20\n", c->ncb_name_26, c->ncb_callName_10);

			return true;
		}
		if (m_network_debug)
			debug_net_printf("setListen: candidate name=[%.16s] call=[%.16s] NO MATCH\n", c->ncb_name_26, c->ncb_callName_10);

	}
	if (m_network_debug)
		debug_net_printf("setListen: NO MATCH\n");

	return false;
}

// ---------------------------------------------------------------------------
// Per-session TCP data connection
// One TcpSession exists for each active CALL/LISTEN pair.
// The session socket carries only MESS_DIRECT_SEND and MESS_HEARTBEAT frames.
// ---------------------------------------------------------------------------
struct TcpSession {
	socket_t    sock = SOCK_INVALID;
	TcpRecvBuf  rbuf;
	std::string peerAddr;
	int         peerPort = 0;
	myNCB* ownerNCB = nullptr; // the LISTEN or CALL NCB for this session

	// Received game payloads ready for Pass 1
	std::mutex              queueMtx;
	std::queue<std::string> dataQueue; // packed message_info strings (MESS_DIRECT_SEND)

	// Send-side mutex: prevents interleaved writes from SendNetwork (main/bg thread)
	// and PollSessions HEARTBEAT sends (network thread).
	std::mutex              sendMtx;

	// Arrivals deliberately held back by the test fault injector
	struct HeldMessage { std::string raw; std::chrono::steady_clock::time_point releaseAt; };
	std::deque<HeldMessage> heldMessages;

	// Liveness tracking
	std::chrono::steady_clock::time_point lastRx;
	std::chrono::steady_clock::time_point lastProbeSent;
	bool probePending = false;
	std::atomic<bool> alive{ false };

	void Touch() { lastRx = std::chrono::steady_clock::now(); probePending = false; }

	long SilenceMs() const {
		return (long)std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::steady_clock::now() - lastRx).count();
	}

	// Send a packed message over the session socket.
	// Thread-safe: sendMtx prevents concurrent writes from different threads.
	bool Send(const std::string& msg) {
		if (!alive.load() || sock == SOCK_INVALID) return false;
		std::lock_guard<std::mutex> lk(sendMtx);
		if (!alive.load() || sock == SOCK_INVALID) return false; // re-check after lock
		if (debug_net) { message_info u = Unpack_Message(msg); LogPkt("DATA TX →", peerAddr, peerPort, u); }
		bool ok = TcpSendFull(sock, msg);
		if (!ok) { alive.store(false); }
		return ok;
	}

	void Close() {
		alive.store(false);
		std::lock_guard<std::mutex> lk(sendMtx); // wait for any in-progress send
		if (sock != SOCK_INVALID) { CLOSE_SOCKET(sock); sock = SOCK_INVALID; }
	}
};

static std::map<myNCB*, std::shared_ptr<TcpSession>> tcpSessions;
static std::mutex                                     sessions_mutex;

static std::shared_ptr<TcpSession> GetSession(myNCB* ncb)
{
	std::lock_guard<std::mutex> lk(sessions_mutex);
	auto it = tcpSessions.find(ncb);
	return (it != tcpSessions.end()) ? it->second : nullptr;
}

static void AddSession(myNCB* ncb, std::shared_ptr<TcpSession> s)
{
	std::lock_guard<std::mutex> lk(sessions_mutex); tcpSessions[ncb] = s;
}

static void RemoveSession(myNCB* ncb)
{
	std::lock_guard<std::mutex> lk(sessions_mutex);
	auto it = tcpSessions.find(ncb);
	if (it != tcpSessions.end()) { it->second->Close(); tcpSessions.erase(it); }
}

// ---------------------------------------------------------------------------
// IsRemoteAlive  (unchanged API)
// ---------------------------------------------------------------------------
bool IsRemoteAlive(const std::string& addr, int port)
{
	std::lock_guard<std::mutex> lk(sessions_mutex);
	for (auto& kv : tcpSessions) {
		auto& s = kv.second;
		if (s->alive && s->peerAddr == addr && s->peerPort == port)
			return s->SilenceMs() < HEARTBEAT_TIMEOUT_MS;
	}
	return false;
}

// ---------------------------------------------------------------------------
// CleanMessages  (unchanged API)
// ---------------------------------------------------------------------------
void CleanMessages(myNCB locNCB)
{
	std::lock_guard<std::mutex> lk(sessions_mutex);
	for (auto& kv : tcpSessions) {
		auto& s = kv.second;
		std::lock_guard<std::mutex> qlk(s->queueMtx);
		while (!s->dataQueue.empty()) s->dataQueue.pop();
	}
}

// ---------------------------------------------------------------------------
// Peer data-accept socket  (one per node, bound to clPort)
// ---------------------------------------------------------------------------
static socket_t peerListenSock = SOCK_INVALID;

// Claim a listening port exclusively.
//
// On Windows SO_REUSEADDR does NOT mean "reuse a TIME_WAIT port" as it does on POSIX -
// it lets a second socket bind a port another socket is already listening on, and which
// of them receives a new connection is then undefined.  Two instances started on one
// machine with overlapping ports (e.g. the client's data port equal to the server's
// control port) therefore steal each other's connections instead of failing, which shows
// up as endless reconnects and "no listenNCB" rejections.  Ask for exclusive use so an
// overlap fails loudly at bind() time.  On POSIX keep SO_REUSEADDR - there it only allows
// rebinding a port left in TIME_WAIT, which is what we want after a restart.
static void set_exclusive_listen(socket_t s)
{
	int opt = 1;
#ifdef _WIN32
	setsockopt(s, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, (char*)&opt, sizeof(opt));
#else
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));
#endif
}

// Bind the port peers connect to for game data.  'port' is in/out: if the configured
// port is taken (typically because it collides with the control port of a server running
// on the same machine) we fall back to an OS-assigned port and report it back, because
// the port we actually listen on is what gets advertised to the server and handed to the
// caller in CALL_ACCEPT.  Losing this socket used to be silent, and only broke later,
// when this node happened to be the one that had to LISTEN - which is why the failure
// looked like it depended on who started the session first.
static bool StartPeerListen(int& port)
{
	peerListenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (peerListenSock == SOCK_INVALID) return false;
	set_exclusive_listen(peerListenSock);
	set_nonblocking(peerListenSock);
	sockaddr_in a{}; a.sin_family = AF_INET; a.sin_port = htons((unsigned short)port);
	a.sin_addr.s_addr = INADDR_ANY;
	if (bind(peerListenSock, (sockaddr*)&a, sizeof(a)) != 0) {
		int e = sock_errno();
		debug_net_printf("PeerListen: data port %d unavailable (err=%d) - taking an OS-assigned port instead\n", port, e);
		sockaddr_in any{}; any.sin_family = AF_INET; any.sin_port = 0; any.sin_addr.s_addr = INADDR_ANY;
		if (bind(peerListenSock, (sockaddr*)&any, sizeof(any)) != 0) {
			debug_net_printf("PeerListen: FATAL - no data port could be bound (err=%d)\n", sock_errno());
			CLOSE_SOCKET(peerListenSock); peerListenSock = SOCK_INVALID; return false;
		}
		sockaddr_in got{}; socklen_t gl = sizeof(got);
		if (getsockname(peerListenSock, (sockaddr*)&got, &gl) != 0) {
			CLOSE_SOCKET(peerListenSock); peerListenSock = SOCK_INVALID; return false;
		}
		port = ntohs(got.sin_port);
		debug_net_printf("PeerListen: using data port %d\n", port);
	}
	else if (port == 0) {
		// Caller asked the OS to choose; report back what we actually got, because this
		// number is what we advertise to the server and to the peer that has to call us.
		sockaddr_in got{}; socklen_t gl = sizeof(got);
		if (getsockname(peerListenSock, (sockaddr*)&got, &gl) != 0) {
			CLOSE_SOCKET(peerListenSock); peerListenSock = SOCK_INVALID; return false;
		}
		port = ntohs(got.sin_port);
		debug_net_printf("PeerListen: using data port %d\n", port);
	}
	listen(peerListenSock, 8);
	if (m_network_debug)
		debug_net_printf("PeerListen: data port %d\n", port);

	return true;
}

// ---------------------------------------------------------------------------
// Control channel
// Client: one TCP socket to server (ctrlSock).
// Server: accept socket (ctrlListenSock) + one socket per client.
// ---------------------------------------------------------------------------
static socket_t  ctrlSock = SOCK_INVALID; // client's connection to server
static TcpRecvBuf ctrlRecvBuf;
static std::mutex ctrlSendMtx;

static socket_t  ctrlListenSock = SOCK_INVALID; // server's accept socket

struct ClientCtrl {
	socket_t    sock = SOCK_INVALID;
	TcpRecvBuf  rbuf;
	std::string addr;
	int         port = 0; // the SOURCE port of the TCP connection from the client
	int         dataPort = 0; // the client's clPort (data port), filled from GET_IP
};
static std::vector<ClientCtrl> ctrlClients;
// ONE recursive mutex protects all access to ctrlClients.
// recursive_mutex is needed because PollCtrlClients (outer lock) calls
// HandleServerMsg which calls SendToCtrlClient (inner lock) — same thread,
// same mutex.  std::mutex would deadlock; std::recursive_mutex allows it.
static std::recursive_mutex ctrlClientsMtx;

static bool SendCtrl(const std::string& msg)
{
	std::lock_guard<std::mutex> lk(ctrlSendMtx);
	if (ctrlSock == SOCK_INVALID) return false;
	if (debug_net) { message_info u = Unpack_Message(msg); LogPkt("CTRL TX →", "server", 0, u); }
	return TcpSendFull(ctrlSock, msg);
}

// Send a control reply to a connected client identified by addr + dataPort.
static bool SendToCtrlClient(const std::string& msg, const std::string& addr, int dataPort)
{
	std::lock_guard<std::recursive_mutex> lk(ctrlClientsMtx);
	for (auto& cc : ctrlClients) {
		if (cc.addr == addr && cc.dataPort == dataPort) {
			if (debug_net) { message_info u = Unpack_Message(msg); LogPkt("CTRL TX →", addr, dataPort, u); }
			return TcpSendFull(cc.sock, msg);
		}
	}
	// Fallback: match by addr only (dataPort not yet set)
	for (auto& cc : ctrlClients) {
		if (cc.addr == addr) {
			if (debug_net) { message_info u = Unpack_Message(msg); LogPkt("CTRL TX →", addr, dataPort, u); }
			return TcpSendFull(cc.sock, msg);
		}
	}
	if (m_network_debug)
		debug_net_printf("SendToCtrlClient: no client %s:%d\n", addr.c_str(), dataPort);

	return false;
}

// ===========================================================================
// NetworkClass
// ===========================================================================
namespace MyNetworkLib {
	/*
	class NetworkClass {
	public:
		std::string clHost;
		int  clPort = 0, clServerPort = 0;
		bool clIam_server = false, clIam_client = false, IpPortIsSet = false;
		bool serverAddName = false, serverAddname = false;
		std::vector<TypeIpPort> registered;

		NetworkClass(bool iam_server, bool iam_client,
			std::string host, int port, int serverPort);
		~NetworkClass();

		void StartNetworkThread();
		void StopNetworkThread();
		void locUpdateNetworkSingleThread();

		// NCB dispatchers
		void AddName(myNCB*, int32_t);
		void CancelNetwork(myNCB*, int32_t);
		void DeleteNetwork(myNCB*, int32_t);
		void CallNetwork(myNCB*, int32_t);
		void ListenNetwork(myNCB*, int32_t);
		void SendNetwork(myNCB*, int32_t);

	private:
		std::thread       netThread;
		std::atomic<bool> netRunning{ false };

		// directPeers: ncb_name/callName (16 chars) → {addr, dataPort}
		struct PeerInfo { std::string addr; int dataPort; };
		std::map<std::string, PeerInfo> directPeers;

		void ConnectToServer();
		void AcceptCtrlClients();
		void PollCtrlClients();
		void PollCtrlSocket();
		void AcceptPeerConnections();
		void PollSessions();
		bool ConnectDataToPeer(myNCB* ncb, const std::string& addr, int dataPort);
		void HandleServerMsg(const std::string& raw, const std::string& senderAddr, int senderDataPort);
		void HandleClientMsg(const std::string& raw);
		void UpdateClient();
	};*/

	// ---------------------------------------------------------------------------
	NetworkClass::NetworkClass(bool iam_server, bool iam_client,
		std::string host, int port, int serverPort)
	{
		clPort = port;
		clServerPort = serverPort;
		clHost = iam_server ? "127.0.0.1" : host;
		clIam_server = iam_server;
		clIam_client = iam_client;
		IpPortIsSet = iam_server;

		// Give every instance its own debug log.  Two instances started from the same
		// folder would otherwise both open "net_messages_log1.txt": the one starting
		// second truncates ("wt") what the first has already written, and from then on
		// their lines interleave in a single file with two unrelated clock() timelines.
		// That makes the capture nearly unusable for diagnosing a desync.
		{
			char portbuf[16];
			snprintf(portbuf, sizeof(portbuf), "%d", clPort);
			debug_net_filename1 = std::string("net_messages_log1_") + portbuf + ".txt";
		}

		// Server: claim the control port FIRST.  It is the one port that cannot move -
		// clients are told to connect there - whereas the data port below can relocate.
		if (clIam_server) {
			ctrlListenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			set_exclusive_listen(ctrlListenSock);
			set_nonblocking(ctrlListenSock);
			sockaddr_in a{}; a.sin_family = AF_INET;
			a.sin_port = htons((unsigned short)clServerPort);
			a.sin_addr.s_addr = INADDR_ANY;
			if (bind(ctrlListenSock, (sockaddr*)&a, sizeof(a)) == 0) {
				listen(ctrlListenSock, 16);
				if (m_network_debug)
					debug_net_printf("NetworkClass: ctrl listen on port %d\n", clServerPort);

			}
			else {
				// Always report this: hosting is impossible and every later symptom
				// (clients looping on "server disconnected") follows from it.
				debug_net_printf("NetworkClass: FATAL - cannot listen on control port %d (err=%d); "
					"another program or game instance already uses it\n", clServerPort, sock_errno());
				CLOSE_SOCKET(ctrlListenSock); ctrlListenSock = SOCK_INVALID;
			}
		}

		// A pure client must never squat on the port the server listens on when both run
		// on this machine: whoever starts first wins it, and if the client wins, the host
		// cannot open its control port at all.  The data port is advertised to the peers,
		// so moving it is transparent - unlike the control port, which is fixed.
		if (!clIam_server && clPort == clServerPort &&
			(clHost.compare(0, 4, "127.") == 0 || clHost == "localhost" || clHost == "::1"))
		{
			debug_net_printf("NetworkClass: data port %d is the local server's control port - "
				"taking an OS-assigned data port instead\n", clPort);
			clPort = 0; // ask the OS; StartPeerListen reports what we actually got
		}

		// Peer data port (may relocate if the configured one is taken).
		if (!StartPeerListen(clPort))
			debug_net_printf("NetworkClass: FATAL - no data port available; peers cannot connect\n");
	}

	NetworkClass::~NetworkClass()
	{
		StopNetworkThread();
		if (peerListenSock != SOCK_INVALID) { CLOSE_SOCKET(peerListenSock);   peerListenSock = SOCK_INVALID; }
		if (ctrlListenSock != SOCK_INVALID) { CLOSE_SOCKET(ctrlListenSock);   ctrlListenSock = SOCK_INVALID; }
		if (ctrlSock != SOCK_INVALID) { CLOSE_SOCKET(ctrlSock);         ctrlSock = SOCK_INVALID; }
	}

	void NetworkClass::StartNetworkThread()
	{
		if (netRunning.load()) return;
		netRunning.store(true);
		netThread = std::thread([this]() {
			if (m_network_debug)
				debug_net_printf("NetThread: started\n");

			while (netRunning.load()) { locUpdateNetworkSingleThread(); std::this_thread::sleep_for(std::chrono::milliseconds(1)); }
			if (m_network_debug)
				debug_net_printf("NetThread: stopped\n");

			});
	}

	void NetworkClass::StopNetworkThread()
	{
		if (!netRunning.load()) return;
		netRunning.store(false);
		if (netThread.joinable()) netThread.join();
	}

	// ---------------------------------------------------------------------------
	void NetworkClass::ConnectToServer()
	{
		if (ctrlSock != SOCK_INVALID) return;
		socket_t s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (s == SOCK_INVALID) return;
		set_nodelay(s);
		sockaddr_in a{}; a.sin_family = AF_INET;
		a.sin_port = htons((unsigned short)clServerPort);
		inet_pton(AF_INET, clHost.c_str(), &a.sin_addr);
		if (connect(s, (sockaddr*)&a, sizeof(a)) != 0) {
			int e = sock_errno();
			if (!would_block(e)) { CLOSE_SOCKET(s); return; }
		}
		set_nonblocking(s);
		{ std::lock_guard<std::mutex> lk(ctrlSendMtx); ctrlSock = s; }
		IpPortIsSet = true;
		if (m_network_debug)
			debug_net_printf("ConnectToServer: connected to %s:%d\n", clHost.c_str(), clServerPort);

	}

	// ---------------------------------------------------------------------------
	void NetworkClass::AcceptCtrlClients()
	{
		if (ctrlListenSock == SOCK_INVALID) return;
		while (true) {
			sockaddr_in from{}; socklen_t fl = sizeof(from);
			socket_t s = accept(ctrlListenSock, (sockaddr*)&from, &fl);
			if (s == SOCK_INVALID) break;
			set_nonblocking(s); set_nodelay(s);
			char ip[INET_ADDRSTRLEN]; inet_ntop(AF_INET, &from.sin_addr, ip, sizeof(ip));
			ClientCtrl cc; cc.sock = s; cc.addr = ip; cc.port = ntohs(from.sin_port);
			// A node joining after the session's first name was registered has to be told
			// about it now - it will not ask, because it considers itself set up as soon
			// as the control connection is up.
			if (serverAddname) {
				shadow_myNCB sn{}; sn.ncb_command_0 = 0xFE;
				TcpSendFull(s, Pack_Message(MESS_SERVER_GIVE_IP, sn, SERVER_NAME_REGISTERED, clPort));
			}
			std::lock_guard<std::recursive_mutex> lk(ctrlClientsMtx);
			ctrlClients.push_back(std::move(cc));
			if (m_network_debug)
				debug_net_printf("AcceptCtrl: client %s:%d\n", ip, cc.port);

		}
	}

	// ---------------------------------------------------------------------------
	void NetworkClass::PollCtrlClients()
	{
		std::lock_guard<std::recursive_mutex> lk(ctrlClientsMtx);
		for (auto it = ctrlClients.begin(); it != ctrlClients.end(); ) {
			ClientCtrl& cc = *it;
			bool ok = cc.rbuf.Poll(cc.sock);
			for (auto& raw : cc.rbuf.ready) {
				message_info u = Unpack_Message(raw);
				if (debug_net) LogPkt("CTRL RX ←", cc.addr, cc.dataPort, u);
				// Intercept GET_IP to learn client's data port
				if (u.message == MESS_CLIENT_GET_IP) cc.dataPort = u.port;
				if (u.message == MESS_CLIENT_TESTADDNAME) cc.dataPort = u.port;
				HandleServerMsg(raw, cc.addr, cc.dataPort ? cc.dataPort : u.port);
			}
			cc.rbuf.ready.clear();
			if (!ok) {
				if (m_network_debug)
					debug_net_printf("PollCtrlClients: client %s:%d disconnected\n", cc.addr.c_str(), cc.port);

				for (int ni = (int)NetworkName.size() - 1; ni >= 0; ni--) {
					if (clientIpPort[ni].adress == cc.addr && clientIpPort[ni].port == cc.dataPort) {
						RemoveListenName(NetworkName[ni]);
						RemoveNetworkName(NetworkName[ni]);
					}
				}
				CLOSE_SOCKET(cc.sock);
				it = ctrlClients.erase(it);
			}
			else { ++it; }
		}
	}

	// ---------------------------------------------------------------------------
	void NetworkClass::PollCtrlSocket()
	{
		if (ctrlSock == SOCK_INVALID) return;
		bool ok = ctrlRecvBuf.Poll(ctrlSock);
		for (auto& raw : ctrlRecvBuf.ready) {
			message_info u = Unpack_Message(raw);
			if (debug_net) LogPkt("CTRL RX ←", clHost, clServerPort, u);
			HandleClientMsg(raw);
		}
		ctrlRecvBuf.ready.clear();
		if (!ok) {
			if (m_network_debug)
				debug_net_printf("PollCtrlSocket: server disconnected\n");

			std::lock_guard<std::mutex> lk(ctrlSendMtx);
			CLOSE_SOCKET(ctrlSock); ctrlSock = SOCK_INVALID;
			IpPortIsSet = false;
		}
	}

	// ---------------------------------------------------------------------------
	// Accept incoming peer data connections (both roles)
	// ---------------------------------------------------------------------------
	void NetworkClass::AcceptPeerConnections()
	{
		if (peerListenSock == SOCK_INVALID) return;
		while (true) {
			sockaddr_in from{}; socklen_t fl = sizeof(from);
			socket_t s = accept(peerListenSock, (sockaddr*)&from, &fl);
			if (s == SOCK_INVALID) break;
			set_nonblocking(s); set_nodelay(s);
			char ip[INET_ADDRSTRLEN]; inet_ntop(AF_INET, &from.sin_addr, ip, sizeof(ip));
			int callerClPort = ntohs(from.sin_port); // fallback na ephemeral
			for (int i = 0; i < (int)NetworkName.size(); i++)
				if (clientIpPort[i].adress == std::string(ip)) { callerClPort = clientIpPort[i].port; break; }
			if (m_network_debug)
				debug_net_printf("AcceptPeer: data connection from %s\n", ip);

			// Find the LISTEN NCB that is waiting for an incoming data connection.
			// setListen() (called from PollCtrlSocket → HandleClientMsg a few lines above
			// in locUpdateNetworkSingleThread) sets ncb_lsn_2 = 20 before we get here,
			// so this lookup should always succeed when the LISTEN_ACCEPT and the TCP
			// connect() from the caller arrive in the same tick (true on loopback).
			// If for some reason it hasn't run yet, close this socket and let the
			// caller reconnect on the next tick.
			myNCB* listenNCB = nullptr;
			{
				std::lock_guard<std::mutex> lk(clientConnMutex);
				for (auto* c : clientConnection) {
					if (c->ncb_lsn_2 == 0) continue;
					bool hasAlive = false;
					{
						std::lock_guard<std::mutex> slk(sessions_mutex);
						auto it = tcpSessions.find(c);
						hasAlive = (it != tcpSessions.end() && it->second->alive);
					}
					if (!hasAlive) { listenNCB = c; break; }
				}
			}

			if (!listenNCB) {
				// setListen hasn't run yet for this NCB — reject and let caller retry
				if (m_network_debug)
					debug_net_printf("AcceptPeer: WARNING no listenNCB ready, closing incoming connection from %s (will retry)\n", ip);

				CLOSE_SOCKET(s);
				continue;
			}

			auto sess = std::make_shared<TcpSession>();
			sess->sock = s;
			sess->peerAddr = std::string(ip);
			sess->peerPort = callerClPort;
			sess->alive = true;
			sess->ownerNCB = listenNCB;
			sess->Touch();
			AddSession(listenNCB, sess);
			listenNCB->ncb_cmd_cplt_49 = NRC_GOODRET;
			if (m_network_debug)
				debug_net_printf("AcceptPeer: session listenNCB=%p name=[%.16s] call=[%.16s]\n", (void*)listenNCB, listenNCB->ncb_name_26, listenNCB->ncb_callName_10);

		}
	}

	// ---------------------------------------------------------------------------
	// Poll all data sessions
	// ---------------------------------------------------------------------------
	void NetworkClass::PollSessions()
	{
		std::vector<std::shared_ptr<TcpSession>> snaps;
		{
			std::lock_guard<std::mutex> lk(sessions_mutex);
			for (auto& kv : tcpSessions) snaps.push_back(kv.second);
		}

		for (auto& sess : snaps) {
			if (!sess->alive) continue;

			bool ok = sess->rbuf.Poll(sess->sock);

			// Fault injection for testing: hold arrivals back for a while instead of
			// acting on them at once.  Only the arrival time is affected - nothing is
			// discarded or reordered, because TCP cannot do either.  The wait is recorded
			// per message rather than slept through here, so this thread keeps polling,
			// answering heartbeats and running the NCB passes; sleeping in this loop would
			// freeze our own networking instead of simulating a silent peer.
			if (NetFaults::Enabled() && !sess->rbuf.ready.empty()) {
				auto now = std::chrono::steady_clock::now();
				for (auto& raw : sess->rbuf.ready)
					sess->heldMessages.push_back({ raw, now + std::chrono::milliseconds(NetFaults::NextHoldMs()) });
				sess->rbuf.ready.clear();
				auto due = std::chrono::steady_clock::now();
				while (!sess->heldMessages.empty() && sess->heldMessages.front().releaseAt <= due) {
					sess->rbuf.ready.push_back(sess->heldMessages.front().raw);
					sess->heldMessages.pop_front();
				}
			}

			for (auto& raw : sess->rbuf.ready) {
				sess->Touch();
				message_info u = Unpack_Message(raw);
				if (debug_net) LogPkt("DATA RX ←", sess->peerAddr, sess->peerPort, u);

				if (u.message == MESS_DIRECT_SEND) {
					size_t depth;
					{
						std::lock_guard<std::mutex> qlk(sess->queueMtx);
						sess->dataQueue.push(raw);
						depth = sess->dataQueue.size();
					}
					if (m_network_debug)
						debug_net_printf("PollSessions: queued DIRECT_SEND size=%u (queue=%zu)\n", u.size, depth);

					// A backlog means the game is reading one message per turn while the
					// peer already produced the next one, so from here on every RECEIVE
					// hands over data one turn old.  That is harmless for the lockstep
					// itself (both nodes still apply the same array, just later), but it is
					// worth reporting: stale packets must NOT be dropped to "catch up",
					// because skipping one would make this node apply a different sequence
					// of inputs than its peer - a real divergence.
					if (depth > 1) {
						static int s_backlogReports = 0;
						if (s_backlogReports < 10) {
							s_backlogReports++;
							debug_net_printf("PollSessions: NOTE data queue depth %zu for peer %s:%d - "
								"receives now run one turn behind\n", depth, sess->peerAddr.c_str(), sess->peerPort);
						}
					}
				}
				else if (u.message == MESS_HEARTBEAT) {
					shadow_myNCB n{}; n.ncb_command_0 = 0xFE;
					sess->Send(Pack_Message(MESS_HEARTBEAT_ACK, n, 0, clPort));
					if (m_network_debug)
						debug_net_printf("PollSessions: HEARTBEAT → ACK\n");

				}
				else if (u.message == MESS_HEARTBEAT_ACK) {
					if (m_network_debug)
						debug_net_printf("PollSessions: HEARTBEAT_ACK\n");

				}
			}
			sess->rbuf.ready.clear();

			if (!ok) {
				if (m_network_debug)
					debug_net_printf("PollSessions: peer %s:%d disconnected\n", sess->peerAddr.c_str(), sess->peerPort);

				sess->alive = false;
				if (sess->ownerNCB) {
					sess->ownerNCB->ncb_retcode_1 = NRC_SCLOSED;
					sess->ownerNCB->ncb_cmd_cplt_49 = NRC_SCLOSED;
				}
				continue;
			}

			// Heartbeat
			long silence = sess->SilenceMs();
			if (silence >= HEARTBEAT_TIMEOUT_MS) {
				if (m_network_debug)
					debug_net_printf("PollSessions: timeout %ldms peer=%s:%d\n", silence, sess->peerAddr.c_str(), sess->peerPort);

				sess->alive = false;
				if (sess->ownerNCB) {
					sess->ownerNCB->ncb_retcode_1 = NRC_SCLOSED;
					sess->ownerNCB->ncb_cmd_cplt_49 = NRC_SCLOSED;
				}
				sess->Close();
			}
			else if (silence >= HEARTBEAT_PROBE_MS && !sess->probePending) {
				long pSil = (long)std::chrono::duration_cast<std::chrono::milliseconds>(
					std::chrono::steady_clock::now() - sess->lastProbeSent).count();
				if (pSil >= HEARTBEAT_PROBE_MS) {
					sess->probePending = true;
					sess->lastProbeSent = std::chrono::steady_clock::now();
					shadow_myNCB n{}; n.ncb_command_0 = 0xFE;
					sess->Send(Pack_Message(MESS_HEARTBEAT, n, 0, clPort));
					if (m_network_debug)
						debug_net_printf("PollSessions: HEARTBEAT probe silence=%ldms\n", silence);

				}
			}
		}
	}

	// ---------------------------------------------------------------------------
	bool NetworkClass::ConnectDataToPeer(myNCB* ncb, const std::string& addr, int dataPort)
	{
		socket_t s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (s == SOCK_INVALID) return false;
		set_nodelay(s);
		sockaddr_in a{}; a.sin_family = AF_INET;
		a.sin_port = htons((unsigned short)dataPort);
		inet_pton(AF_INET, addr.c_str(), &a.sin_addr);
		if (m_network_debug)
			debug_net_printf("ConnectDataToPeer: %s:%d for ncb=[%.16s]\n", addr.c_str(), dataPort, ncb->ncb_name_26);

		if (connect(s, (sockaddr*)&a, sizeof(a)) != 0) {
			int e = sock_errno();
			if (!would_block(e))
			{
				CLOSE_SOCKET(s);
				if (m_network_debug)
					debug_net_printf("ConnectDataToPeer: failed err=%d\n", e);

				return false;
			}
		}
		set_nonblocking(s);
		auto sess = std::make_shared<TcpSession>();
		sess->sock = s;
		sess->peerAddr = addr;
		sess->peerPort = dataPort;
		sess->alive = true;
		sess->ownerNCB = ncb;
		sess->Touch();
		AddSession(ncb, sess);
		return true;
	}

	// ---------------------------------------------------------------------------
	// Server message handler
	// ---------------------------------------------------------------------------
	void NetworkClass::HandleServerMsg(const std::string& raw,
		const std::string& senderAddr,
		int                senderDataPort)
	{
		message_info u = Unpack_Message(raw);

		if (u.message == MESS_CLIENT_GET_IP) {
			shadow_myNCB n{}; n.ncb_command_0 = 0xFE;
			std::string reply = Pack_Message(MESS_SERVER_GIVE_IP, n, -1, u.port,
				senderAddr.c_str(), (int)senderAddr.size() + 1);
			SendToCtrlClient(reply, senderAddr, u.port);
			// A node that connects after the first name was registered would never see
			// the announcement sent at that moment, so repeat the current state here.
			if (serverAddname)
				SendToCtrlClient(Pack_Message(MESS_SERVER_GIVE_IP, n, SERVER_NAME_REGISTERED, clPort),
					senderAddr, u.port);
			return;
		}

		if (u.message == MESS_CLIENT_TESTADDNAME) {
			TypeIpPort ip{ senderAddr, u.port };
			shadow_myNCB n{}; n.ncb_command_0 = 0xFE;
			if (GetNameNetwork(u.data).empty()) {
				AddNetworkName(u.data, ip);
				SendToCtrlClient(Pack_Message(MESS_SERVER_TESTADDNAME_OK, n, u.index, -10), senderAddr, senderDataPort);
				char cmp[16] = { 0 };
				sprintf(cmp, "NETH2%c0", u.data[5]);
				while (strlen(cmp) < 15) strcat(cmp, " ");
				if (memcmp(u.data, cmp, 15) == 0) {
					serverAddname = true;
					// Tell every node that the session's first name (…0) is now taken.
					// This is what ReceiveServerAddName() reports, and it is how a client
					// can wait for the server to claim its name before claiming its own.
					// The flag used to be raised here and never sent anywhere, so
					// ReceiveServerAddName() always answered false and the wait built on
					// it could never finish.
					std::lock_guard<std::recursive_mutex> lk(ctrlClientsMtx);
					shadow_myNCB sn{}; sn.ncb_command_0 = 0xFE;
					std::string note = Pack_Message(MESS_SERVER_GIVE_IP, sn, SERVER_NAME_REGISTERED, clPort);
					for (auto& cc : ctrlClients)
						if (cc.sock != SOCK_INVALID) TcpSendFull(cc.sock, note);
					receiveServerAddName = true; // also true for the node hosting the session
				}
			}
			else if (ExistNetworkName(u.data, ip)) {
				SendToCtrlClient(Pack_Message(MESS_SERVER_TESTADDNAME_OK, n, u.index, -10), senderAddr, senderDataPort);
			}
			else {
				SendToCtrlClient(Pack_Message(MESS_SERVER_TESTADDNAME_REJECT, n, u.index, -10), senderAddr, senderDataPort);
			}
		}
		else if (u.message == MESS_CLIENT_MESSAGE_LISTEN) {
			AddListenName((myNCB*)u.data);
		}
		else if (u.message == MESS_CLIENT_MESSAGE_CALL) {
			TypeIpPort callIP = GetIpPortFromName(u.messNCB.ncb_callName_10);
			if (AddListenName2(&u.messNCB)) {
				// Tell caller: listener IP + data port
				// If listener registered via loopback, use the server's real LAN IP as seen by caller
				std::string listenerIp = callIP.adress;
				if (listenerIp == "127.0.0.1" || listenerIp == "::1") {
					for (auto& cc2 : ctrlClients) {
						if (cc2.addr == senderAddr && cc2.dataPort == senderDataPort) {
							sockaddr_in la{}; socklen_t ll = sizeof(la);
							if (getsockname(cc2.sock, (sockaddr*)&la, &ll) == 0) {
								char rip[INET_ADDRSTRLEN];
								inet_ntop(AF_INET, &la.sin_addr, rip, sizeof(rip));
								if (strcmp(rip, "0.0.0.0") != 0) listenerIp = rip;
							}
							break;
						}
					}
				}
				struct CallAcceptInfo { char ip[64]; int dataPort; };
				CallAcceptInfo ci{}; strncpy(ci.ip, listenerIp.c_str(), 63); ci.dataPort = callIP.port;
				SendToCtrlClient(Pack_Message(MESS_SERVER_CALL_ACCEPT, u.messNCB, u.index, -10,
					(char*)&ci, sizeof(ci)),
					senderAddr, senderDataPort);

				// Tell listener: caller IP + data port
				struct ListenAcceptInfo { char ncb_data[sizeof(myNCB)]; char ip[64]; int dataPort; };
				ListenAcceptInfo li{};
				memcpy(li.ncb_data, u.data, u.size < (int)sizeof(li.ncb_data) ? u.size : sizeof(li.ncb_data));
				strncpy(li.ip, senderAddr.c_str(), 63);
				li.dataPort = u.port; // caller's clPort

				// Route LISTEN_ACCEPT to the listener's control socket
				// (ctrlClientsMtx already held recursively by PollCtrlClients caller)
				std::lock_guard<std::recursive_mutex> lk(ctrlClientsMtx);
				for (auto& cc : ctrlClients) {
					if (cc.addr == callIP.adress && cc.dataPort == callIP.port) {
						TcpSendFull(cc.sock,
							Pack_Message(MESS_SERVER_LISTEN_ACCEPT, u.messNCB, u.index,
								clServerPort, (char*)&li, sizeof(li)));
						break;
					}
				}
			}
			else {
				shadow_myNCB n{}; n.ncb_command_0 = 0xFE;
				SendToCtrlClient(Pack_Message(MESS_SERVER_CALL_REJECT, u.messNCB, u.index, -10), senderAddr, senderDataPort);
			}
		}
		else if (u.message == MESS_CLIENT_CANCEL) {
			if (u.data[0] != 0) RemoveListenName(u.data);
		}
		else if (u.message == MESS_CLIENT_DELETE) {
			CleanMessages(myNCBfromShadow(u.messNCB));
			RemoveNetworkName(u.data);
		}
	}

	// ---------------------------------------------------------------------------
	// Client message handler (replies from the server)
	// ---------------------------------------------------------------------------
	void NetworkClass::HandleClientMsg(const std::string& raw)
	{
		message_info u = Unpack_Message(raw);

		if (u.message == MESS_SERVER_GIVE_IP) {
			if (u.index == SERVER_NAME_REGISTERED) {
				// Not an address reply: the server is announcing that the session's
				// first NetBIOS name is registered.  ReceiveServerAddName() reports this.
				receiveServerAddName = true;
				if (m_network_debug)
					debug_net_printf("HandleClient: server name registered\n");
				return;
			}
			IpPortIsSet = true;
			if (m_network_debug)
				debug_net_printf("HandleClient: GIVE_IP — confirmed\n");

			return;
		}

		if (u.message == MESS_SERVER_TESTADDNAME_OK) {
			std::lock_guard<std::mutex> lk(connections_mutex);
			connectionTime* ct = GetConnection(u.index);
			if (ct) ct->state = NETI_ADD_NAME_OK;
		}
		else if (u.message == MESS_SERVER_TESTADDNAME_REJECT) {
			std::lock_guard<std::mutex> lk(connections_mutex);
			connectionTime* ct = GetConnection(u.index);
			if (ct) ct->state = NETI_ADD_NAME_REJECT;
		}
		else if (u.message == MESS_SERVER_CALL_ACCEPT) {
			std::lock_guard<std::mutex> lk(connections_mutex);
			connectionTime* ct = GetConnection(u.index);
			if (ct) {
				ct->state = NETI_CALL_ACCEPT;
				ct->connection->ncb_retcode_1 = NRC_GOODRET;
				ct->connection->ncb_cmd_cplt_49 = NRC_GOODRET;
				if (m_network_debug)
					debug_net_printf("HandleClient: CALL_ACCEPT idx=%d name=[%.16s]\n", u.index, ct->connection->ncb_name_26);

				struct { char ip[64]; int dataPort; } ci{};
				if (u.size >= (int)sizeof(ci)) {
					memcpy(&ci, u.data, sizeof(ci));
					if (ci.ip[0] && ci.dataPort > 0) {
						std::string key(u.messNCB.ncb_callName_10, 16);
						directPeers[key] = { std::string(ci.ip), ci.dataPort };
						if (m_network_debug)
							debug_net_printf("HandleClient: CALL_ACCEPT peer=[%.16s] %s:%d\n", u.messNCB.ncb_callName_10, ci.ip, ci.dataPort);

						ConnectDataToPeer(ct->connection, std::string(ci.ip), ci.dataPort);
						// Register CALL NCB in clientConnection so Pass4 tracks
						// liveness the same way as LISTEN NCBs.
						// ncb_lsn_2=20 is required for Pass4 to consider it established.
						ct->connection->ncb_lsn_2 = 20;
						setListenConnection(ct->connection);
					}
				}
			}
		}
		else if (u.message == MESS_SERVER_CALL_REJECT) {
			std::lock_guard<std::mutex> lk(connections_mutex);
			connectionTime* ct = GetConnection(u.index);
			if (ct) ct->state = NETI_CALL_REJECT;
		}
		else if (u.message == MESS_SERVER_LISTEN_ACCEPT) {
			// tmp.ncb_name_26 must hold the CALLER's name so setListen can match it
			// against the LISTEN NCB's ncb_callName_10 (= who we were waiting for).
			// u.messNCB.ncb_name_26 IS the caller's name ("NETH201").
			// u.messNCB.ncb_callName_10 is the LISTENER's name ("NETH200") — wrong field.
			myNCB tmp; memset(&tmp, 0, sizeof(tmp));
			memcpy(tmp.ncb_name_26, u.messNCB.ncb_name_26, 16);  // caller's name
			if (m_network_debug)
				debug_net_printf("HandleClient: LISTEN_ACCEPT callerName=[%.16s] listenerName=[%.16s]\n", u.messNCB.ncb_name_26, u.messNCB.ncb_callName_10);

			setListen(&tmp);

			// Record caller's data port
			struct { char ncb_data[sizeof(myNCB)]; char ip[64]; int dataPort; } li{};
			if (u.size >= (int)sizeof(li)) {
				memcpy(&li, u.data, sizeof(li));
				if (li.ip[0] && li.dataPort > 0) {
					std::string key(u.messNCB.ncb_name_26, 16);
					directPeers[key] = { std::string(li.ip), li.dataPort };
					if (m_network_debug)
						debug_net_printf("HandleClient: LISTEN_ACCEPT caller=%s:%d\n", li.ip, li.dataPort);

					// Incoming TCP connection accepted by AcceptPeerConnections()
				}
			}
		}
		else if (u.message == MESS_SERVER_LISTEN_REJECT) {
			// Nothing; LISTEN keeps waiting
		}
	}

	// ---------------------------------------------------------------------------
	void NetworkClass::locUpdateNetworkSingleThread()
	{
		if (clIam_client && ctrlSock == SOCK_INVALID) ConnectToServer();
		if (clIam_server) { AcceptCtrlClients(); PollCtrlClients(); }
		if (clIam_client) PollCtrlSocket();
		AcceptPeerConnections();
		PollSessions();
		if (clIam_client) UpdateClient();
	}

	// ---------------------------------------------------------------------------
	// UpdateClient — NetBIOS NCB state machine
	// ---------------------------------------------------------------------------
	void NetworkClass::UpdateClient()
	{
		if (!IpPortIsSet) {
			// The control link is not up (yet, or any more): keep asking the server for
			// our address, but do NOT skip the NCB state machine below.
			//
			// Returning here - as this used to do - stopped Pass 2 from ever completing a
			// command once the server had gone away.  Network.cpp busy-waits on
			// ncb_cmd_cplt_49 for HANG_UP, DELETE_NAME and CANCEL, so leaving the game
			// after the server quit first froze the whole process: the command it was
			// spinning on could never be completed by anyone.
			static clock_t lastProbe = 0;
			clock_t nowProbe = clock();
			if (lastProbe == 0 || (long)((nowProbe - lastProbe) * 1000 / CLOCKS_PER_SEC) >= 250) {
				lastProbe = nowProbe;
				shadow_myNCB n{}; n.ncb_command_0 = 0xFE;
				SendCtrl(Pack_Message(MESS_CLIENT_GET_IP, n, GetNextIndex(), clPort));
			}
		}

		std::lock_guard<std::mutex> conn_lk(connections_mutex);
		std::vector<int32_t> toDelete;
		clock_t now = clock();

		// ------------------------------------------------------------------
		// Pass 1 — deliver RECEIVE operations from session data queues.
		// A RECEIVE NCB is matched to a session by comparing ncb_name_26
		// (the local NetBIOS name) with the session ownerNCB->ncb_name_26.
		// This correctly routes incoming data to the right side (NETH200 or
		// NETH201) without any size-matching magic.
		// ------------------------------------------------------------------
		for (connectionTime& conn : handleConnections)
		{
			if (conn.connection->ncb_command_0 != 0x95) continue;
			if (conn.connection->ncb_cmd_cplt_49 != NRC_PENDING) continue;
			if (!conn.connection->ncb_buffer_4.p) continue;

			// Find a session whose owner has the same local name
			std::shared_ptr<TcpSession> sess;
			bool deadPeerForName = false; // a matching session exists but the peer is gone
			{
				std::lock_guard<std::mutex> lk(sessions_mutex);
				for (auto& kv : tcpSessions) {
					if (!kv.second->ownerNCB) continue;
					if (memcmp(kv.second->ownerNCB->ncb_name_26,
						conn.connection->ncb_name_26, 16) != 0) continue;
					if (kv.second->alive) { sess = kv.second; break; }
					deadPeerForName = true;
				}
			}
			if (!sess) {
				// If the only session for this local name is dead (peer disconnected
				// or was banished), fail the blocking RECEIVE instead of spinning
				// forever in NetworkReceivePacket_74C9D().  A normal re-negotiation
				// gap has NO session at all (HANG_UP removes it), so this only fires
				// on a genuinely lost peer and does not disturb reconnect.
				if (deadPeerForName) {
					conn.connection->ncb_retcode_1 = NRC_SCLOSED;
					conn.connection->ncb_cmd_cplt_49 = NRC_SCLOSED;
					toDelete.push_back(conn.index);
				}
				if (m_network_debug)
					debug_net_printf("Pass1: RECEIVE idx=%d no session name=[%.16s] deadPeer=%d\n", conn.index, conn.connection->ncb_name_26, (int)deadPeerForName);

				continue;
			}

			std::string raw;
			{
				std::lock_guard<std::mutex> qlk(sess->queueMtx);
				if (sess->dataQueue.empty()) continue;
				raw = sess->dataQueue.front();
				sess->dataQueue.pop();
			}

			message_info u = Unpack_Message(raw);
			if (u.size > conn.connection->ncb_bufferLength_8) {
				if (m_network_debug)
					debug_net_printf("Pass1: RECEIVE idx=%d size=%u > buf=%u DROP\n", conn.index, u.size, conn.connection->ncb_bufferLength_8);

				continue;
			}
			memcpy(conn.connection->ncb_buffer_4.p, u.data, u.size);
			conn.connection->ncb_bufferLength_8 = (uint16_t)u.size;
			conn.connection->ncb_retcode_1 = NRC_GOODRET;
			conn.connection->ncb_cmd_cplt_49 = NRC_GOODRET;
			toDelete.push_back(conn.index);
			if (m_network_debug)
				debug_net_printf("Pass1: RECEIVE idx=%d DELIVERED size=%u from=[%.16s] to=[%.16s]\n", conn.index, u.size, u.messNCB.ncb_name_26, u.messNCB.ncb_callName_10);

		}

		// ------------------------------------------------------------------
		// Pass 2 — timeout / retry per NCB command
		// ------------------------------------------------------------------
		for (connectionTime& conn : handleConnections)
		{
			if (conn.connection->ncb_cmd_cplt_49 != NRC_PENDING &&
				conn.connection->ncb_command_0 != 0x91 &&
				conn.connection->ncb_command_0 != 0x95) continue;

			long elapsed = (long)((now - conn.startTime) * 1000 / CLOCKS_PER_SEC);
			if (elapsed < conn.timeout) continue;

			bool del = true;
			switch (conn.connection->ncb_command_0)
			{
			case 0x35: // CANCEL
				conn.connection->ncb_retcode_1 = NRC_GOODRET;
				conn.connection->ncb_cmd_cplt_49 = NRC_GOODRET;
				break;
			case 0x7F: // INIT
				conn.connection->ncb_cmd_cplt_49 = NRC_GOODRET;
				break;
			case 0x90: // CALL
				if (conn.state == NETI_CALL_ACCEPT) {
					conn.connection->ncb_retcode_1 = NRC_GOODRET;
					conn.connection->ncb_cmd_cplt_49 = NRC_GOODRET;
				}
				else if (conn.state == NETI_CALL_REJECT || conn.retryCount >= MAX_RETRIES) {
					conn.connection->ncb_cmd_cplt_49 = NRC_SCLOSED;
				}
				else {
					conn.startTime = now; conn.retryCount++; del = false;
					CallNetwork(conn.connection, conn.index);
				}
				break;
			case 0x91: // LISTEN — stays pending
				del = false;
				break;
			case 0x92: // HANG_UP
				conn.connection->ncb_lsn_2 = 0;
				conn.connection->ncb_cmd_cplt_49 = NRC_GOODRET;
				RemoveSession(conn.connection);
				deleteListenConnection(conn.connection);
				break;
			case 0x94: // SEND — complete immediately (TCP send already done)
				conn.connection->ncb_retcode_1 = NRC_GOODRET;
				conn.connection->ncb_cmd_cplt_49 = NRC_GOODRET;
				break;
			case 0x95: // RECEIVE — never times out
				del = false;
				break;
			case 0xb0: // ADD_NAME
				if (conn.state == NETI_ADD_NAME_OK) {
					conn.connection->ncb_retcode_1 = NRC_GOODRET;
					conn.connection->ncb_cmd_cplt_49 = NRC_GOODRET;
				}
				else if (conn.state == NETI_ADD_NAME_REJECT) {
					conn.connection->ncb_retcode_1 = NRC_INUSE;
					conn.connection->ncb_cmd_cplt_49 = NRC_INUSE;
				}
				else {
					conn.startTime = now; del = false;
					AddName(conn.connection, conn.index);
				}
				break;
			case 0xb1: // DELETE_NAME
				conn.connection->ncb_cmd_cplt_49 = NRC_GOODRET;
				break;
			default:
				conn.connection->ncb_cmd_cplt_49 = NRC_GOODRET;
				break;
			}
			if (del) toDelete.push_back(conn.index);
		}

		for (int32_t idx : toDelete)
			for (auto it = handleConnections.begin(); it != handleConnections.end(); )
				it = (it->index == idx) ? handleConnections.erase(it) : ++it;

		// ------------------------------------------------------------------
		// Pass 4 — session liveness → ncb_cmd_cplt_49
		// ------------------------------------------------------------------
		std::vector<myNCB*> connSnap;
		{ std::lock_guard<std::mutex> clk(clientConnMutex); connSnap = clientConnection; }
		for (auto* c : clientConnection)
		{
			if (c->ncb_lsn_2 == 0 || c->ncb_cmd_cplt_49 == NRC_PENDING) continue;
			auto sess = GetSession(c);
			bool alive = sess && sess->alive && sess->SilenceMs() < HEARTBEAT_TIMEOUT_MS;
			uint8_t old = c->ncb_cmd_cplt_49;
			if (alive) {
				// Only revive an NCB that liveness detection had previously marked closed.
				//
				// Writing NRC_GOODRET unconditionally used to race with the game thread:
				// the check above can read "not pending" a moment before the game arms a
				// new RECEIVE (setting NRC_PENDING), and the write then completed that
				// RECEIVE with no data attached.  Network.cpp only busy-waits for
				// ncb_cmd_cplt_49 to change, so it accepted the empty completion, left the
				// message sitting in the queue and carried on - sending its own update
				// anyway.  Every occurrence pushed the reader one message further behind,
				// which is why the token holder ended up ~25 turns late and never saw the
				// last actions of a peer that then disconnected.
				if (old == NRC_SCLOSED)
					c->ncb_cmd_cplt_49 = NRC_GOODRET;
			}
			else if (c->ncb_cmd_cplt_49 != NRC_SCLOSED) {
				c->ncb_cmd_cplt_49 = NRC_SCLOSED;
				c->ncb_retcode_1 = NRC_SCLOSED;
				if (m_network_debug)
					debug_net_printf("Pass4: CLOSED name=[%.16s] lsn=%d\n", c->ncb_name_26, c->ncb_lsn_2);

			}
			if (m_network_debug)
				debug_net_printf("Pass4: lsn=%d name=[%.16s] alive=%d cplt %02X→%02X\n", c->ncb_lsn_2, c->ncb_name_26, (int)alive, old, c->ncb_cmd_cplt_49);

		}
	}

	// ---------------------------------------------------------------------------
	// Public NCB dispatchers
	// ---------------------------------------------------------------------------
	void NetworkClass::AddName(myNCB* c, int32_t index)
	{
		SendCtrl(Pack_Message(MESS_CLIENT_TESTADDNAME, myNCBtoShadow(*c), index, clPort,
			c->ncb_name_26, sizeof(c->ncb_name_26)));
	}

	void NetworkClass::CancelNetwork(myNCB* c, int32_t index)
	{
		SendCtrl(Pack_Message(MESS_CLIENT_CANCEL, myNCBtoShadow(*c), index, clPort,
			c->ncb_name_26, sizeof(c->ncb_name_26)));
		deleteListenConnection(c); // removes from clientConnection (LISTEN or CALL NCB)
		RemoveSession(c);          // closes TCP data session
	}

	void NetworkClass::DeleteNetwork(myNCB* c, int32_t index)
	{
		SendCtrl(Pack_Message(MESS_CLIENT_DELETE, myNCBtoShadow(*c), index, clPort,
			c->ncb_name_26, sizeof(c->ncb_name_26)));
		locUpdateNetworkSingleThread();
		CleanMessages(*c);
	}

	void NetworkClass::CallNetwork(myNCB* c, int32_t index)
	{
		myNCB safe = *c; safe.ncb_buffer_4.p = nullptr;
		SendCtrl(Pack_Message(MESS_CLIENT_MESSAGE_CALL, myNCBtoShadow(*c), index, clPort,
			(char*)&safe, sizeof(safe)));
	}

	void NetworkClass::ListenNetwork(myNCB* c, int32_t index)
	{
		if (m_network_debug)
			debug_net_printf("ListenNetwork: name=[%.16s] call=[%.16s]\n", c->ncb_name_26, c->ncb_callName_10);

		myNCB safe = *c; safe.ncb_buffer_4.p = nullptr;
		SendCtrl(Pack_Message(MESS_CLIENT_MESSAGE_LISTEN, myNCBtoShadow(*c), index, clPort,
			(char*)&safe, sizeof(safe)));
		setListenConnection(c);
	}

	void NetworkClass::SendNetwork(myNCB* c, int32_t index)
	{
		assert(c->ncb_command_0 == 0x94);

		// Find session by matching sender's ncb_name_26 to ownerNCB->ncb_name_26
		std::shared_ptr<TcpSession> sess;
		{
			std::lock_guard<std::mutex> lk(sessions_mutex);
			for (auto& kv : tcpSessions) {
				if (!kv.second->alive || !kv.second->ownerNCB) continue;
				if (memcmp(kv.second->ownerNCB->ncb_name_26, c->ncb_name_26, 16) == 0)
				{
					sess = kv.second; break;
				}
			}
		}

		if (!sess) {
			if (m_network_debug)
				debug_net_printf("SendNetwork: no session for name=[%.16s]\n", c->ncb_name_26);

			c->ncb_cmd_cplt_49 = NRC_SCLOSED;
			return;
		}

		std::string packed = Pack_Message(MESS_DIRECT_SEND, myNCBtoShadow(*c), index, clPort,
			(char*)c->ncb_buffer_4.p, c->ncb_bufferLength_8);
		if (!sess->Send(packed)) {
			if (m_network_debug)
				debug_net_printf("SendNetwork: TCP send failed for name=[%.16s]\n", c->ncb_name_26);

			sess->alive = false;
			c->ncb_cmd_cplt_49 = NRC_SCLOSED;
			return;
		}
		// TCP is reliable — mark SEND complete immediately
		c->ncb_retcode_1 = NRC_GOODRET;
		c->ncb_cmd_cplt_49 = NRC_GOODRET;
		if (m_network_debug)
			debug_net_printf("SendNetwork: sent size=%u name=[%.16s]\n", c->ncb_bufferLength_8, c->ncb_name_26);

	}

} // namespace MyNetworkLib

// ---------------------------------------------------------------------------
// Global instance
// ---------------------------------------------------------------------------
MyNetworkLib::NetworkClass* locNetworkClass = nullptr;
std::string                 IHaveNameStrP = "";

// ---------------------------------------------------------------------------
// simulateInterupt — called by setNetbios_75044() in Network.cpp
// ---------------------------------------------------------------------------
int lastnetworkname = 0;

void simulateInterupt(myNCB* connection)
{
	int       locTimeout = 0;
	int32_t   locIndex = GetNextIndex();
	Neti_type state = NETI_NOT_SET;

	switch (connection->ncb_command_0)
	{
	case 0x35:  // CANCEL
		locTimeout = 10000;
		{
			myNCB* second = (myNCB*)connection->ncb_buffer_4.p;
			if (second) { second->ncb_retcode_1 = NRC_CMDCAN; second->ncb_cmd_cplt_49 = NRC_CMDCAN; }
		}
		connection->ncb_retcode_1 = NRC_GOODRET;
		connection->ncb_lsn_2 = 0;
		connection->ncb_cmd_cplt_49 = NRC_GOODRET;
		locNetworkClass->CancelNetwork(connection, locIndex);
		break;
	case 0x7F:  // INIT
		locTimeout = 200;
		connection->ncb_retcode_1 = 0x03;
		connection->ncb_cmd_cplt_49 = 0x03;
		break;
	case 0x90:  // CALL
		locTimeout = 1000;
		connection->ncb_retcode_1 = NRC_PENDING;
		connection->ncb_lsn_2 = 0xE8;
		connection->ncb_cmd_cplt_49 = NRC_PENDING;
		locNetworkClass->CallNetwork(connection, locIndex);
		state = NETI_CALL;
		break;
	case 0x91:  // LISTEN
		locTimeout = 200;
		connection->ncb_retcode_1 = NRC_PENDING;
		connection->ncb_cmd_cplt_49 = NRC_PENDING;
		locNetworkClass->ListenNetwork(connection, locIndex);
		state = NETI_LISTEN;
		break;
	case 0x92:  // HANG_UP
		// Over TCP a HANG_UP just closes the data socket (RemoveSession in Pass 2) —
		// there is no round-trip to wait for.  The old 500 ms timeout was pure dead
		// time, and NetworkCanceling_73669() issues a HANG_UP for every one of the 8
		// player slots (even the unconnected ones), so a banish/disconnect stalled the
		// caller for ~7*500 ms = 3.5 s.  A short timeout removes that stall.
		locTimeout = 40;
		connection->ncb_cmd_cplt_49 = NRC_PENDING;
		break;
	case 0x94:  // SEND
		locTimeout = 5000;
		connection->ncb_retcode_1 = NRC_PENDING;
		connection->ncb_cmd_cplt_49 = NRC_PENDING;
		locNetworkClass->SendNetwork(connection, locIndex);
		break;
	case 0x95:  // RECEIVE
		locTimeout = 10000;
		connection->ncb_retcode_1 = NRC_PENDING;
		connection->ncb_cmd_cplt_49 = NRC_PENDING;
		state = NETI_RECEIVE;
		break;
	case 0xb0:  // ADD_NAME
		locTimeout = 1000;
		connection->ncb_retcode_1 = NRC_PENDING;
		connection->ncb_num_3 = lastnetworkname + 0x02;
		lastnetworkname++;
		connection->ncb_cmd_cplt_49 = NRC_PENDING;
		locNetworkClass->AddName(connection, locIndex);
		state = NETI_ADD_NAME;
		break;
	case 0xb1:  // DELETE_NAME
		// DELETE is fire-and-forget: DeleteNetwork() has already sent the
		// MESS_CLIENT_DELETE to the server over reliable TCP, so there is no
		// reply to wait for.  The old 10000 ms timeout made Pass 2 hold this
		// NCB in NRC_PENDING for a full 10 s, during which NetworkDeleteName_74A86()
		// busy-waits — that is the multi-second hang seen when a player is banished
		// (banish → NetworkEvent → NetworkCanceling → NetworkDeleteName).
		locTimeout = 100;
		connection->ncb_retcode_1 = NRC_PENDING;
		locNetworkClass->DeleteNetwork(connection, locIndex);
		break;
	}

	connectionTime lc;
	lc.connection = connection;
	lc.timeout = locTimeout;
	lc.startTime = clock();
	lc.index = locIndex;
	lc.state = state;
	lc.retryCount = 0;
	{ std::lock_guard<std::mutex> lk(connections_mutex); handleConnections.push_back(lc); }
}

// ---------------------------------------------------------------------------
// ResetNetworkGameState — drop all per-match state so a NEW match starts clean.
// Without this, a second match inherits stale data sessions / established-connection
// entries / pending NCB commands keyed by the (reused) NCB pointers from the first
// match; that made the peers issue redundant CALLs and clobber each other's data
// session, so they never reconnected (restart bug #3).  Sockets stay open.
// ---------------------------------------------------------------------------
void ResetNetworkGameState()
{
	{
		std::lock_guard<std::mutex> lk(sessions_mutex);
		for (auto& kv : tcpSessions) if (kv.second) kv.second->Close();
		tcpSessions.clear();
	}
	{ std::lock_guard<std::mutex> lk(clientConnMutex);   clientConnection.clear(); }
	{ std::lock_guard<std::mutex> lk(connections_mutex); handleConnections.clear(); }

	if (m_network_debug)
		debug_net_printf("ResetNetworkGameState: cleared sessions/connections/commands for new match\n");

}

// ---------------------------------------------------------------------------
// Debug helpers  (unchanged API)
// ---------------------------------------------------------------------------
void printState(myNCB** connections) {
	if (m_network_debug)
	{
		for (int i = 0; i < 3; i++)
			debug_net_printf("NetworkGetState: %d %p lsn=%d cplt=%s\n",
				i, connections[i], connections[i]->ncb_lsn_2,
				(!connections[i]->ncb_cmd_cplt_49) ? "ok" : "pending");
	}
}
void printState2(char* text) {
	if (m_network_debug)
		debug_net_printf("%s", text);
}
void timeState(bool start, const char* text) {
	if (m_network_debug)
	{
		/*
		if (start || (timest_index == 0)) timest_timer = clock();
		char buff[100];
		snprintf(buff, sizeof(buff), "%s | %d", text, (int)(clock() - timest_timer));
		timest_buffer[timest_index].assign(buff, strlen(buff));
		timest_index++;
		if (timest_index > timest_max_mess) {
			ofstream ofs("net_time_messages_log.txt", std::ofstream::out);
			for (int i = 0; i < timest_max_mess; i++)
				ofs << timest_buffer[i] << endl;
			ofs.close();
			exit(0);
		}*/
	}
}

// ---------------------------------------------------------------------------
// Init / shutdown  (unchanged API)
// ---------------------------------------------------------------------------
void InitMyNetLib(bool iam_server, bool iam_client,
	char* ip, int networkPort, int serverPort)
{
#ifdef _WIN32
	WSADATA wsa; WSAStartup(MAKEWORD(2, 2), &wsa);
#endif
	NetFaults::Configure(CommandLineParams.NetDelayMs(),
		CommandLineParams.NetJitterMs(),
		CommandLineParams.NetStallMs(),
		CommandLineParams.NetStallEvery());
	locNetworkClass = new MyNetworkLib::NetworkClass(
		iam_server, iam_client, ip, networkPort, serverPort);
	locNetworkClass->StartNetworkThread();
}

void EndMyNetLib()
{
	if (locNetworkClass) {
		locNetworkClass->StopNetworkThread();
		delete locNetworkClass;
		locNetworkClass = nullptr;
	}
#ifdef _WIN32
	WSACleanup();
#endif
}

void SetNetworkDebug()
{
	m_network_debug = true;
}
