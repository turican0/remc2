#ifndef PORT_NETWORK
#define PORT_NETWORK

#define _CRT_SECURE_NO_WARNINGS

#include <cstdlib>
#include <cstdint>
#include <iostream>
#include <fstream>

#include "engine/defs.h"
#include "portability/port_outputs.h"

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
#pragma pack(pop)

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
		void AcceptCtrlClients();
		void PollCtrlClients();
		void PollCtrlSocket();
		void AcceptPeerConnections();
		void PollSessions();
		bool ConnectDataToPeer(myNCB* ncb, const std::string& addr, int dataPort);
		void HandleServerMsg(const std::string& raw, const std::string& senderAddr, int senderDataPort);
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

void debug_net_printf(const char* format, ...);

void timeState(bool start, const char* text);

//extern MyNetworkLib::NetworkClass* locNetworkClass;

#endif //PORT_NETWORK
