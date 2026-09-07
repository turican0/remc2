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
#include "../engine/CommandLineParser.h"
// The game layer keeps its own idea of who the server is (engine/Network.cpp).  A hand-over
// has to move that too, not just the transport's own flag - see the TAKEOVER path below.
extern bool Iam_server;
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
#define NRC_BUFLEN   0x06    // message longer than the buffer offered by the RECEIVE
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
// First thing a caller says on a freshly opened peer connection.  Until now the two kinds
// of connection were told apart by the port they arrived on: control on the server's port,
// game data on each node's own.  One port has to carry both, so the connection now says
// what it is instead - a data connection opens with this, a control connection opens with
// one of the CLIENT_ messages above.  It also carries the caller's own port, which used to
// be guessed from the name table.
const int32_t MESS_PEER_HELLO = 21;
// The whole membership, in name order, sent by the server whenever it changes.  Every node
// keeps a copy: it is what a node that has to take over hosting seeds its name table from,
// and what everybody agrees the running order is, so the successor can be worked out
// without asking anyone.
const int32_t MESS_SERVER_ROSTER = 22;
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
			if (m_network_debug)
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
				if (m_network_debug)
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
	if (GetNameNetwork(name).empty()) {
		NetworkName.push_back(name); clientIpPort.push_back(ip);
		if (m_network_debug)
			debug_net_printf("NAMES: +[%.15s] %s:%d (%d total)\n",
				name.c_str(), ip.adress.c_str(), ip.port, (int)NetworkName.size());
	}
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
	if (m_network_debug)
		debug_net_printf("NAMES: -[%.15s] (%d left)\n",
			NetworkName[idx].c_str(), (int)NetworkName.size() - 1);
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
	// Say which of the three reasons turned a call down.  A rejected call is how a third
	// player ends up registered but never connected, and the reason is not guessable from
	// the outside.
	TypeIpPort id1 = GetIpPortFromName(c->ncb_callName_10);
	if (id1.adress == "x999") {
		if (m_network_debug)
			debug_net_printf("CALLREJ: no address for the listener [%.16s] (caller [%.16s])\n",
				c->ncb_callName_10, c->ncb_name_26);
		return false;
	}
	TypeIpPort id2 = GetIpPortFromName(c->ncb_name_26);
	if (id2.adress == "x999") {
		if (m_network_debug)
			debug_net_printf("CALLREJ: no address for the caller [%.16s] (listener [%.16s])\n",
				c->ncb_name_26, c->ncb_callName_10);
		return false;
	}
	int idx = GetNameListenIndex(c->ncb_name_26);
	if (idx < 0) {
		std::string armed;
		for (size_t i = 0; i < ListenName.size(); i++) {
			armed += "[" + TrimName(ListenName[i].c_str(), (int)ListenName[i].size()) + "->"
				+ TrimName(ListenName2[i].c_str(), (int)ListenName2[i].size()) + "] ";
		}
		if (m_network_debug)
			debug_net_printf("CALLREJ: nobody is listening for [%.16s]; armed listens: %s\n",
				c->ncb_name_26, armed.empty() ? "none" : armed.c_str());
		return false;
	}
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
	if (::bind(peerListenSock, (sockaddr*)&a, sizeof(a)) != 0) {
		int e = sock_errno();
		if (m_network_debug)
			debug_net_printf("PeerListen: data port %d unavailable (err=%d) - taking an OS-assigned port instead\n", port, e);
		sockaddr_in any{}; any.sin_family = AF_INET; any.sin_port = 0; any.sin_addr.s_addr = INADDR_ANY;
		if (::bind(peerListenSock, (sockaddr*)&any, sizeof(any)) != 0) {
			if (m_network_debug)
				debug_net_printf("PeerListen: FATAL - no data port could be bound (err=%d)\n", sock_errno());
			CLOSE_SOCKET(peerListenSock); peerListenSock = SOCK_INVALID; return false;
		}
		sockaddr_in got{}; socklen_t gl = sizeof(got);
		if (getsockname(peerListenSock, (sockaddr*)&got, &gl) != 0) {
			CLOSE_SOCKET(peerListenSock); peerListenSock = SOCK_INVALID; return false;
		}
		port = ntohs(got.sin_port);
		if (m_network_debug)
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
		if (m_network_debug)
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
	clock_t     lastRx = 0;   // last time anything arrived from this client
	clock_t     lastProbe = 0;
};

// One line of the membership as it goes over the wire.  Fixed-size fields keep the parsing
// on the far side trivial - there are at most eight of these.
#pragma pack(push, 1)
struct RosterEntry {
	char    name[16];
	char    ip[64];
	int32_t port;
};
#pragma pack(pop)

// Everyone's copy of the membership, in name order.
static std::vector<RosterEntry> knownRoster;
static std::mutex rosterMtx;

// A peer that drops out of the membership list is not coming back under that name, so any
// command still waiting for it has to be told.  Without this the slot it occupied keeps a
// LISTEN pending for ever (measured: "lsn=0 cplt=pending" on the host for the rest of the
// run), the game never re-arms it, and a newcomer can only be accommodated by taking a
// different slot - which is what handing out a fresh name did.  Releasing the slot is what
// lets a name be used again once its holder is gone.
static void ReleasePendingForName(const char* goneName)
{
	if (!goneName || !goneName[0]) return;
	std::string want = TrimName(goneName, (int)strlen(goneName));
	std::lock_guard<std::mutex> lk(connections_mutex);
	for (auto& c : handleConnections) {
		if (!c.connection) continue;
		std::string call = TrimName(c.connection->ncb_callName_10, 16);
		if (call != want) continue;
		if (c.connection->ncb_cmd_cplt_49 != NRC_PENDING) continue;
		c.connection->ncb_retcode_1 = NRC_SCLOSED;
		c.connection->ncb_cmd_cplt_49 = NRC_SCLOSED;
		if (m_network_debug)
			debug_net_printf("PEERGONE: released a pending command waiting for [%s]\n", want.c_str());
	}
}

// Names that were in the previous list and are missing from the new one.
static void ReleasePendingForVanished(const std::vector<RosterEntry>& before,
	const std::vector<RosterEntry>& after)
{
	for (auto& b : before) {
		bool stillThere = false;
		for (auto& a : after)
			if (strncmp(a.name, b.name, sizeof(a.name)) == 0) { stillThere = true; break; }
		if (!stillThere) ReleasePendingForName(b.name);
	}
}

// What this node is registered as, so it can find its own place in the membership list.
//
// Only ever set from the server's acceptance.  It used to be written when the name was
// ASKED for, and a name that comes back rejected then stays behind as ours: joining a
// second match, a node asked for NETH200 (refused - it belongs to the host) and NETH201
// (refused - the other survivor had just taken it) and was still working through the
// indices when the host died.  Both survivors then compared the successor's name with
// their own, both matched NETH201, and both declared themselves the new server: each
// seeded a membership list of one, they never opened a session, and the level was never
// started for want of a second player.
static std::string myNetName;
// The name asked for and not yet answered.
static std::string pendingNetName;

// Set when this node follows a hand-over: the new server has to be told who we are.
//
// Following one only re-points the address and reconnects, and the new server seeds its
// name table from the roster - so the newcomer shows up in the membership list at first and
// then falls out of it again, because that entry is not backed by a control client that has
// identified itself.  Measured after a hand-over in the lobby: the list said two members,
// then one, and from then on each survivor saw only itself, never opened a session with the
// other, and the level was never started for want of a second player.
static bool reRegisterWithNewServer = false;

// ---------------------------------------------------------------------------
// Connect / disconnect notice for the game to show on screen.
//
// Written here on the network thread and read by the drawing code on the game thread, so the
// text is copied out under the lock rather than handed over as a pointer into this string -
// the receive buffers were shared that way and it cost a heap-use-after-free (see the note on
// locUpdateNetworkSingleThread).
//
// A list rather than one line, because events come in bursts: everybody joins within a second
// of each other, and when a node goes away its peers all notice at once.  Each notice carries
// its own stamp and lives its own five seconds, so a new one appears BELOW those still up, and
// as the older ones expire from the front the rest move up a line.  Four at a time is as many
// as fit above the rest of the overlay; the oldest is dropped if more arrive.
static const long   NOTICE_SHOW_MS = 5000;  // how long one notice stays on screen
static const size_t NOTICE_MAX = 4;         // lines the display has room for
struct ConnectionNotice {
	std::string text;
	clock_t     at;
};
static std::mutex                   noticeMtx;
static std::deque<ConnectionNotice> notices;

// The peer is this same instance.  Every node on one machine answers on the loopback address,
// so the port is the only thing that tells them apart, and our own data port is ours.
static bool IsSelfPeer(const std::string& addr, int port, int ourPort)
{
	return port == ourPort && (addr == "127.0.0.1" || addr == "::1" || addr == "localhost");
}

static void PublishConnectionNotice(bool connected, const std::string& addr, int port, bool self)
{
	char line[128];
	snprintf(line, sizeof(line), "%s TO: %s:%d%s",
		connected ? "CONNECTED" : "DISCONNECTED", addr.c_str(), port, self ? " (self)" : "");
	std::lock_guard<std::mutex> lk(noticeMtx);
	notices.push_back({ line, clock() });
	while (notices.size() > NOTICE_MAX)
		notices.pop_front();

	EventDispatcher::I->DispatchEvent(EventType::E_SHOW_NETWORK_MESSAGE, std::string(line));
}

// Control-channel liveness.  A closed socket is noticed at once, but a node that stops
// answering while its socket stays open - a frozen process, a suspended machine, a route
// that drops packets without a reset - used to hold everyone else forever: measured, a
// client waited over a minute on such a server and never recovered.  The data sessions have
// carried a heartbeat for exactly this reason; the control channel now does too, with the
// same timings (HEARTBEAT_PROBE_MS / HEARTBEAT_TIMEOUT_MS).
static clock_t ctrlLastRx = 0;    // client side: last frame from the server
static clock_t ctrlLastProbe = 0; // client side: last probe sent to the server

// Milliseconds since a stamp.  A stamp of 0 means "it never happened", which has to read as
// a long time ago, not as "just now" - otherwise a probe that has never been sent looks
// fresh and is never sent at all.
static long MsSince(clock_t t)
{
	if (t == 0) return 0x7FFFFFFF;
	return (long)((clock() - t) * 1000 / CLOCKS_PER_SEC);
}

// A connection that has been accepted but has not said yet what it is for.  One port serves
// both roles, so the first frame decides: MESS_PEER_HELLO makes it a data session, a
// CLIENT_ message makes it a control client.  Anything else, or nothing at all within the
// timeout, and it is dropped.
struct PendingConn {
	socket_t    sock = SOCK_INVALID;
	TcpRecvBuf  rbuf;
	std::string addr;
	int         srcPort = 0;
	clock_t     since = 0;
};
static std::vector<PendingConn> pendingConns;
static std::mutex pendingConnsMtx;
static const long PENDING_TIMEOUT_MS = 5000;
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

		// The host used to hold two ports: a control port clients connect to, and a separate
		// data port for peer traffic.  It is one port now - the control port serves both, and
		// the host advertises it as its data port too.  One port per node is also what the
		// planned server hand-over needs: a client that takes over hosting has to be
		// reachable for control traffic on a port it already owns.
		//
		// Done before the log name is built, so the file is named after the port in use.
		if (clIam_server && clServerPort > 0)
			clPort = clServerPort;

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
			if (::bind(ctrlListenSock, (sockaddr*)&a, sizeof(a)) == 0) {
				listen(ctrlListenSock, 16);
				if (m_network_debug)
					debug_net_printf("NetworkClass: ctrl listen on port %d\n", clServerPort);

			}
			else {
				// Always report this: hosting is impossible and every later symptom
				// (clients looping on "server disconnected") follows from it.
				if (m_network_debug)
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
			if (m_network_debug)
				debug_net_printf("NetworkClass: data port %d is the local server's control port - "
					"taking an OS-assigned data port instead\n", clPort);
			clPort = 0; // ask the OS; StartPeerListen reports what we actually got
		}

		// Peer data port (may relocate if the configured one is taken).  On the host that is
		// the control socket it already has: opening a second one on the same port would
		// fail, and there is nothing for it to do - AcceptIncoming() takes both kinds of
		// connection off the one listener.
		if (clIam_server && ctrlListenSock != SOCK_INVALID) {
			peerListenSock = ctrlListenSock;
			if (m_network_debug)
				debug_net_printf("NetworkClass: one port %d for control and data\n", clPort);
		}
		else if (!StartPeerListen(clPort))
			if (m_network_debug)
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
		ctrlLastRx = clock();   // start the silence clock from the moment we are connected
		ctrlLastProbe = 0;
		IpPortIsSet = true;
		if (m_network_debug)
			debug_net_printf("ConnectToServer: connected to %s:%d\n", clHost.c_str(), clServerPort);

		// Introduce ourselves again after a hand-over.  Without this the new server keeps only
		// the seeded entry, which nothing refreshes.
		if (reRegisterWithNewServer && !myNetName.empty()) {
			reRegisterWithNewServer = false;
			shadow_myNCB n{}; n.ncb_command_0 = 0xFE;
			char padded[16] = { 0 };
			snprintf(padded, sizeof(padded), "%-15s", myNetName.c_str());
			SendCtrl(Pack_Message(MESS_CLIENT_TESTADDNAME, n, 0, clPort, padded, 16));
			if (m_network_debug)
				debug_net_printf("TAKEOVER: registering [%s] with the new server\n", myNetName.c_str());
		}

	}

	// ---------------------------------------------------------------------------
	// Accept on whichever listening sockets this node has and park the connection until it
	// says what it is for.  Both kinds arrive here now, so nothing may be decided from the
	// port alone.
	void NetworkClass::AcceptIncoming()
	{
		const socket_t listeners[2] = { ctrlListenSock, peerListenSock };
		for (int li = 0; li < 2; li++) {
			socket_t ls = listeners[li];
			if (ls == SOCK_INVALID) continue;
			if (li == 1 && peerListenSock == ctrlListenSock) continue; // one merged socket
			while (true) {
				sockaddr_in from{}; socklen_t fl = sizeof(from);
				socket_t s = accept(ls, (sockaddr*)&from, &fl);
				if (s == SOCK_INVALID) break;
				set_nonblocking(s); set_nodelay(s);
				char ip[INET_ADDRSTRLEN]; inet_ntop(AF_INET, &from.sin_addr, ip, sizeof(ip));
				PendingConn pc;
				pc.sock = s; pc.addr = ip; pc.srcPort = ntohs(from.sin_port); pc.since = clock();
				std::lock_guard<std::mutex> lk(pendingConnsMtx);
				pendingConns.push_back(std::move(pc));
				if (m_network_debug)
					debug_net_printf("Accept: connection from %s:%d, waiting for its first message\n",
						ip, (int)ntohs(from.sin_port));
			}
		}
	}

	// Turn a classified connection into a control client.
	void NetworkClass::AdoptCtrlClient(PendingConn& pc, const std::string& firstRaw)
	{
		ClientCtrl cc;
		cc.sock = pc.sock; cc.addr = pc.addr; cc.port = pc.srcPort;
		cc.rbuf = pc.rbuf;               // whatever else already arrived stays queued
		cc.lastRx = clock();
		message_info u = Unpack_Message(firstRaw);
		if (u.message == MESS_CLIENT_GET_IP || u.message == MESS_CLIENT_TESTADDNAME
			|| u.message == MESS_HEARTBEAT || u.message == MESS_HEARTBEAT_ACK)
			cc.dataPort = u.port;   // every one of these carries the sender's own port
		// A node joining after the session's first name was registered has to be told
		// about it now - it will not ask, because it considers itself set up as soon
		// as the control connection is up.
		if (serverAddname) {
			shadow_myNCB sn{}; sn.ncb_command_0 = 0xFE;
			TcpSendFull(pc.sock, Pack_Message(MESS_SERVER_GIVE_IP, sn, SERVER_NAME_REGISTERED, clPort));
		}
		{
			std::lock_guard<std::recursive_mutex> lk(ctrlClientsMtx);
			ctrlClients.push_back(std::move(cc));
		}
		if (m_network_debug)
			debug_net_printf("AcceptCtrl: client %s:%d\n", pc.addr.c_str(), pc.srcPort);
		// A node that has just arrived - or come back after a hand-over - needs the current
		// membership straight away, not at the next change.
		BroadcastRoster();
		// A keep-alive is answered, not handed on - HandleServerMsg knows nothing about it.
		if (u.message == MESS_HEARTBEAT) {
			shadow_myNCB n{}; n.ncb_command_0 = 0xFE;
			TcpSendFull(pc.sock, Pack_Message(MESS_HEARTBEAT_ACK, n, 0, clPort));
			return;
		}
		if (u.message == MESS_HEARTBEAT_ACK)
			return;
		HandleServerMsg(firstRaw, pc.addr, u.port ? u.port : pc.srcPort, (intptr_t)pc.sock);
	}

	// Turn a classified connection into a peer data session, attached to the LISTEN NCB that
	// is waiting for one.
	bool NetworkClass::AdoptDataSession(PendingConn& pc, const std::string& firstRaw)
	{
		message_info u = Unpack_Message(firstRaw);
		const int callerPort = u.port ? u.port : pc.srcPort;

		// Attach the connection to the LISTEN that was waiting for THIS caller.  A LISTEN
		// names the peer it expects in ncb_callName_10, and the hello says who is calling,
		// so the two can be matched by name instead of by position.
		const std::string callerName = TrimName(u.messNCB.ncb_name_26, (int)sizeof(u.messNCB.ncb_name_26));
		myNCB* listenNCB = nullptr;
		myNCB* anyFreeListen = nullptr;
		{
			std::lock_guard<std::mutex> lk(clientConnMutex);
			for (auto* c : clientConnection) {
				const bool nameMatch = !callerName.empty()
					&& callerName == TrimName(c->ncb_callName_10, (int)sizeof(c->ncb_callName_10));
				bool hasAlive = false;
				{
					std::lock_guard<std::mutex> slk(sessions_mutex);
					auto it = tcpSessions.find(c);
					hasAlive = (it != tcpSessions.end() && it->second->alive);
				}
				if (hasAlive) continue;
				// The fallback for a caller that does not name itself still demands an
				// established LISTEN, so an anonymous connection cannot land on one the
				// server has not approved yet.
				if (c->ncb_lsn_2 != 0 && !anyFreeListen) anyFreeListen = c;
				if (nameMatch) {
					// Deliberately NOT requiring ncb_lsn_2 here.  That field is set locally
					// when MESS_SERVER_LISTEN_ACCEPT finds its way back, and the caller's data
					// connection can beat that message home - measured 4 ms after the CALL was
					// accepted, during a hand-over.  The pairing has already been settled by
					// the server, which only accepts a CALL once it has found this very
					// LISTEN, so the connection belongs here whether or not our own
					// bookkeeping has caught up.  Turning it away instead stranded the
					// survivors: nothing ever re-opens a data connection that was refused.
					listenNCB = c;
					break;
				}
			}
		}
		// An older peer that says nothing about itself still has to be let in.
		if (!listenNCB) listenNCB = anyFreeListen;
		if (!listenNCB) {
			// No LISTEN is armed for this caller yet.  Say "not yet" and leave the socket
			// open: the connection goes back on the pending pile and is offered again.
			//
			// Closing it here used to be final, because nothing ever re-opens a refused data
			// connection.  That is survivable when the two sides are milliseconds apart, but
			// on a rejoin they are not: whoever gets through the menus first calls while the
			// other is still working its way there - measured at ~790 ms with no LISTEN armed
			// at all - and the second match then never formed a session.
			if (m_network_debug) {
				// Rate-limited: this is retried on every poll while the connection is parked.
				static clock_t lastWhy = 0;
				if (lastWhy == 0 || (long)((clock() - lastWhy) * 1000 / CLOCKS_PER_SEC) >= 1000) {
					lastWhy = clock();
					if (m_network_debug)
						debug_net_printf("AcceptPeer: nobody listening for caller [%s] yet, connection parked\n",
							callerName.c_str());
				}
			}
			return false;
		}

		auto sess = std::make_shared<TcpSession>();
		sess->sock = pc.sock;
		sess->peerAddr = pc.addr;
		sess->peerPort = callerPort;
		sess->alive = true;
		sess->ownerNCB = listenNCB;
		sess->rbuf = pc.rbuf;            // anything that arrived behind the hello
		sess->Touch();
		AddSession(listenNCB, sess);
		PublishConnectionNotice(true, sess->peerAddr, sess->peerPort,
			IsSelfPeer(sess->peerAddr, sess->peerPort, clPort));
		// Adopting the connection IS the establishment of the session, so mark the LISTEN
		// established here rather than waiting for MESS_SERVER_LISTEN_ACCEPT to come back
		// and let setListen() do it.  The CALL side already does exactly this when its own
		// connection goes through.  Leaving it to the control message meant that after a
		// hand-over the accepting node kept lsn=0, NetworkGetState_74FE1() went on calling
		// the peer "not connected", and every turn was skipped with
		// "SENDTO: slot N is SKIPPED" - so the caller sat in a RECEIVE that nobody answered.
		listenNCB->ncb_lsn_2 = 20;
		listenNCB->ncb_cmd_cplt_49 = NRC_GOODRET;
		if (m_network_debug)
			debug_net_printf("AcceptPeer: data connection from %s:%d, session listenNCB=%p name=[%.16s] call=[%.16s]\n",
				pc.addr.c_str(), callerPort, (void*)listenNCB, listenNCB->ncb_name_26, listenNCB->ncb_callName_10);
		return true;
	}

	// Read the first frame of every parked connection and hand it to whichever side owns it.
	void NetworkClass::PollPendingConnections()
	{
		std::vector<PendingConn> take;
		{
			std::lock_guard<std::mutex> lk(pendingConnsMtx);
			take.swap(pendingConns);
		}
		std::vector<PendingConn> keep;
		for (auto& pc : take) {
			bool ok = pc.rbuf.Poll(pc.sock);
			if (!pc.rbuf.ready.empty()) {
				const std::string first = pc.rbuf.ready.front();
				pc.rbuf.ready.erase(pc.rbuf.ready.begin());
				message_info u = Unpack_Message(first);
				if (u.message == MESS_PEER_HELLO) {
					if (!AdoptDataSession(pc, first)) {
						// Nobody is listening for this caller yet.  Put the hello back and
						// park the connection; PENDING_TIMEOUT_MS still bounds the wait, so a
						// connection nobody ever claims is dropped rather than kept for ever.
						if ((long)((clock() - pc.since) * 1000 / CLOCKS_PER_SEC) >= PENDING_TIMEOUT_MS) {
							if (m_network_debug)
								debug_net_printf("Accept: nobody claimed the data connection from %s in %ld ms, dropping it\n",
									pc.addr.c_str(), PENDING_TIMEOUT_MS);
							CLOSE_SOCKET(pc.sock);
							continue;
						}
						pc.rbuf.ready.insert(pc.rbuf.ready.begin(), first);
						keep.push_back(std::move(pc));
						continue;
					}
				}
				// A control connection can well open with a heartbeat: a client connects as
				// soon as it starts and then stays quiet until it wants something, so the
				// keep-alive is often the first thing it ever says.
				else if (u.message == MESS_CLIENT_GET_IP || u.message == MESS_CLIENT_TESTADDNAME
					|| u.message == MESS_CLIENT_MESSAGE_LISTEN || u.message == MESS_CLIENT_MESSAGE_CALL
					|| u.message == MESS_CLIENT_CANCEL || u.message == MESS_CLIENT_DELETE
					|| u.message == MESS_HEARTBEAT || u.message == MESS_HEARTBEAT_ACK) {
					AdoptCtrlClient(pc, first);
				}
				else {
					if (m_network_debug)
						debug_net_printf("Accept: connection from %s opened with %s, dropping it\n",
							pc.addr.c_str(), MessageIndexToText(u.message));
					CLOSE_SOCKET(pc.sock);
				}
				continue;
			}
			if (!ok) { CLOSE_SOCKET(pc.sock); continue; }
			if ((long)((clock() - pc.since) * 1000 / CLOCKS_PER_SEC) >= PENDING_TIMEOUT_MS) {
				if (m_network_debug)
					debug_net_printf("Accept: connection from %s said nothing in %ld ms, dropping it\n",
						pc.addr.c_str(), PENDING_TIMEOUT_MS);
				CLOSE_SOCKET(pc.sock);
				continue;
			}
			keep.push_back(std::move(pc));
		}
		if (!keep.empty()) {
			std::lock_guard<std::mutex> lk(pendingConnsMtx);
			for (auto& pc : keep) pendingConns.push_back(std::move(pc));
		}
	}

	// Send the membership to everyone.  Called by the server after anything that changes it:
	// a name registered, a name deleted, a client lost.  Cheap enough (eight entries) to
	// send whole rather than as differences, which also means a node that misses one update
	// is corrected by the next.
	void NetworkClass::BroadcastRoster()
	{
		if (!clIam_server) return;

		std::vector<RosterEntry> entries;
		for (int i = 0; i < (int)NetworkName.size() && i < 8; i++) {
			RosterEntry e{};
			std::string n = TrimName(NetworkName[i].c_str(), (int)NetworkName[i].size());
			strncpy(e.name, n.c_str(), sizeof(e.name) - 1);
			strncpy(e.ip, clientIpPort[i].adress.c_str(), sizeof(e.ip) - 1);
			e.port = clientIpPort[i].port;
			entries.push_back(e);
		}
		// Sorted by name, so "who comes next" is the same question for everyone.
		std::sort(entries.begin(), entries.end(),
			[](const RosterEntry& a, const RosterEntry& b) { return strncmp(a.name, b.name, sizeof(a.name)) < 0; });

		std::vector<RosterEntry> previous;
		{
			std::lock_guard<std::mutex> lk(rosterMtx);
			previous = knownRoster;
			knownRoster = entries;
		}
		ReleasePendingForVanished(previous, entries);

		shadow_myNCB n{}; n.ncb_command_0 = 0xFE;
		std::string msg = Pack_Message(MESS_SERVER_ROSTER, n, (int32_t)entries.size(), clPort,
			entries.empty() ? nullptr : (const char*)entries.data(),
			(int)(entries.size() * sizeof(RosterEntry)));
		{
			std::lock_guard<std::recursive_mutex> lk(ctrlClientsMtx);
			for (auto& cc : ctrlClients)
				if (cc.sock != SOCK_INVALID) TcpSendFull(cc.sock, msg);
		}
		if (m_network_debug) {
			std::string list;
			for (auto& e : entries) {
				char b[128];
				snprintf(b, sizeof(b), "%s@%s:%d ", e.name, e.ip, e.port);
				list += b;
			}
			if (m_network_debug)
				debug_net_printf("ROSTER: %d member(s) sent: %s\n", (int)entries.size(), list.c_str());
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
				cc.lastRx = clock();
				// Answered here rather than in HandleServerMsg: the reply has to go back on
				// this very socket, and at this point we hold it.
				if (u.message == MESS_HEARTBEAT) {
					shadow_myNCB n{}; n.ncb_command_0 = 0xFE;
					TcpSendFull(cc.sock, Pack_Message(MESS_HEARTBEAT_ACK, n, 0, clPort));
					continue;
				}
				// Intercept GET_IP to learn client's data port
				if (u.message == MESS_CLIENT_GET_IP) cc.dataPort = u.port;
				if (u.message == MESS_CLIENT_TESTADDNAME) cc.dataPort = u.port;
				HandleServerMsg(raw, cc.addr, cc.dataPort ? cc.dataPort : u.port, (intptr_t)cc.sock);
			}
			cc.rbuf.ready.clear();
			// Idle is normal - a client only speaks when it wants something - so ask before
			// concluding anything, and only give up when it has not answered either.
			if (ok && cc.lastRx != 0) {
				const long silence = MsSince(cc.lastRx);
				if (silence >= HEARTBEAT_TIMEOUT_MS) {
					if (m_network_debug)
						debug_net_printf("PollCtrlClients: client %s:%d silent for %ld ms - dropping it\n",
							cc.addr.c_str(), cc.port, silence);
					ok = false;
				}
				else if (silence >= HEARTBEAT_PROBE_MS && MsSince(cc.lastProbe) >= HEARTBEAT_PROBE_MS) {
					cc.lastProbe = clock();
					shadow_myNCB n{}; n.ncb_command_0 = 0xFE;
					TcpSendFull(cc.sock, Pack_Message(MESS_HEARTBEAT, n, 0, clPort));
				}
			}
			if (!ok) {
				if (m_network_debug)
					debug_net_printf("PollCtrlClients: client %s:%d disconnected\n", cc.addr.c_str(), cc.port);

				bool membershipChanged = false;
				for (int ni = (int)NetworkName.size() - 1; ni >= 0; ni--) {
					if (clientIpPort[ni].adress == cc.addr && clientIpPort[ni].port == cc.dataPort) {
						RemoveListenName(NetworkName[ni]);
						RemoveNetworkName(NetworkName[ni]);
						membershipChanged = true;
					}
				}
				CLOSE_SOCKET(cc.sock);
				it = ctrlClients.erase(it);
				if (membershipChanged) BroadcastRoster();
			}
			else { ++it; }
		}
	}

	// The server is gone.  Everybody left works out who takes over from the membership they
	// all hold, so nobody has to be asked and nobody can disagree: the list is in name order,
	// the server's own entry drops out, and the first name left is the new server.  Whoever
	// that is starts answering; the others point themselves at it.
	//
	// The data sessions between peers are not touched - they never went through the server in
	// the first place, and tearing them down would take the running game with them.
	void NetworkClass::HandleServerLoss()
	{
		std::vector<RosterEntry> roster;
		{
			std::lock_guard<std::mutex> lk(rosterMtx);
			roster = knownRoster;
		}
		if (roster.empty()) {
			if (m_network_debug)
				debug_net_printf("TAKEOVER: server lost, but no membership is known - staying put\n");
			return;
		}

		// The entry that answers on the address we were talking to is the one that died.
		std::string deadName;
		for (auto& e : roster)
			if (clHost == e.ip && clServerPort == e.port) { deadName = e.name; break; }

		const RosterEntry* successor = nullptr;
		for (auto& e : roster) {
			if (!deadName.empty() && deadName == e.name) continue;
			successor = &e;   // roster is sorted by name, so the first survivor wins
			break;
		}
		if (!successor) {
			if (m_network_debug)
				debug_net_printf("TAKEOVER: server [%s] lost and nobody else is left\n", deadName.c_str());
			return;
		}

		if (!myNetName.empty() && myNetName == successor->name) {
			// Our turn.  We already listen on our own port - the control and data ports are
			// one - so there is nothing to open, only a table to seed and a role to assume.
			if (m_network_debug)
				debug_net_printf("TAKEOVER: server [%s] is gone, [%s] takes over on %s:%d\n",
					deadName.c_str(), successor->name, successor->ip, successor->port);

			NetworkName.clear();
			clientIpPort.clear();
			for (auto& e : roster) {
				if (!deadName.empty() && deadName == e.name) continue;
				std::string padded = e.name;
				while (padded.size() < 15) padded += " ";
				AddNetworkName(padded, TypeIpPort{ std::string(e.ip), e.port });
			}

			clIam_server = true;
			// Move the game layer's notion of the server role as well.  It is set once from
			// the command line and never moved, so after a hand-over the new server still
			// behaved like a client - and the work that only the server does, starting the
			// level from the lobby among it, was left undone.  That is what stranded the
			// survivors in the menus when the host died before the level began.
			Iam_server = true;
			serverAddname = true;          // tell joiners the session's first name is taken
			clHost = "127.0.0.1";
			clServerPort = clPort;         // from now on we are the address others come to
			IpPortIsSet = true;
			BroadcastRoster();
		}
		else {
			if (m_network_debug)
				debug_net_printf("TAKEOVER: server [%s] is gone, following [%s] to %s:%d\n",
					deadName.c_str(), successor->name, successor->ip, successor->port);
			clHost = successor->ip;
			clServerPort = successor->port;
			reRegisterWithNewServer = true;   // say who we are once the connection is up
			// ConnectToServer() picks this up on the next tick; until it answers we keep
			// trying, which is what a node that is still starting up needs.
		}
	}

	// ---------------------------------------------------------------------------
	void NetworkClass::PollCtrlSocket()
	{
		if (ctrlSock == SOCK_INVALID) return;
		bool ok = ctrlRecvBuf.Poll(ctrlSock);
		// Work from a list of our own, not from the live buffer.  HandleClientMsg() below is a
		// long way from here and reaches a good deal of the transport; anything down there that
		// polls the control socket again would invalidate a reference held into ctrlRecvBuf,
		// and the reference is what this loop iterates over.  The swap costs nothing and makes
		// the guarantee local instead of a promise about code elsewhere.
		std::vector<std::string> frames;
		frames.swap(ctrlRecvBuf.ready);
		for (auto& raw : frames) {
			message_info u = Unpack_Message(raw);
			if (debug_net) LogPkt("CTRL RX ←", clHost, clServerPort, u);
			ctrlLastRx = clock();
			if (u.message == MESS_HEARTBEAT_ACK) continue;   // the probe came back, nothing more to do
			if (u.message == MESS_HEARTBEAT) {               // whoever asks gets an answer
				shadow_myNCB n{}; n.ncb_command_0 = 0xFE;
				SendCtrl(Pack_Message(MESS_HEARTBEAT_ACK, n, 0, clPort));
				continue;
			}
			HandleClientMsg(raw);
		}
		// No clear() here: the swap above already emptied ctrlRecvBuf.ready, and clearing it
		// now would throw away anything that arrived while the loop was running.

		// Probe the server when the link has been quiet, and give up on it after the same
		// silence the data sessions allow.  Without this a server that stops answering but
		// keeps its socket open is never noticed at all.
		if (ok && ctrlLastRx != 0) {
			const long silence = MsSince(ctrlLastRx);
			if (silence >= HEARTBEAT_TIMEOUT_MS) {
				if (m_network_debug)
					debug_net_printf("PollCtrlSocket: server silent for %ld ms - treating it as gone\n", silence);
				ok = false;
			}
			else if (silence >= HEARTBEAT_PROBE_MS && MsSince(ctrlLastProbe) >= HEARTBEAT_PROBE_MS) {
				ctrlLastProbe = clock();
				shadow_myNCB n{}; n.ncb_command_0 = 0xFE;
				SendCtrl(Pack_Message(MESS_HEARTBEAT, n, 0, clPort));
			}
		}

		if (!ok) {
			if (m_network_debug)
				debug_net_printf("PollCtrlSocket: server disconnected\n");

			{
				std::lock_guard<std::mutex> lk(ctrlSendMtx);
				CLOSE_SOCKET(ctrlSock); ctrlSock = SOCK_INVALID;
			}
			IpPortIsSet = false;
			ctrlLastRx = 0;
			// Losing the server is not something to sit out: with three or more players the
			// others cannot be introduced to each other again until somebody answers.
			if (!clIam_server)
				HandleServerLoss();
		}
	}

	// ---------------------------------------------------------------------------
	// Accept incoming peer data connections (both roles)
	// ---------------------------------------------------------------------------
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
			if (NetFaults::Enabled()) {
				auto now = std::chrono::steady_clock::now();
				if (!sess->rbuf.ready.empty()) {
					for (auto& raw : sess->rbuf.ready)
						sess->heldMessages.push_back({ raw, now + std::chrono::milliseconds(NetFaults::NextHoldMs()) });
					sess->rbuf.ready.clear();
				}
				// Release on every poll, not only when something new turns up.  Gating the
				// release on a fresh arrival deadlocked the very thing it was measuring: in
				// lockstep the next message is what the held one is holding up, so a message
				// delayed by 30 ms actually waited for the next heartbeat.  That is why the
				// latency scenarios used to crawl at one exchange per few seconds and trip
				// the 3 s STUCK threshold - they were timing the heartbeat, not the delay.
				while (!sess->heldMessages.empty() && sess->heldMessages.front().releaseAt <= now) {
					sess->rbuf.ready.push_back(sess->heldMessages.front().raw);
					sess->heldMessages.pop_front();
				}
			}

			// Same reason as in PollCtrlSocket(): take the frames out of the session buffer
			// before working on them, so no reference into rbuf.ready outlives it.
			std::vector<std::string> frames;
			frames.swap(sess->rbuf.ready);
			for (auto& raw : frames) {
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
							if (m_network_debug)
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
			// Not cleared: frames.swap() above emptied rbuf.ready, and a clear() here would
			// discard whatever arrived in the meantime.

			if (!ok) {
				if (m_network_debug)
					debug_net_printf("PollSessions: peer %s:%d disconnected\n", sess->peerAddr.c_str(), sess->peerPort);

				// Outside the debug test above: the notice is a feature of the game, not
				// diagnostics, so it has to appear whether or not logging is switched on.
				PublishConnectionNotice(false, sess->peerAddr, sess->peerPort,
					IsSelfPeer(sess->peerAddr, sess->peerPort, clPort));
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

				// A peer that went silent is gone as surely as one whose socket closed - this
				// is the path a killed or frozen node leaves by, so it announces itself too.
				PublishConnectionNotice(false, sess->peerAddr, sess->peerPort,
					IsSelfPeer(sess->peerAddr, sess->peerPort, clPort));
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
		PublishConnectionNotice(true, sess->peerAddr, sess->peerPort,
			IsSelfPeer(sess->peerAddr, sess->peerPort, clPort));
		// Announce what this connection is for, and say who is calling: the listener has one
		// LISTEN armed per player and has to attach this connection to the right one.  With
		// two players either choice happens to be correct; with three, guessing hands the
		// third player's connection to a slot that was waiting for somebody else.
		sess->Send(Pack_Message(MESS_PEER_HELLO, myNCBtoShadow(*ncb), 0, clPort));
		return true;
	}

	// ---------------------------------------------------------------------------
	// Server message handler
	// ---------------------------------------------------------------------------
	void NetworkClass::HandleServerMsg(const std::string& raw,
		const std::string& senderAddr,
		int                senderDataPort,
		intptr_t           senderSockRaw)
	{
		const socket_t senderSock = (senderSockRaw == -1) ? SOCK_INVALID : (socket_t)senderSockRaw;
		message_info u = Unpack_Message(raw);

		// Answer whoever asked, on the connection they asked over.  Routing a reply by
		// ip+port used to send it to the wrong node whenever the sender's data port was not
		// registered yet: SendToCtrlClient then falls back to matching on address alone, and
		// on loopback every node is 127.0.0.1, so the reply went to the first match - which
		// on a node that has just taken the server role is its own client connection.  The
		// caller waited for a CALL_ACCEPT that the new server had handed to itself.
		auto ReplyToSender = [&](const std::string& msg) -> bool {
			if (senderSock != SOCK_INVALID) {
				if (debug_net) { message_info m = Unpack_Message(msg); LogPkt("CTRL TX →", senderAddr, senderDataPort, m); }
				return TcpSendFull(senderSock, msg);
			}
			return SendToCtrlClient(msg, senderAddr, senderDataPort);
		};

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
			// The session's first name belongs to the node hosting it.
			//
			// Every node drops its name when a match ends and asks for one again for the next, so
			// between matches the whole name space is briefly free.  A node arriving in that gap
			// used to be handed the first name - measured: the newcomer ended up as NETH200 and the
			// machine actually hosting the session was pushed to NETH201, after which nothing was
			// exchanged at all.  The protocol already says joiners should wait for the server to
			// register (MESS_SERVER_GIVE_IP / SERVER_NAME_REGISTERED); this makes the server hold
			// them off - but ONLY for that one name.  Refusing every name until the host has
			// registered was tried and breaks restarting: after a match the client asks first, is
			// turned away from all eight indices and gives up, and the menu reports it could not
			// join.  A joiner arriving first simply takes the second name and waits in the lobby.
			{
				char firstName[16] = { 0 };
				snprintf(firstName, sizeof(firstName), "NETH2%c0", u.data[5]);
				while (strlen(firstName) < 15) strcat(firstName, " ");
				const bool wantsFirstName = (memcmp(u.data, firstName, 15) == 0);
				// ...and the first name belongs to the node hosting the session, nobody else.
				//
				// Its own registration comes over its own control connection, so it is the request
				// whose data port is the port this server listens on.  Letting anybody else have
				// that name makes the game treat the joiner as node 0 while the transport server is
				// somebody else - the two disagree about who is who, and the session comes apart as
				// soon as they have to work together.
				const bool senderIsHost = (u.port == clPort);
				if (wantsFirstName && !senderIsHost) {
					if (m_network_debug)
						debug_net_printf("NAME: [%.15s] belongs to the host, refusing it to %s:%d\n",
							u.data, senderAddr.c_str(), u.port);
					ReplyToSender(Pack_Message(MESS_SERVER_TESTADDNAME_REJECT, n, u.index, -10));
					return;
				}

			}
			if (GetNameNetwork(u.data).empty()) {
				AddNetworkName(u.data, ip);
				ReplyToSender(Pack_Message(MESS_SERVER_TESTADDNAME_OK, n, u.index, -10));
				BroadcastRoster();   // membership changed
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
				ReplyToSender(Pack_Message(MESS_SERVER_TESTADDNAME_OK, n, u.index, -10));
			}
			else {
				ReplyToSender(Pack_Message(MESS_SERVER_TESTADDNAME_REJECT, n, u.index, -10));
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
				ReplyToSender(Pack_Message(MESS_SERVER_CALL_ACCEPT, u.messNCB, u.index, -10,
					(char*)&ci, sizeof(ci)));

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
				ReplyToSender(Pack_Message(MESS_SERVER_CALL_REJECT, u.messNCB, u.index, -10));
			}
		}
		else if (u.message == MESS_CLIENT_CANCEL) {
			if (u.data[0] != 0) RemoveListenName(u.data);
		}
		else if (u.message == MESS_CLIENT_DELETE) {
			CleanMessages(myNCBfromShadow(u.messNCB));
			{
				// The host handing its own name back ends the "server is registered" state, so the
				// rule above applies again to the next match rather than only to the first.
				char firstName[16] = { 0 };
				snprintf(firstName, sizeof(firstName), "NETH2%c0", u.data[5]);
				while (strlen(firstName) < 15) strcat(firstName, " ");
				if (memcmp(u.data, firstName, 15) == 0) {
					serverAddname = false;
					receiveServerAddName = false;
				}
			}
			RemoveNetworkName(u.data);
			BroadcastRoster();   // membership changed
		}
	}

	// ---------------------------------------------------------------------------
	// Client message handler (replies from the server)
	// ---------------------------------------------------------------------------
	void NetworkClass::HandleClientMsg(const std::string& raw)
	{
		message_info u = Unpack_Message(raw);

		if (u.message == MESS_SERVER_ROSTER) {
			std::vector<RosterEntry> entries;
			const int count = (int)(u.size / sizeof(RosterEntry));
			for (int i = 0; i < count && i < 8; i++) {
				RosterEntry e{};
				memcpy(&e, u.data + i * sizeof(RosterEntry), sizeof(RosterEntry));
				e.name[sizeof(e.name) - 1] = 0;
				e.ip[sizeof(e.ip) - 1] = 0;
				entries.push_back(e);
			}
			std::vector<RosterEntry> previous;
			{
				std::lock_guard<std::mutex> lk(rosterMtx);
				previous = knownRoster;
				knownRoster = entries;
			}
			ReleasePendingForVanished(previous, entries);
			if (m_network_debug) {
				std::string list;
				for (auto& e : entries) {
					char b[128];
					snprintf(b, sizeof(b), "%s@%s:%d ", e.name, e.ip, e.port);
					list += b;
				}
				if (m_network_debug)
					debug_net_printf("ROSTER: %d member(s) received: %s\n", (int)entries.size(), list.c_str());
			}
			return;
		}

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
			if (!pendingNetName.empty()) {
				myNetName = pendingNetName;
				pendingNetName.clear();
				if (m_network_debug)
					debug_net_printf("NAMES: this node is [%s]\n", myNetName.c_str());
			}
			std::lock_guard<std::mutex> lk(connections_mutex);
			connectionTime* ct = GetConnection(u.index);
			if (ct) ct->state = NETI_ADD_NAME_OK;
		}
		else if (u.message == MESS_SERVER_TESTADDNAME_REJECT) {
			pendingNetName.clear();      // that one was not ours
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
	// One thread at a time, and never nested.
	//
	// Despite the name this is not confined to a single thread: the network thread runs it in
	// a loop, and callers on the game thread have walked into it too.  Nothing inside is
	// written for concurrent use - the receive buffers (ctrlRecvBuf, every session's rbuf)
	// carry no lock at all, and the poll loops hold references INTO those buffers while they
	// process a frame.  A second thread reaching Poll() at that moment appends to `ready`,
	// which reallocates the vector, or clears it; either way the reference the first thread is
	// iterating over now points at freed memory.  That is the crash: a heap-use-after-free in
	// Unpack_Message() called from PollCtrlSocket(), which ASan turns into an immediate
	// internal__exit(1) - the instance vanishes with no CRT shutdown and nothing in the log.
	// It fired while the game thread was tearing a session down between matches, which is why
	// only the two-matches scenario saw it, and only sometimes.
	//
	// A tick that finds another one already in progress returns rather than waiting for it.
	// There is nothing for it to do that the running tick is not already doing, and making a
	// caller wait here would put the network thread's latency on the game thread.
	static std::atomic<bool> tickBusy{ false };
	struct TickGuard {
		std::atomic<bool>& flag; bool held;
		explicit TickGuard(std::atomic<bool>& f) : flag(f), held(false) {
			bool expected = false;
			held = f.compare_exchange_strong(expected, true);
		}
		~TickGuard() { if (held) flag.store(false); }
	};

	void NetworkClass::locUpdateNetworkSingleThread()
	{
		TickGuard tick(tickBusy);
		if (!tick.held) return;

		if (clIam_client && ctrlSock == SOCK_INVALID) ConnectToServer();
		AcceptIncoming();
		PollPendingConnections();
		// Not only when hosting: every node has a listening port now, so any node can end up
		// holding control connections - which is what a hand-over of the server role needs.
		PollCtrlClients();
		if (clIam_client) PollCtrlSocket();
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
		// A RECEIVE NCB is matched to its session by NCB pointer: the game keeps one
		// NCB per player slot (connection_E12AE[]) and uses that same object for the
		// CALL/LISTEN that created the session and for every later SEND/RECEIVE on
		// that peer, which is exactly the key tcpSessions is stored under.
		// ------------------------------------------------------------------
		for (connectionTime& conn : handleConnections)
		{
			if (conn.connection->ncb_command_0 != 0x95) continue;
			if (conn.connection->ncb_cmd_cplt_49 != NRC_PENDING) continue;
			if (!conn.connection->ncb_buffer_4.p) continue;

			// The session is keyed by the NCB the game used for CALL/LISTEN, and the game
			// reuses that very NCB — connection_E12AE[slot] — for the RECEIVE on the same
			// peer (NetworkReceivePacket_74C9D just overwrites ncb_command_0).  So the
			// session that belongs to this RECEIVE is found by pointer, exactly.
			//
			// Matching on ncb_name_26 (our OWN name) instead was ambiguous the moment a
			// third player joined: every session on a node carries the same local name, so
			// the scan attached the RECEIVE to whichever peer came first in the map.  With
			// two nodes there is only one session and the wrong choice cannot happen; with
			// three, a RECEIVE waiting for peer A kept looking into peer B's empty queue
			// and never saw its data, while that data sat queued for good.
			std::shared_ptr<TcpSession> sess = GetSession(conn.connection);
			if (!sess || !sess->alive) {
				// A session that exists but is dead means the peer is gone (disconnected
				// or banished): fail the blocking RECEIVE instead of spinning forever in
				// NetworkReceivePacket_74C9D().  NO session at all is the normal
				// re-negotiation gap (HANG_UP removes it) — keep waiting, so reconnect
				// is not disturbed.
				if (sess) {
					conn.connection->ncb_retcode_1 = NRC_SCLOSED;
					conn.connection->ncb_cmd_cplt_49 = NRC_SCLOSED;
					toDelete.push_back(conn.index);
				}
				if (m_network_debug)
					debug_net_printf("Pass1: RECEIVE idx=%d no live session name=[%.16s] call=[%.16s] deadPeer=%d\n",
						conn.index, conn.connection->ncb_name_26, conn.connection->ncb_callName_10, (int)(sess != nullptr));

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
				// Too big for the buffer the game offered.  NetBIOS answers this with
				// NRC_BUFLEN and as much of the message as fits - it does NOT stay pending.
				//
				// This used to `continue` after the message had already been popped off the
				// queue above, so the message was destroyed AND the RECEIVE stayed pending
				// for ever.  Nothing re-sends in lockstep, so both nodes then sat waiting on
				// each other - measured at 54 s and counting.  Failing the command lets the
				// game see the mismatch and act on it; silently swallowing it cannot be
				// recovered from by anybody.
				const uint16_t fits = conn.connection->ncb_bufferLength_8;
				memcpy(conn.connection->ncb_buffer_4.p, u.data, fits);
				conn.connection->ncb_retcode_1 = NRC_BUFLEN;
				conn.connection->ncb_cmd_cplt_49 = NRC_BUFLEN;
				toDelete.push_back(conn.index);
				if (m_network_debug)
					debug_net_printf("Pass1: RECEIVE idx=%d size=%u > buf=%u BUFLEN name=[%.16s] call=[%.16s]\n",
						conn.index, u.size, fits, conn.connection->ncb_name_26, conn.connection->ncb_callName_10);

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
				else if (conn.retryCount >= MAX_RETRIES) {
					conn.connection->ncb_cmd_cplt_49 = NRC_SCLOSED;
				}
				else {
					// A rejected CALL is retried rather than failed outright.  When the
					// server dies, each survivor works out the new token holder on its own
					// clock and calls it; whoever gets there before that node has armed its
					// LISTENs is turned away by a few milliseconds.  Treating that as final
					// left the survivors with no link to each other at all - measured at 7 ms
					// between the CALL being rejected and the LISTEN being posted.
					//
					// MAX_RETRIES at a 1 s timeout still gives up on a name that really is
					// not there, so a genuinely wrong CALL is not retried for ever.
					conn.state = NETI_CALL;
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
				// ...but the record is reclaimed once the NCB has been completed by
				// somebody else: PollSessions writes NRC_SCLOSED straight into the owner
				// NCB when its peer drops.  Pass 1 only removes entries it completed
				// itself, so without this every disconnect left a dead RECEIVE record in
				// handleConnections for the rest of the match, re-scanned on every tick.
				del = (conn.connection->ncb_cmd_cplt_49 != NRC_PENDING);
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
		// Asked for, not owned yet - myNetName is written when the server says yes.
		pendingNetName = TrimName(c->ncb_name_26, (int)sizeof(c->ncb_name_26));
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
		{
			// Handing the name back makes this node nameless until it registers again; a
			// stale one here is what made two nodes believe they were the same player.
			const std::string gone = TrimName(c->ncb_name_26, (int)sizeof(c->ncb_name_26));
			if (gone == myNetName) myNetName.clear();
			if (gone == pendingNetName) pendingNetName.clear();
		}
		SendCtrl(Pack_Message(MESS_CLIENT_DELETE, myNCBtoShadow(*c), index, clPort,
			c->ncb_name_26, sizeof(c->ncb_name_26)));
		// No network tick from here.  This is the game thread - simulateInterupt(DELETE_NAME),
		// reached from NetworkCanceling_73669() when a match is torn down - and the tick is
		// the network thread's, running in a loop of its own.  Two threads inside it at once
		// corrupt the unlocked receive buffers; see locUpdateNetworkSingleThread().
		//
		// Nothing is lost by not ticking: DELETE is fire-and-forget, the message above has
		// already gone out over TCP, and the network thread comes round within a millisecond.
		// The call was inherited from the single-threaded version, where it replaced a
		// singleThreadSleep(400) that gave the send time to leave.
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

		// Look the session up by NCB pointer — the game reuses the per-peer NCB
		// (connection_E12AE[slot]) that opened the session for every SEND on it, so this
		// names the destination exactly.  Scanning for a session by ncb_name_26 (our OWN
		// name) matched every session this node has, so with three players a turn meant
		// for one peer was handed to whichever session came first in the map.
		std::shared_ptr<TcpSession> sess = GetSession(c);

		if (!sess || !sess->alive) {
			if (m_network_debug)
				debug_net_printf("SendNetwork: no live session name=[%.16s] call=[%.16s] deadPeer=%d\n",
					c->ncb_name_26, c->ncb_callName_10, (int)(sess != nullptr));

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
			debug_net_printf("SendNetwork: sent size=%u name=[%.16s] call=[%.16s] → %s:%d\n",
				c->ncb_bufferLength_8, c->ncb_name_26, c->ncb_callName_10,
				sess->peerAddr.c_str(), sess->peerPort);

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
// See port_net.h for why the game layer needs this.
int NetworkRosterPlayers(const char* namePrefix, bool present[8])
{
	for (int i = 0; i < 8; i++) present[i] = false;
	if (!namePrefix) return -1;
	const size_t prefixLen = strlen(namePrefix);

	std::vector<RosterEntry> roster;
	{
		std::lock_guard<std::mutex> lk(rosterMtx);
		roster = knownRoster;
	}
	if (roster.empty()) return -1;

	int marked = 0;
	for (auto& e : roster) {
		// Names are padded with spaces, so the printable part ends at the first blank.
		size_t end = 0;
		while (end < sizeof(e.name) && e.name[end] != '\0' && e.name[end] != ' ') end++;
		if (end <= prefixLen) continue;                              // too short to carry one
		if (strncmp(e.name, namePrefix, prefixLen) != 0) continue;   // not one of ours
		size_t p = prefixLen;
		int idx = 0;
		while (p < end && e.name[p] >= '0' && e.name[p] <= '9') { idx = idx * 10 + (e.name[p] - '0'); p++; }
		if (p != end) continue;                                      // trailing junk
		if (idx >= 0 && idx < 8 && !present[idx]) { present[idx] = true; marked++; }
	}
	return marked;
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