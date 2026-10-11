#ifndef PORT_NETWORK
#define PORT_NETWORK

#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <fstream>

#include "../engine/defs.h"
#include "../engine/EventDispatcher.h"
#include "../portability/port_outputs.h"

#include <thread>
#include <atomic>
#include <cassert>
#include <vector>
#include <mutex>
#include <deque>
#include <map>

extern std::string debug_net_filename1;

typedef struct {//lenght 10
	char byte_0[50];
}
type_v2x;

typedef union {
	uint8_t* p;
	uint32_t align;
} p64align;

#pragma pack(push, 1)
typedef struct {//lenght 66(changed to 70)
	uint8_t ncb_command_0;
	uint8_t ncb_retcode_1;
	uint8_t ncb_lsn_2;
	uint8_t ncb_num_3;
	p64align ncb_buffer_4;
	uint16_t ncb_bufferLength_8;
	char ncb_callName_10[16];
	char ncb_name_26[16];
	uint8_t ncb_rto_42;
	uint8_t ncb_sto_43;
	//p64align ncb_post_44;// not used
	//uint8_t ncb_lana_num_48;// not used
	volatile uint8_t ncb_cmd_cplt_49;//lock for end command
	//uint8_t ncb_reserved_50[16];// not used
}
myNCB;

typedef struct {//lenght 66(changed to 70)
	uint8_t ncb_command_0;
	uint8_t ncb_retcode_1;
	uint8_t ncb_lsn_2;
	uint8_t ncb_num_3;
	uint32_t ncb_buffer_4;
	uint16_t ncb_bufferLength_8;
	char ncb_callName_10[16];
	char ncb_name_26[16];
	uint8_t ncb_rto_42;
	uint8_t ncb_sto_43;
	//p64align ncb_post_44;// not used
	//uint8_t ncb_lana_num_48;// not used
	uint8_t ncb_cmd_cplt_49;//lock for end command
	//uint8_t ncb_reserved_50[16];// not used
}
shadow_myNCB;
#pragma pack(pop)

// ---------------------------------------------------------------------------
// TypeIpPort - used both for network names and as sender identifier
// (replaces asio::ip::udp::endpoint throughout)
// ---------------------------------------------------------------------------
typedef struct {
	std::string adress;
	int port;
} TypeIpPort;

// ---------------------------------------------------------------------------
// Wire format
// ---------------------------------------------------------------------------
#pragma pack(push, 1)
typedef struct message_info {
	uint32_t     size;
	int32_t      message;
	int32_t      index;
	int32_t      port;
	uint32_t     seq_num;
	uint32_t     ack_num;
	shadow_myNCB messNCB;
	char         data[2048 * 30];
};
#pragma pack()

// Defined in port_net.cpp: a connection that has been accepted but has not said yet whether
// it carries control traffic or game data.
struct PendingConn;

namespace MyNetworkLib {

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
		// One port carries both roles, so an accepted connection is parked until its first
		// message says whether it is a control client or a peer data session.
		void AcceptIncoming();
		void PollPendingConnections();
		void AdoptCtrlClient(::PendingConn& pc, const std::string& firstRaw);
		// False means "not yet" - no LISTEN is armed for this caller.  The socket is left
		// open so the caller can park the connection and offer it again.
		bool AdoptDataSession(::PendingConn& pc, const std::string& firstRaw);
		void PollCtrlClients();
		// Server: hand the current membership to every client.
		void BroadcastRoster();
		// The server went away: decide who takes the role over, and either take it or follow.
		void HandleServerLoss();
		void PollCtrlSocket();
		void PollSessions();
		bool ConnectDataToPeer(myNCB* ncb, const std::string& addr, int dataPort);
		// senderSock is the connection the request arrived on.  Replies go straight back
		// down it: addressing them by ip+port cannot distinguish two nodes that share an
		// address, which on loopback is every node in the session.
		//
		// Carried as intptr_t so this header does not have to pull in the socket headers;
		// -1 is the invalid handle on both platforms (INVALID_SOCKET is (SOCKET)~0).
		void HandleServerMsg(const std::string& raw, const std::string& senderAddr, int senderDataPort,
			intptr_t senderSock = -1);
		void HandleClientMsg(const std::string& raw);
		void UpdateClient();
	};

}

extern MyNetworkLib::NetworkClass* locNetworkClass;

void simulateInterupt(myNCB* connection);

// Clears per-match network state (data sessions, established-connection list, pending
// NCB commands) so a fresh multiplayer match does not inherit stale entries from a
// previous one.  Sockets stay open.  Fixes restart bug (#3).
void ResetNetworkGameState();

void EndMyNetLib();

void SetNetworkDebug();

void AddRecMess(std::string message);
std::string GetRecMess();
unsigned int GetRecCount();
//void CleanMessages(myNCB locNCB);

void printState(myNCB** connections);
void printState2(char* text);

bool ReceiveServerAddName();

void InitMyNetLib(bool iam_server, bool iam_client, char* ip, int networkPort, int serverPort);

// Which player slots the transport still sees in the session.
//
// Worked out from the membership list the server broadcasts on every change, so it covers
// peers this node holds no data session with.  That matters when the server dies: the game
// only ever talked to the token holder, so it cannot see the other survivors by itself, and
// the orderly hand-over it would otherwise use begins by receiving the survivor list FROM
// the node that went away.  The slot number is the trailing decimal of the NetBIOS name,
// which is how the names were built: namePrefix (the game's nethID) followed by the index.
// The prefix has to be passed in because it ends in digits itself - "NETH21" plus slot 1 is
// "NETH211", so reading the trailing number off the end yields 211, not 1.
//
// Fills present[0..7] and returns how many slots were marked, or -1 if the transport has no
// membership list yet - in which case the caller must fall back to what it knows itself.
int NetworkRosterPlayers(const char* namePrefix, bool present[8]);

void debug_net_printf(const char* format, ...);

void timeState(bool start, const char* text);

//extern MyNetworkLib::NetworkClass* locNetworkClass;

#endif //PORT_NETWORK
