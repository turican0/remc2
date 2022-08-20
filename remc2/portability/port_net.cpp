#define _CRT_SECURE_NO_WARNINGS
#include "port_net.h"
#include <thread>

#define USE_BOOST_ASIO_

#ifdef USE_BOOST_ASIO_
#include <boost/asio.hpp>
using namespace boost;
#else
#include <asio.hpp>
#endif

uint8_t* Zero_pointer_net = nullptr;

int timest_index = 0;
clock_t timest_timer = 0;
const int timest_max_mess = 400;
std::string timest_buffer[timest_max_mess];

//#define TEST_NETWORK_MESSAGES

//#define TEST_BAD_NETWORK

//#define TEST_TIME_NETWORK_MESSAGES

using namespace std;

typedef struct {
	myNCB* connection;
	long startTime;
	int timeout;
	int32_t index;
} connectionTime;
std::vector<connectionTime> handleConnections;

enum Neti_type {
	NETI_NOT_SET,
	NETI_ADD_NAME,
	NETI_ADD_NAME_REJECT,
	NETI_ADD_NAME_OK,
	NETI_CALL,
	NETI_CALL_ACCEPT,
	NETI_CALL_REJECT,
	NETI_LISTEN,
	//#define NETI_LISTEN_ACCEPT,
	NETI_LISTEN_REJECT,
	NETI_RECEIVE
	};

Neti_type netstate_shared = NETI_NOT_SET;

const int32_t MESS_UNKNOWN = -1;
const int32_t MESS_CLIENT_SERVER_NAME_ADDED = 0;
const int32_t MESS_SERVER_SERVER_NAME_ADDED = 1;
//const int32_t MESS_CLIENT_REGISTER_TIMEOUT = 2;
//const int32_t MESS_SERVER_REGISTER_TIMEOUT = 3;
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
const int32_t MESS_CLIENT_SEND = 15;
const int32_t MESS_SERVER_SEND_OK = 16;
const int32_t MESS_SERVER_SEND = 17;
const int32_t MESS_CLIENT_GET_IP = 18;
const int32_t MESS_SERVER_GIVE_IP = 19;

char* MessageIndexToText(int32_t index)
{
	switch (index) {
	case MESS_CLIENT_SERVER_NAME_ADDED:
		return (char*)"MESS_CLIENT_SERVER_NAME_ADDED";
	case MESS_SERVER_SERVER_NAME_ADDED:
		return (char*)"MESS_SERVER_SERVER_NAME_ADDED";
//	case MESS_CLIENT_REGISTER_TIMEOUT:
//		return (char*)"MESS_CLIENT_REGISTER_TIMEOUT";
//	case MESS_SERVER_REGISTER_TIMEOUT:
//		return (char*)"MESS_SERVER_REGISTER_TIMEOUT";
	case MESS_CLIENT_TESTADDNAME:
		return (char*)"MESS_CLIENT_TESTADDNAME";
	case MESS_SERVER_TESTADDNAME_OK:
		return (char*)"MESS_SERVER_TESTADDNAME_OK";
	case MESS_SERVER_TESTADDNAME_REJECT:
		return (char*)"MESS_SERVER_TESTADDNAME_REJECT";
	case MESS_CLIENT_MESSAGE_LISTEN:
		return (char*)"MESS_CLIENT_MESSAGE_LISTEN";
	case MESS_CLIENT_MESSAGE_CALL:
		return (char*)"MESS_CLIENT_MESSAGE_CALL";
	case MESS_SERVER_CALL_ACCEPT:
		return (char*)"MESS_SERVER_CALL_ACCEPT";
	case MESS_SERVER_LISTEN_ACCEPT:
		return (char*)"MESS_SERVER_LISTEN_ACCEPT";
	case MESS_SERVER_CALL_REJECT:
		return (char*)"MESS_SERVER_CALL_REJECT";
	case MESS_SERVER_LISTEN_REJECT:
		return (char*)"MESS_SERVER_LISTEN_REJECT";
	case MESS_CLIENT_CANCEL:
		return (char*)"MESS_CLIENT_CANCEL";
	case MESS_CLIENT_DELETE:
		return (char*)"MESS_CLIENT_DELETE";
	case MESS_CLIENT_SEND:
		return (char*)"MESS_CLIENT_SEND";
	case MESS_SERVER_SEND_OK:
		return (char*)"MESS_SERVER_SEND_OK";
	case MESS_SERVER_SEND:
		return (char*)"MESS_SERVER_SEND";
	case MESS_CLIENT_GET_IP:
		return (char*)"MESS_CLIENT_GET_IP";
	case MESS_SERVER_GIVE_IP:
		return (char*)"MESS_SERVER_GIVE_IP";
	}
	return (char*)"";
}

#pragma pack (1)
typedef struct message_info{
	uint32_t size;
	int32_t message;
	int32_t index;
	int32_t port;
	shadow_myNCB messNCB;
	char data[2048*30];
};
#pragma pack (16)

shadow_myNCB myNCBtoShadow(myNCB from) {
	shadow_myNCB to;
	to.ncb_command_0= from.ncb_command_0;
	to.ncb_retcode_1 = from.ncb_retcode_1;
	to.ncb_lsn_2 = from.ncb_lsn_2;
	to.ncb_num_3 = from.ncb_num_3;
	to.ncb_buffer_4 = (from.ncb_buffer_4.p - Zero_pointer_net);
	to.ncb_bufferLength_8 = from.ncb_bufferLength_8;
	for (int i = 0; i < 16; i++)to.ncb_callName_10[i] = from.ncb_callName_10[i];
	for (int i = 0; i < 16; i++)to.ncb_name_26[i] = from.ncb_name_26[i];
	to.ncb_rto_42 = from.ncb_rto_42;
	to.ncb_sto_43 = from.ncb_sto_43;
	to.ncb_cmd_cplt_49 = from.ncb_cmd_cplt_49;
	return to;
};
myNCB myNCBfromShadow(shadow_myNCB from) {
	myNCB to;
	to.ncb_command_0 = from.ncb_command_0;
	to.ncb_retcode_1 = from.ncb_retcode_1;
	to.ncb_lsn_2 = from.ncb_lsn_2;
	to.ncb_num_3 = from.ncb_num_3;
	to.ncb_buffer_4.p = (from.ncb_buffer_4 + Zero_pointer_net);
	to.ncb_bufferLength_8 = from.ncb_bufferLength_8;
	for (int i = 0; i < 16; i++)to.ncb_callName_10[i] = from.ncb_callName_10[i];
	for (int i = 0; i < 16; i++)to.ncb_name_26[i] = from.ncb_name_26[i];
	to.ncb_rto_42 = from.ncb_rto_42;
	to.ncb_sto_43 = from.ncb_sto_43;
	to.ncb_cmd_cplt_49 = from.ncb_cmd_cplt_49;
	return to;
};

std::string DataToString(message_info messInfo)
{
	std::string output;
	for (int i = 0; i < messInfo.size + sizeof(shadow_myNCB) + 24; i++)
	{
		output += ((char*)&messInfo)[i];
	}
	return output;
}

std::string Pack_Message(uint32 message, shadow_myNCB locNCB, int32_t locIndex, int32_t port=-1, const char* data = NULL, int size_of_data = 0) {
	message_info locmessage_info;
	locmessage_info.message = message;
	locmessage_info.size = size_of_data;
	locmessage_info.messNCB = locNCB;
	locmessage_info.index = locIndex;
	locmessage_info.port = port;
	if(data)
		memcpy(locmessage_info.data, data, size_of_data);
	return DataToString(locmessage_info);
}

message_info Unpack_Message(std::string data) {
	message_info output;
	for (int i = 0; i < sizeof(shadow_myNCB) + 24; i++)
		((char*)(&output))[i] = data[i];
	for (int i = sizeof(shadow_myNCB) + 24; i < output.size + sizeof(shadow_myNCB) + 24; i++)
		((char*)(&output))[i] = data[i];
	return output;
}

void singleThreadSleep(int sleepMs)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(sleepMs));
}

std::string IHaveNameStrP = "";

FILE* debug_net_output;
const char* debug_net_filename1 = "net_messages_log.txt";
std::string debug_net_filename2 = {};

bool debug_net_first = true;

std::mutex print_mt;

void debug_net_printf(const char* format, ...) {
	print_mt.lock();
	char prbuffer[1024*40];
	va_list arg;
	//int done;
	va_start(arg, format);
	vsprintf(prbuffer, format, arg);
	va_end(arg);

	//std::string exepath = get_exe_path();
	//path = exepath + "/" + debug_net_filename;
	//net_path = debug_net_filename;

	if (debug_net_first)
	{
		debug_net_output = fopen(debug_net_filename2.c_str(), "wt");
		debug_net_first = false;
	}
	else
		debug_net_output = fopen(debug_net_filename2.c_str(), "at");
	fprintf(debug_net_output, "%d|%s", clock(),prbuffer);
	fclose(debug_net_output);
#ifdef DEBUG_PRINT_DEBUG_TO_SCREEN
	printf(prbuffer);
#endif
	print_mt.unlock();
}


std::vector<std::string> NetworkName;
typedef struct {
	std::string adress;
	int port;
} TypeIpPort;
std::vector<TypeIpPort> clientIpPort;

std::vector<std::string> ListenName;
std::vector<TypeIpPort> clientListenID;
std::vector<std::string> ListenName2;
std::vector<TypeIpPort> clientListenID2;

std::vector<myNCB*> clientConnection;

bool receiveServerAddName = false;

bool ReceiveServerAddName() {
	return receiveServerAddName;
}

TypeIpPort GetIpPortFromName(std::string name) {
	for (int i = 0; i < NetworkName.size(); i++)
		if (!NetworkName[i].compare(name))
			return clientIpPort[i];
	TypeIpPort tempIpPort;
	tempIpPort.adress = "x999";
	return tempIpPort;
}

std::string GetNameNetworkFromId(TypeIpPort ipPort) {
	for (int i = 0; i < NetworkName.size(); i++)
		if ((ipPort.adress == clientIpPort[i].adress) && (ipPort.port == clientIpPort[i].port))
			return NetworkName[i];
	return "";
}

std::string GetNameNetwork(std::string name) {
	for (std::string locNetworkName : NetworkName)	
		if (!name.compare(locNetworkName))
			return locNetworkName;
	return "";
}

int GetIndexNetworkIpPort(TypeIpPort ipPort) {
	for (int i = 0; i < clientIpPort.size(); i++)
		if ((ipPort.adress == clientIpPort[i].adress) && (ipPort.port == clientIpPort[i].port))
			return i;
	return -1;
}

int GetNameNetworkIndex(std::string name) {
	for (int i = 0; i < NetworkName.size(); i++)
		if (!name.compare(NetworkName[i]))
			return i;
	return -1;
}

void AddNetworkName(std::string name, TypeIpPort ipPort) {
	if (!GetNameNetwork(name).compare(""))
	{
		NetworkName.push_back(name);
		clientIpPort.push_back(ipPort);
	}
#ifdef TEST_NETWORK_MESSAGES
	debug_net_printf("AddNetworkName - net name added:%s %s %d\n", name.c_str(), ipPort.adress.c_str(), ipPort.port);
#endif //TEST_NETWORK_MESSAGES
}

bool ExistNetworkName(std::string name, TypeIpPort ipPort) {
	for (int i = 0; i < NetworkName.size(); i++)
		if ((!name.compare(NetworkName[i]))&&(clientIpPort[i].adress== ipPort.adress) && (clientIpPort[i].port == ipPort.port))
			return true;
	return false;
}

TypeIpPort GetOtherSide(TypeIpPort ipPort) {
	for (int i = 0; i < clientListenID.size(); i++)
		if ((clientListenID[i].adress== ipPort.adress)&& (clientListenID[i].port == ipPort.port))
			return clientListenID2[i];
	for (int i = 0; i < clientListenID2.size(); i++)
		if ((clientListenID2[i].adress == ipPort.adress)&& (clientListenID2[i].port == ipPort.port))
			return clientListenID[i];
	TypeIpPort tempIpPort;
	tempIpPort.adress = "x1000";
	return tempIpPort;
}

std::string GetListenNetwork(std::string name) {
	for (std::string locListenName : ListenName)
		if (!name.compare(locListenName))
			return locListenName;
	return "";
}

int GetNameListenIndex(std::string name) {
	for (int i = 0; i < ListenName.size(); i++)
		if (!name.compare(ListenName[i]))
			return i;
	return -1;
}

int GetName2ListenIndex(std::string name) {
	for (int i = 0; i < ListenName2.size(); i++)
		if (!name.compare(ListenName2[i]))
			return i;
	return -1;
}

void AddListenName(myNCB* connection) {//10 26
	//if (GetNameNetwork(name).compare(""))
		if (!GetListenNetwork(connection->ncb_callName_10).compare(""))
		{
			ListenName.push_back(connection->ncb_callName_10);
			clientListenID.push_back(GetIpPortFromName(connection->ncb_callName_10));
			ListenName2.push_back(connection->ncb_name_26);
			clientListenID2.push_back(GetIpPortFromName(connection->ncb_name_26));
		#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("AddListenName - listen name added:%s %s\n", connection->ncb_callName_10, connection->ncb_name_26);
		#endif //TEST_NETWORK_MESSAGES
		}
}

bool AddListenName2(const shadow_myNCB* connection){
	TypeIpPort id1 = GetIpPortFromName(connection->ncb_callName_10);
	if(id1.adress=="x999") return false;
	TypeIpPort id2 = GetIpPortFromName(connection->ncb_name_26);
	if (id2.adress == "x999") return false;
	//fix it
	int indexid = GetNameListenIndex(connection->ncb_name_26);
	if (indexid == -1) return false;
	//ListenName[indexid] = connection->ncb_callName_10;
	clientListenID[indexid] = id1;
	//ListenName2[indexid]= connection->ncb_name_26;
	clientListenID2[indexid] = id2;
#ifdef TEST_NETWORK_MESSAGES
	debug_net_printf("listen accepted:%s %d,%s %d\n", connection->ncb_callName_10, indexid, connection->ncb_name_26, id2);
#endif //TEST_NETWORK_MESSAGES
	return true;
}

bool IsListenName2(const shadow_myNCB* connection) {
	TypeIpPort id1 = GetIpPortFromName(connection->ncb_callName_10);
	if (id1.adress == "x999") return false;
	TypeIpPort id2 = GetIpPortFromName(connection->ncb_name_26);
	if (id2.adress == "x999") return false;
	//fix it
	int indexid = GetNameListenIndex(connection->ncb_name_26);
	if (indexid == -1) return false;
	if (ListenName[indexid] != connection->ncb_callName_10)
		return false;
	if ((clientListenID[indexid].adress != id1.adress)|| (clientListenID[indexid].port != id1.port))
		return false;
	if (ListenName2[indexid] != connection->ncb_name_26)
		return false;
	if ((clientListenID2[indexid].adress != id2.adress) || (clientListenID2[indexid].port != id1.port))
		return false;
	return true;
}

void RemoveListenName(std::string name) {
	int index = GetNameListenIndex(name);
	ListenName.erase(ListenName.begin() + index);
	clientListenID.erase(clientListenID.begin() + index);
	ListenName2.erase(ListenName2.begin() + index);
	clientListenID2.erase(clientListenID2.begin() + index);
	index = GetName2ListenIndex(name);
	ListenName.erase(ListenName.begin() + index);
	clientListenID.erase(clientListenID.begin() + index);
	ListenName2.erase(ListenName2.begin() + index);
	clientListenID2.erase(clientListenID2.begin() + index);
}

void RemoveNetworkName(std::string name) {
	int index = GetNameNetworkIndex(name);
	if (index == -1)return;
	NetworkName.erase(NetworkName.begin()+ index);
	clientIpPort.erase(clientIpPort.begin() + index);
}

bool TestAddName(std::string name, uint32_t clientID) {
	if(GetNameNetwork(name).compare(""))
		;
	return false;
}

long oldtime_shared;
int networkTimeout_shared = 10000;

class myMutex {
private:
	bool locked = false;
public:
	void lock() {
		while (locked);
		locked = true;
	};
	void unlock() {
		locked = false;
	};
};//replace std::mutex for debug

//myMutex handleConnections_mt;
std::mutex handleConnections_mt;
std::mutex netstate_mt;
std::mutex networkTimeout_mt;

Neti_type netstate()
{
	Neti_type result;
	netstate_mt.lock();
	result = netstate_shared;
	netstate_mt.unlock();
	return result;
}

void netstate(Neti_type input)
{
	netstate_mt.lock();
	netstate_shared= input;
	netstate_mt.unlock();
}

std::mutex clientConnection_mt;

bool setListen(myNCB* locNCB) {
	clientConnection_mt.lock();
	bool result = false;
	for (int i = 0; i < clientConnection.size(); i++)
		if (memcmp(locNCB->ncb_name_26,clientConnection[i]->ncb_name_26,sizeof(clientConnection[i]->ncb_name_26)))
		{
			clientConnection[i]->ncb_lsn_2 = 20;
			clientConnection[i]->ncb_cmd_cplt_49 = 0;
			result = true;
			break;
		}
	clientConnection_mt.unlock();
	return result;
}

connectionTime* GetConnection(int32_t index) {
	connectionTime* result = NULL;
	for (auto locConn = handleConnections.begin(); locConn != handleConnections.end();)
	{
		if ((*locConn).index == index) {
			result = &(*locConn);
		}
		++locConn;
	}
	return result;
};

void DeleteConnection(int32_t index)
{
	for (auto locConn = handleConnections.begin(); locConn != handleConnections.end();)
	{
		if ((*locConn).index == index) {

			locConn = handleConnections.erase(locConn);
		}
		else {
			++locConn;
		}
	}
};

void SetNetworkTime(int32_t index, int timeout) {
#ifdef TEST_NETWORK_MESSAGES
	//debug_net_printf("handleConnections_mt.lock - SetNetworkTime\n");
#endif //TEST_NETWORK_MESSAGES
	handleConnections_mt.lock();
	for (auto locConn = handleConnections.begin(); locConn != handleConnections.end();)
	{
		if ((*locConn).index == index) {

			(*locConn).timeout = timeout;
		}
		++locConn;
	}
	handleConnections_mt.unlock();
};

int32_t lastIndex = 0;
std::mutex index_mt;

int32_t GetNextIndex() {
	index_mt.lock();
	lastIndex++;
	if (lastIndex > 1000000)lastIndex = 0;
	int32_t locIndex = lastIndex;
	index_mt.unlock();
	return lastIndex;
}

void setListenConnection(myNCB* connection) {
	clientConnection_mt.lock();
	bool result = false;
	for (int i = 0; i < clientConnection.size(); i++)
		if (clientConnection[i] == connection)
		{
			result = true;
			break;
		}
	if (!result)
		clientConnection.push_back(connection);
	clientConnection_mt.unlock();
}

enum { max_length = 1024 };

void deleteListenConnection(myNCB* connection) {
	clientConnection_mt.lock();
	for (int i = 0; i < clientConnection.size(); i++)
		if (clientConnection[i] == connection)
		{
			clientConnection.erase(clientConnection.begin() + i);
			break;
		}
	clientConnection_mt.unlock();
};

namespace MyNetworkLib {
	class typeConfirmedMessage {
	public:
		std::string cxMessage;
		std::string cxDestination_ip;
		unsigned short cxPort;
		std::chrono::system_clock::time_point timestamp;
		typeConfirmedMessage(std::string message, std::string destination_ip, unsigned short port) {
			cxMessage = message;
			cxDestination_ip = destination_ip;
			cxPort = port;
			timestamp = std::chrono::system_clock::now();
		};
	};	

	const int maxConfirmedMessage = 2000;

	class NetworkClass
	{
	private:
		thread receiver_thread;
		thread receiver2_thread;
		thread handler_thread;
		bool HandleReceiver2On;
		bool HandleReceiverOn;
		bool HandleHandlerOn;
		int clPort;
		int clServerPort;
		std::string clHost;
		bool clIam_server = false;
		
		bool IpPortIsSet = false;

		unsigned int stamp = 0;

		asio::io_service io_service_TCP;
		asio::ip::tcp::socket* sockptr_TCP;

		std::vector<TypeIpPort> registered;

		bool serverAddname = false;

		void Receiver2();
		void Receiver();

		bool HandleMessagesServerPre(std::string bufferStr, asio::ip::udp::endpoint sender);
		bool HandleMessagesClientPre(std::string bufferStr, asio::ip::udp::endpoint sender);
		void HandleMessagesServer(std::string bufferStr, asio::ip::udp::endpoint sender);
		void HandleMessagesClient(std::string bufferStr, asio::ip::udp::endpoint sender);

		void Handler();
		void SendGiveIPMessage(asio::ip::udp::endpoint sender, int port);
		
		bool Registered(TypeIpPort ipPort);
		void Register(TypeIpPort ipPort);

		void StartReceiver2();
		void StopReceiver2();
		void StartReceiver();
		void StopReceiver();
		void StartHandler();
		void StopHandler();

		void SendToServer(const std::string& message);
		void SendToClient(const std::string& message, std::string adress, int port);

	public:
		NetworkClass(bool iam_server, std::string host, int port, int serverPort, bool IAmServer);
		~NetworkClass();

		bool SendMessage_UDP(const std::string& message, const std::string& destination_ip, const unsigned short port);

		void AddName(myNCB* connection, int32_t index);
		void AddNameReceive(bool nameIsOk, uint32_t id);
		void CancelNetwork(myNCB* connection, int32_t index);
		void DeleteNetwork(myNCB* connection, int32_t index);
		void CallNetwork(myNCB* connection, int32_t index);
		void ListenNetwork(myNCB* connection, int32_t index);
		void SendNetwork(myNCB* connection, int32_t index);
		void ReceiveNetwork(myNCB* connection);
	};

	NetworkClass::NetworkClass(bool iam_server, std::string host, int port, int serverPort, bool IAmServer) {
		clPort = port;
		clServerPort = serverPort;
		clHost = host;
		clIam_server = iam_server;

		//if ((clIam_server) && (clPort!=clServerPort))
		//	StartReceiverServer();
		StartReceiver();
		StartReceiver2();
		StartHandler();
	};

	NetworkClass::~NetworkClass() {
		//if ((clIam_server) && (clPort != clServerPort))
		//	StopReceiverServer();
		StopReceiver();
		StopReceiver2();
		StopHandler();
	};

	bool NetworkClass::SendMessage_UDP(const std::string& message, const std::string& destination_ip,
		const unsigned short port) {
		if (!IpPortIsSet)
		{
			message_info unpacked_message = Unpack_Message(message);
			if ((unpacked_message.message!= MESS_CLIENT_GET_IP)&& (unpacked_message.message != MESS_SERVER_GIVE_IP))
				return false;
		}

#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("Send message: %s ip: %s port: %d\n", message.c_str(), destination_ip.c_str(), port);
#endif //TEST_NETWORK_MESSAGES
		asio::io_service io_service;
		asio::ip::udp::socket socket(io_service);
		// Create the remote endpoint using the destination ip address and
		// the target port number.  This is not a broadcast
		auto remote = asio::ip::udp::endpoint(asio::ip::address::from_string(destination_ip), port);
		try {
			// Open the socket, socket's destructor will
			// automatically close it.
			if(remote.address().is_v4())
				socket.open(asio::ip::udp::v4());
			else
				socket.open(asio::ip::udp::v6());
			// And send the string... (synchronous / blocking)
			socket.send_to(asio::buffer(message), remote);
		}
#ifdef USE_BOOST__
		catch (const system::system_error& ex)
#else
		catch (std::exception& e)
#endif
		{
			// Exception thrown!
			// Examine ex.code() and ex.what() to see what went wrong!
			return false;
		}
		return true;
	}

	void NetworkClass::StartReceiver2() {
		HandleReceiver2On = true;
		receiver2_thread = std::thread(&NetworkClass::Receiver2, this);
	}

	void NetworkClass::StopReceiver2() {
		HandleReceiver2On = false;
		SendMessage_UDP("Xend", "127.0.0.1", clPort);
		singleThreadSleep(500);
		receiver2_thread.join();
	};

	void NetworkClass::StartReceiver() {
		HandleReceiverOn = true;
		receiver_thread = std::thread(&NetworkClass::Receiver, this);
	}

	void NetworkClass::StopReceiver() {
		HandleReceiverOn = false;
		singleThreadSleep(500);
		receiver_thread.join();
	};

	void NetworkClass::StartHandler() {
		HandleHandlerOn = true;
		handler_thread = std::thread(&NetworkClass::Handler, this);
	}

	void NetworkClass::StopHandler() {
		HandleHandlerOn = false;
		singleThreadSleep(500);
		handler_thread.join();
	};

	void NetworkClass::SendGiveIPMessage(asio::ip::udp::endpoint sender, int port) {		
		shadow_myNCB nullNCB;
		nullNCB.ncb_command_0 = 254;

		SendToClient(Pack_Message(MESS_SERVER_GIVE_IP, nullNCB, -1, port, sender.address().to_string().c_str(), 1+strlen(sender.address().to_string().c_str())), sender.address().to_string(), port);
	};

	bool NetworkClass::Registered(TypeIpPort ipPort) {
		for (int i = 0; i < registered.size(); i++)
			if ((registered[i].adress == ipPort.adress) && (registered[i].port == ipPort.port))
				return true;
		return false;
	}

	void NetworkClass::Register(TypeIpPort ipPort) {
		registered.push_back(ipPort);
	}

	void NetworkClass::Handler() {
		while (HandleHandlerOn)
		{
			if (!IpPortIsSet)
			{
				shadow_myNCB nullNCB;
				nullNCB.ncb_command_0 = 254;
				SendToServer(Pack_Message(MESS_CLIENT_GET_IP, nullNCB, GetNextIndex(), clPort));
				singleThreadSleep(500);
				continue;
			}

			//new
			if (!receiveServerAddName)
			{
				shadow_myNCB nullNCB;
				nullNCB.ncb_command_0 = 254;
				SendToServer(Pack_Message(MESS_CLIENT_SERVER_NAME_ADDED, nullNCB, GetNextIndex(), clPort));
				singleThreadSleep(1000);
			}
			//new
#ifdef TEST_NETWORK_MESSAGES
	//debug_net_printf("handleConnections_mt.lock - clock_t actClock = clock()\n");
#endif //TEST_NETWORK_MESSAGES

			handleConnections_mt.lock();
			for (connectionTime actConnection : handleConnections)//now
			{
				switch (actConnection.connection->ncb_command_0)
				{
					case 0x95: {//RECEIVE(opposite send)//fix it!!!!!!!!!!!!!!!!!!!
						if (GetRecCount() > 0)
						{
							message_info unpacked_message = Unpack_Message(GetRecMess());
							if ((unpacked_message.message != MESS_SERVER_SEND)||(unpacked_message.messNCB.ncb_command_0!=0x94))
							{
								AddRecMess(Pack_Message(unpacked_message.message, unpacked_message.messNCB, unpacked_message.index, clServerPort, unpacked_message.data, unpacked_message.size));
								break;
							}
							if (actConnection.connection->ncb_bufferLength_8 != unpacked_message.size)
							{
								//int a = 1;//this is code only for hunt errors
								//int b = 0;
								//int c = a / b;
								break;
							}
							actConnection.connection->ncb_bufferLength_8 = unpacked_message.size;
							memcpy(actConnection.connection->ncb_buffer_4.p, unpacked_message.data, unpacked_message.size);
							actConnection.connection->ncb_cmd_cplt_49 = 0x0;
	#ifdef TEST_NETWORK_MESSAGES
							debug_net_printf("processEnd: CONVERT FROM MESSAGE:%d:%d\n", actConnection.connection->ncb_bufferLength_8, 10/*strlen(tempstr.c_str())*/);
							debug_net_printf("processEnd: lastconnection set to NULL RECEIVE\n");
	#endif //TEST_NETWORK_MESSAGES
							DeleteConnection(actConnection.index);
						}
						break;
					}
				}
			}
			clock_t actClock = clock();
			for (connectionTime actConnection : handleConnections)
			{
				if (actClock > actConnection.startTime + actConnection.timeout)//after timeout
				{
					bool deleteConnection = true;
#ifdef TEST_NETWORK_MESSAGES
					debug_net_printf("processEnd: WAITING FOR MESSAGE TIMEOUT:%x\n", actConnection.connection->ncb_command_0);
#endif //TEST_NETWORK_MESSAGES			
					switch (actConnection.connection->ncb_command_0)
					{
					case 0x35: {//CANCEL
						actConnection.connection->ncb_retcode_1 = 0x0;
						strcpy(actConnection.connection->ncb_name_26, "");
						strcpy(actConnection.connection->ncb_callName_10, "");
						actConnection.connection->ncb_cmd_cplt_49 = 0x0;
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("processEnd: lastconnection set to NULL CANCEL\n");
#endif //TEST_NETWORK_MESSAGES
						break;
					}
					case 0x7F: {//INIT
						actConnection.connection->ncb_cmd_cplt_49 = 0;
						actConnection.connection = NULL;
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("processEnd: lastconnection set to NULL INIT\n");
#endif //TEST_NETWORK_MESSAGES
						break;
					}
					case 0x90: {//CALL(opposite listen)
						if (netstate() == NETI_CALL)
						{
							actConnection.startTime = actClock;
							deleteConnection = false;
							SendToServer(Pack_Message(MESS_CLIENT_MESSAGE_CALL, myNCBtoShadow(*actConnection.connection), actConnection.index, clPort, actConnection.connection->ncb_name_26, sizeof(actConnection.connection->ncb_name_26)));
						}
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("processEnd: lastconnection set to NULL CALL\n");
#endif //TEST_NETWORK_MESSAGES
						break;
					}
					case 0x91: {//LISTEN(opposite call)
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("processEnd: lastconnection set to NULL LISTEN\n");
#endif //TEST_NETWORK_MESSAGES
						break;
					}
					case 0x94: {//SEND(opposite receive)
						actConnection.connection->ncb_cmd_cplt_49 = 0;
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("processEnd: lastconnection set to NULL SEND\n");
#endif //TEST_NETWORK_MESSAGES
						break;
					}
					case 0x95: {//RECEIVE(opposite send)//fix it!!!!!!!!!!!!!!!!!!!
						if (GetRecCount() > 0)
						{
							message_info unpacked_message = Unpack_Message(GetRecMess());
							if ((unpacked_message.message != MESS_SERVER_SEND) || (unpacked_message.messNCB.ncb_command_0 != 0x94))
							{
								AddRecMess(Pack_Message(unpacked_message.message, unpacked_message.messNCB, unpacked_message.index, clServerPort, unpacked_message.data, unpacked_message.size));
								break;
							}
						}
						actConnection.connection->ncb_bufferLength_8 = 0;
						actConnection.connection->ncb_cmd_cplt_49 = 0;//10//timeout
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("processEnd: CONVERT FROM MESSAGE - timeout:%d:%d\n", actConnection.connection->ncb_bufferLength_8, 10/*strlen(tempstr.c_str())*/);
						debug_net_printf("processEnd: lastconnection set to NULL RECEIVE - timeout\n");
#endif //TEST_NETWORK_MESSAGES
						break;
					}
					case 0xb0: {//ADD_NAME
						if (netstate() == NETI_ADD_NAME_REJECT)
						{
							actConnection.connection->ncb_cmd_cplt_49 = 22;
#ifdef TEST_NETWORK_MESSAGES
							debug_net_printf("processEnd: NETI_ADD_NAME_NOTOK\n");
#endif //TEST_NETWORK_MESSAGES
						}
						else if (netstate() == NETI_ADD_NAME_OK)
						{
							IHaveNameStrP = actConnection.connection->ncb_name_26;
							actConnection.connection->ncb_cmd_cplt_49 = 0;
#ifdef TEST_NETWORK_MESSAGES
							debug_net_printf("processEnd: NETI_ADD_NAME_OK\n");
#endif //TEST_NETWORK_MESSAGES
						}
						else
						{
							actConnection.startTime = actClock;
							deleteConnection = false;
							SendToServer(Pack_Message(MESS_CLIENT_TESTADDNAME, myNCBtoShadow(*actConnection.connection), actConnection.index, clPort, actConnection.connection->ncb_name_26, sizeof(actConnection.connection->ncb_name_26)));
							//actConnection.connection->ncb_cmd_cplt_49 = 22;
#ifdef TEST_NETWORK_MESSAGES
							debug_net_printf("I try MESS_CLIENT_TESTADDNAME again\n");
#endif //TEST_NETWORK_MESSAGES
						}
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("processEnd: lastconnection set to NULL ADD_NAME\n");
#endif //TEST_NETWORK_MESSAGES
						break;
					}
					case 0xb1: {//DELETE_NAME 
						actConnection.connection->ncb_cmd_cplt_49 = 0;
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("processEnd: lastconnection set to NULL INIT\n");
#endif //TEST_NETWORK_MESSAGES
						break;
					}

					default: {
						actConnection.connection->ncb_cmd_cplt_49 = 0;
					}

					}
					if (deleteConnection)
						DeleteConnection(actConnection.index);
				}
			}
			handleConnections_mt.unlock();

			singleThreadSleep(1);
		}
	}

	bool NetworkClass::HandleMessagesServerPre(std::string bufferStr, asio::ip::udp::endpoint sender)
	{
		message_info unpacked_message = Unpack_Message(bufferStr);
		if (unpacked_message.message == MESS_CLIENT_GET_IP)
		{
			SendGiveIPMessage(sender, unpacked_message.port);
			return true;
		}
		return false;
	}

	bool NetworkClass::HandleMessagesClientPre(std::string bufferStr, asio::ip::udp::endpoint sender)
	{
		message_info unpacked_message = Unpack_Message(bufferStr);
		if (unpacked_message.message == MESS_SERVER_GIVE_IP)
		{
			IpPortIsSet = true;
			return true;
		}
		return false;
	}

	void NetworkClass::HandleMessagesServer(std::string bufferStr, asio::ip::udp::endpoint sender)
	{
	#ifdef TEST_NETWORK_MESSAGES
		//debug_net_printf("Server Get Message:%s\n", messages[0].c_str());
	#endif //TEST_NETWORK_MESSAGES

		message_info unpacked_message = Unpack_Message(bufferStr);

		//myprintf("Server: message - %s\n", MessageIndexToText(unpacked_message.message));
	#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("Server: message - %s\n", MessageIndexToText(unpacked_message.message));
	#endif //TEST_NETWORK_MESSAGES

		if (unpacked_message.message == MESS_CLIENT_SERVER_NAME_ADDED)
		{
			TypeIpPort locIpPort;
			locIpPort.adress = sender.address().to_string();
			locIpPort.port = unpacked_message.port;
			if (!Registered(locIpPort))
			{
				Register(locIpPort);
			}
			if (serverAddname)
			{
				shadow_myNCB nullNCB;
				nullNCB.ncb_command_0 = 254;
				SendToClient(Pack_Message(MESS_SERVER_SERVER_NAME_ADDED, nullNCB, unpacked_message.index, - 10), sender.address().to_string(), unpacked_message.port);
	#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("Client %s can AddName\n", sender.address().to_string().c_str());
	#endif //TEST_NETWORK_MESSAGES
			}
		}

		else if (unpacked_message.message == MESS_CLIENT_TESTADDNAME) {
			TypeIpPort locIpPort;
			locIpPort.adress = sender.address().to_string();
			locIpPort.port = unpacked_message.port;
			if ((!GetNameNetwork(unpacked_message.data).compare("")))
			{
				AddNetworkName(unpacked_message.data, locIpPort);
				shadow_myNCB nullNCB;
				nullNCB.ncb_command_0 = 254;
				SendToClient(Pack_Message(MESS_SERVER_TESTADDNAME_OK, nullNCB, unpacked_message.index, -10), sender.address().to_string(), unpacked_message.port);
	#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("Server: MESSAGE_TESTADDNAME OK:%s %d\n", unpacked_message.data, sender.address().to_string());
	#endif //TEST_NETWORK_MESSAGES
				char compText[16];
				sprintf(compText, "NETH2%c0        ", unpacked_message.data[5]);
				if (!strcmp(unpacked_message.data, compText))//is Server
					serverAddname = true;
			}
			else if (ExistNetworkName(unpacked_message.data, locIpPort))
			{
				shadow_myNCB nullNCB;
				nullNCB.ncb_command_0 = 254;
				SendToClient(Pack_Message(MESS_SERVER_TESTADDNAME_OK, nullNCB, unpacked_message.index, -10), sender.address().to_string(), unpacked_message.port);
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("Server: MESSAGE_TESTADDNAME OK:%s %d\n", unpacked_message.data, sender.address().to_string());
#endif //TEST_NETWORK_MESSAGES
			}
			else
			{
				shadow_myNCB nullNCB;
				nullNCB.ncb_command_0 = 254;
				SendToClient(Pack_Message(MESS_SERVER_TESTADDNAME_REJECT, nullNCB, unpacked_message.index, - 10), sender.address().to_string(), unpacked_message.port);
	#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("Server: MESSAGE_TESTADDNAME REJECT:%s %d\n", unpacked_message.data, sender.address().to_string());
	#endif //TEST_NETWORK_MESSAGES
			}
		}
		else if (unpacked_message.message == MESS_CLIENT_MESSAGE_LISTEN)
		{
			AddListenName((myNCB*)unpacked_message.data);
			//"NETH200        "
	#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("Server: MESSAGE_LISTEN:%s ; %s %s\n", ((shadow_myNCB*)unpacked_message.data)->ncb_callName_10, ((myNCB*)unpacked_message.data)->ncb_name_26, sender.address().to_string().c_str());
	#endif //TEST_NETWORK_MESSAGES
		}
		else if (unpacked_message.message == MESS_CLIENT_MESSAGE_CALL)
		{
			TypeIpPort callLocIpPort = GetIpPortFromName(unpacked_message.messNCB.ncb_callName_10);
			if (AddListenName2(&unpacked_message.messNCB))
			{
				SendToClient(Pack_Message(MESS_SERVER_CALL_ACCEPT, unpacked_message.messNCB, unpacked_message.index), sender.address().to_string(), unpacked_message.port);
				SendToClient(Pack_Message(MESS_SERVER_LISTEN_ACCEPT, unpacked_message.messNCB, unpacked_message.index, clServerPort, unpacked_message.data, unpacked_message.size), callLocIpPort.adress, callLocIpPort.port);
				//"NETH200        "
	#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("Server: NETI_LISTEN_CONNECTED:%s %s %s\n", unpacked_message.messNCB.ncb_callName_10, unpacked_message.messNCB.ncb_name_26, sender.address().to_string().c_str());
	#endif //TEST_NETWORK_MESSAGES
			}/* else if (IsListenName2(&unpacked_message.messNCB)) {
				SendToClient(Pack_Message(MESS_SERVER_CALL_ACCEPT, unpacked_message.messNCB, unpacked_message.index, -10), sender.address().to_string(), unpacked_message.port);
				SendToClient(Pack_Message(MESS_SERVER_LISTEN_ACCEPT, unpacked_message.messNCB, unpacked_message.index, -10, clServerPort, unpacked_message.data, unpacked_message.size), callLocIpPort.adress, callLocIpPort.port);
				//"NETH200        "
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("Server: NETI_LISTEN_CONNECTED:%s %s %s\n", unpacked_message.messNCB.ncb_callName_10, unpacked_message.messNCB.ncb_name_26, sender.address().to_string().c_str());
#endif //TEST_NETWORK_MESSAGES
			}*/
			else
			{
				SendToClient(Pack_Message(MESS_SERVER_CALL_REJECT, unpacked_message.messNCB, unpacked_message.index, -10), sender.address().to_string(), unpacked_message.port);
				SendToClient(Pack_Message(MESS_SERVER_LISTEN_REJECT, unpacked_message.messNCB, unpacked_message.index, -10), callLocIpPort.adress, callLocIpPort.port);
				//"NETH200        "
	#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("Server: NETI_LISTEN_REJECT:%s %s %s\n", unpacked_message.messNCB.ncb_callName_10, unpacked_message.messNCB.ncb_name_26, sender.address().to_string().c_str());
	#endif //TEST_NETWORK_MESSAGES
			}
		}
		else if (unpacked_message.message == MESS_CLIENT_CANCEL)
		{
			if (unpacked_message.data[0] != 0)
				RemoveListenName(unpacked_message.data);
	#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("Server: MESSAGE_CANCEL:%s %s\n", unpacked_message.data, sender.address().to_string().c_str());
	#endif //TEST_NETWORK_MESSAGES
		}
		else if (unpacked_message.message == MESS_CLIENT_DELETE)
		{
			CleanMessages(myNCBfromShadow(unpacked_message.messNCB));
			RemoveNetworkName(unpacked_message.data);
	#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("Server: MESSAGE_DELETE:%s %s\n", unpacked_message.data, sender.address().to_string().c_str());
	#endif //TEST_NETWORK_MESSAGES
		}
		else if (unpacked_message.message == MESS_CLIENT_SEND)
		{
			TypeIpPort locIpPort;
			locIpPort.adress = sender.address().to_string();
			locIpPort.port = unpacked_message.port;
			TypeIpPort otherid = GetOtherSide(locIpPort);
			if (otherid.adress != "x1000")
			{
				SendToClient(Pack_Message(MESS_SERVER_SEND, unpacked_message.messNCB, unpacked_message.index, clServerPort, unpacked_message.data, unpacked_message.size), otherid.adress, otherid.port);
				SendToClient(Pack_Message(MESS_SERVER_SEND_OK, unpacked_message.messNCB, unpacked_message.index), sender.address().to_string(), unpacked_message.port);
			}
	#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("Server: MESSAGE_SEND:%s %d %s %d\n", /*unpacked_message.data,*/ otherid.adress.c_str(), otherid.port, sender.address().to_string().c_str(), unpacked_message.port);
	#endif //TEST_NETWORK_MESSAGES
		}
	}

	void NetworkClass::HandleMessagesClient(std::string bufferStr, asio::ip::udp::endpoint sender)
	{
		//client
	#ifdef TEST_NETWORK_MESSAGES
	//debug_net_printf("Client Get Message:%s\n", messages[0].c_str());
	#endif //TEST_NETWORK_MESSAGES
		message_info unpacked_message = Unpack_Message(bufferStr);

		//myprintf("Client: message - %s\n", MessageIndexToText(unpacked_message.message));
	#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("Client: message - %s\n", MessageIndexToText(unpacked_message.message));
	#endif //TEST_NETWORK_MESSAGES

		if (unpacked_message.message == MESS_SERVER_SERVER_NAME_ADDED)
		{
			receiveServerAddName = true;
			//myprintf("Can Add Name...\n");
		}
		else if (unpacked_message.message == MESS_SERVER_TESTADDNAME_OK)
		{
			netstate(NETI_ADD_NAME_OK);
			SetNetworkTime(unpacked_message.index, 0);
	#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("Client: NETI_ADD_NAME_OK:\n");
	#endif //TEST_NETWORK_MESSAGES
		}
		else if (unpacked_message.message == MESS_SERVER_TESTADDNAME_REJECT)
		{
			netstate(NETI_ADD_NAME_REJECT);
			SetNetworkTime(unpacked_message.index, 0);
	#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("CLIENT NETI_ADD_NAME_REJECT:\n");
	#endif //TEST_NETWORK_MESSAGES
		}
		else if (unpacked_message.message == MESS_SERVER_CALL_ACCEPT)
		{
			netstate(NETI_CALL_ACCEPT);

	#ifdef TEST_NETWORK_MESSAGES
			//debug_net_printf("handleConnections_mt.lock - //lastconnection_shared->ncb_lsn_2 = 20;\n");
	#endif //TEST_NETWORK_MESSAGES
			handleConnections_mt.lock();
			//lastconnection_shared->ncb_lsn_2 = 20;
			connectionTime* ptrConn = GetConnection(unpacked_message.index);
			if (ptrConn)
			{
				myNCB* mypointer = ptrConn->connection;
				mypointer->ncb_retcode_1 = 0x00;
				mypointer->ncb_cmd_cplt_49 = 0x00;
			}

			handleConnections_mt.unlock();

			SetNetworkTime(unpacked_message.index, 0);
#ifdef TEST_NETWORK_MESSAGES
			if (ptrConn)
				debug_net_printf("CLIENT MESSAGE_CALL_ACCEPT: %p|%s|%s|\n", ptrConn->connection, ptrConn->connection->ncb_callName_10, ptrConn->connection->ncb_name_26);
			else
				debug_net_printf("CLIENT MESSAGE_CALL_ACCEPT\n");
#endif //TEST_NETWORK_MESSAGES
		}
		else if (unpacked_message.message == MESS_SERVER_CALL_REJECT)
		{
			netstate(NETI_CALL_REJECT);
			SetNetworkTime(unpacked_message.index, 0);
	#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("CLIENT MESSAGE_CALL_REJECT:\n");
	#endif //TEST_NETWORK_MESSAGES
		}
		else if (unpacked_message.message == MESS_SERVER_LISTEN_ACCEPT)
		{
			//lastconnection_shared->ncb_lsn_2 = 20;
			setListen((myNCB*)unpacked_message.data);
	#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("CLIENT MESSAGE_LISTEN_ACCEPT:\n");
	#endif //TEST_NETWORK_MESSAGES
		}
		else if (unpacked_message.message == MESS_SERVER_LISTEN_REJECT)
		{
	#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("CLIENT MESSAGE_LISTEN_REJECT:\n");
	#endif //TEST_NETWORK_MESSAGES
		}
		else if (unpacked_message.message == MESS_SERVER_SEND)
		{
			AddRecMess(Pack_Message(MESS_SERVER_SEND, unpacked_message.messNCB, unpacked_message.index, clServerPort, unpacked_message.data, unpacked_message.size));
			SetNetworkTime(unpacked_message.index, 0);
	#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("CLIENT MESSAGE_SEND:\n");
	#endif //TEST_NETWORK_MESSAGES
		}
		else if (unpacked_message.message == MESS_SERVER_SEND_OK)
		{
			SetNetworkTime(unpacked_message.index, 0);
	#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("CLIENT MESSAGE_SEND OK:\n");
	#endif //TEST_NETWORK_MESSAGES
		}
	}

	typedef struct {
		std::string messages;
		asio::ip::udp::endpoint sender;
	} type_savedMessages;
	std::vector<type_savedMessages> savedMessages;

	void NetworkClass::Receiver2() {
		asio::io_context io_context;
		asio::ip::udp adress_type = asio::ip::udp::v6();
		if (asio::ip::address::from_string(clHost).is_v4())
			asio::ip::udp adress_type = asio::ip::udp::v4();
		asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(adress_type, clPort));
		while (HandleReceiver2On)
		{
			char buffer[65536];
			asio::ip::udp::endpoint sender;
			std::size_t bytes_transferred = socket.receive_from(asio::buffer(buffer), sender);
			std::string bufferStr;
			bufferStr.assign(buffer, bytes_transferred);
			type_savedMessages locMessage;
			locMessage.messages = bufferStr;
			locMessage.sender = sender;

			if (bufferStr == "Xend")continue;
#ifdef TEST_BAD_NETWORK
			message_info unpacked_message = Unpack_Message(bufferStr);
			if ((rand() % 3 == 0)||(unpacked_message.message == MESS_CLIENT_MESSAGE_LISTEN) || (unpacked_message.message == MESS_SERVER_LISTEN_ACCEPT))
			{
#endif //TEST_BAD_NETWORK
				savedMessages.push_back(locMessage);
#ifdef TEST_BAD_NETWORK
			}
#endif //TEST_BAD_NETWORK
			singleThreadSleep(1);
		}
	}

	void NetworkClass::Receiver() {
		while (HandleReceiverOn)
		{
			if (savedMessages.size() > 0)
			{
				type_savedMessages locMessage = savedMessages.back();
				savedMessages.pop_back();
				asio::ip::udp::endpoint sender = locMessage.sender;
				std::string bufferStr = locMessage.messages;
				bool handleMessage = true;

#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("Receive message: %s ip: %s port: %d\n", bufferStr.c_str(), sender.address().to_string().c_str(), clPort);
#endif //TEST_NETWORK_MESSAGES

				if (bufferStr.size() == 0)
					continue;

				if (clIam_server)
					if (HandleMessagesServerPre(bufferStr, sender))
						continue;
				if (HandleMessagesClientPre(bufferStr, sender))
					continue;

				if (handleMessage)
				{
					if (clIam_server/* && clPort == clServerPort*/)
						HandleMessagesServer(bufferStr, sender);
					HandleMessagesClient(bufferStr, sender);
				}
			}
			singleThreadSleep(1);
		}
	}

	void NetworkClass::SendToServer(const std::string& message) {
		SendMessage_UDP(message, clHost, clServerPort);
	};

	void NetworkClass::SendToClient(const std::string& message, std::string adress, int port) {
		SendMessage_UDP(message, adress, port);
	};

	void NetworkClass::AddName(myNCB* connection, int32_t index) {
		SendToServer(Pack_Message(MESS_CLIENT_TESTADDNAME, myNCBtoShadow(*connection), index, clPort, connection->ncb_name_26, sizeof(connection->ncb_name_26)));
	};

	void NetworkClass::AddNameReceive(bool nameIsOk, uint32_t id)
	{
		if (nameIsOk)
			;// AddNetworkName(std::string name, uint32_t id)
		else
			;//xx
	}

	void NetworkClass::CancelNetwork(myNCB* connection, int32_t index) {
		SendToServer(Pack_Message(MESS_CLIENT_CANCEL, myNCBtoShadow (*connection), index, clPort, connection->ncb_name_26, sizeof(connection->ncb_name_26)));
		deleteListenConnection(connection);
	}

	void NetworkClass::DeleteNetwork(myNCB* connection, int32_t index) {
		SendToServer(Pack_Message(MESS_CLIENT_DELETE, myNCBtoShadow(*connection), index, clPort, connection->ncb_name_26, sizeof(connection->ncb_name_26)));
		singleThreadSleep(400);
		CleanMessages(*connection);
		savedMessages.clear();
	}

	void NetworkClass::CallNetwork(myNCB* connection, int32_t index) {
		SendToServer(Pack_Message(MESS_CLIENT_MESSAGE_CALL, myNCBtoShadow(*connection), index, clPort, (char*)connection, sizeof(*connection)));// +connection->ncb_name_26 + std::string(";") + connection->ncb_callName_10);
	};

	void NetworkClass::ListenNetwork(myNCB* connection, int32_t index) {
		SendToServer(Pack_Message(MESS_CLIENT_MESSAGE_LISTEN, myNCBtoShadow(*connection), index, clPort, (char*)connection, sizeof(*connection)));/*->ncb_callName_10 + ';' + connection->ncb_name_26*/
		setListenConnection(connection);
	};

	void NetworkClass::SendNetwork(myNCB* connection, int32_t index) {
		if (connection->ncb_command_0 != 0x94)
		{
			int a = 10;
			int b = 0;
			int c = a / b;
		}
		SendToServer(Pack_Message(MESS_CLIENT_SEND, myNCBtoShadow(*connection), index, clPort, (char*)connection->ncb_buffer_4.p, connection->ncb_bufferLength_8));
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("CONVERT TO MESSAGE:%d:%d\n", connection->ncb_bufferLength_8, connection->ncb_bufferLength_8);
#endif //TEST_NETWORK_MESSAGES
	};

	void NetworkClass::ReceiveNetwork(myNCB* connection) {
	};
}

MyNetworkLib::NetworkClass* locNetworkClass;


int messageToDelete = 1000;

std::vector<std::string> recMessages;
std::mutex RecMess_mt;

void AddRecMess(std::string message){
	RecMess_mt.lock();
	recMessages.push_back(message);
	RecMess_mt.unlock();
};


unsigned int GetRecCount() {
	RecMess_mt.lock();
	unsigned int result= recMessages.size();;
	RecMess_mt.unlock();
	return result;
};

std::string GetRecMess() {
	std::string result;
	RecMess_mt.lock();
	result = recMessages.front();
	recMessages.erase(recMessages.begin());
	RecMess_mt.unlock();
	return result;
};

void CleanMessages(myNCB locNCB) {
	std::string result;
	RecMess_mt.lock();
	for (auto it = recMessages.begin(); it != recMessages.end();)
	{
		message_info unpackedMess = Unpack_Message(*it);
		if ((memcpy(unpackedMess.messNCB.ncb_name_26, locNCB.ncb_name_26,sizeof(locNCB.ncb_name_26)))||
			(memcpy(unpackedMess.messNCB.ncb_callName_10, locNCB.ncb_name_26, sizeof(locNCB.ncb_name_26))))
		{
			it = recMessages.erase(it);
		}
		else {
			++it;
		}
	}
	RecMess_mt.unlock();
}

int lastnetworkname = 0;

void makeNetwork(myNCB* connection) {
	//v10x->esi = 0;
	int locTimeout = 0;
	int32_t locIndex = GetNextIndex();
	switch (connection->ncb_command_0) {
	case 0x35: {//CANCEL
		locTimeout = 10000;		
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("makeNetwork - SET NETWORK CANCEL %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		myNCB* secondcon = (myNCB*)connection->ncb_buffer_4.p;
		if (secondcon)
		{
			//*secondcon = *connection;
			secondcon->ncb_retcode_1 = 0x0b;
			secondcon->ncb_cmd_cplt_49 = 0x0b;
		}

		connection->ncb_retcode_1 = 0x00;
		connection->ncb_lsn_2 = 0x00;
		connection->ncb_cmd_cplt_49 = 0x00;
		locNetworkClass->CancelNetwork(connection, locIndex);
		break;
	}
	case 0x7f: {//? 
		locTimeout = 200;
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("makeNetwork - SET NETWORK INIT\n");
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0x03;
		connection->ncb_cmd_cplt_49 = 0x03;
		break;
	}
	case 0x90: {//CALL
		locTimeout = 1000;
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("makeNetwork - SET NETWORK CALL %p|%s|%s|\n", connection,connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_lsn_2 = 0xe8;
		connection->ncb_cmd_cplt_49 = 0xff;
		locNetworkClass->CallNetwork(connection, locIndex);
		netstate(NETI_CALL);
		break;
	}
	case 0x91: {//LISTEN
		locTimeout = 200;
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("makeNetwork - SET NETWORK LISTEN %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_cmd_cplt_49 = 0xff;
		locNetworkClass->ListenNetwork(connection, locIndex);
		netstate(NETI_LISTEN);
		singleThreadSleep(200);
		break;
	}
	case 0x92: {//HANG UP
		break;
	}
	case 0x94: {//SEND
		locTimeout = 0;
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("makeNetwork - SET NETWORK SEND %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_cmd_cplt_49 = 0xff;

		locNetworkClass->SendNetwork(connection, locIndex);
		break;
	}
	case 0x95: {//RECEIVE
		locTimeout = 1500;//500;
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("makeNetwork - SET NETWORK RECEIVE %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_cmd_cplt_49 = 0xff;

		locNetworkClass->ReceiveNetwork(connection);
		netstate(NETI_RECEIVE);

		break;
	}
	case 0xb0: {//ADD_NAME 
		locTimeout = 1000;
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("makeNetwork - SET NETWORK ADD_NAME %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_num_3 = lastnetworkname + 0x02;
		lastnetworkname++;
		connection->ncb_cmd_cplt_49 = 0xff;

		locNetworkClass->AddName(connection, locIndex);

		netstate(NETI_ADD_NAME);

		break;
	}
	case 0xb1: {//DELETE_NAME 
		locTimeout = 10000;
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("makeNetwork - SET NETWORK DELETE_NAME %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		locNetworkClass->DeleteNetwork(connection, locIndex);

		break;
	}
		 
	}

	connectionTime locConn;
	locConn.connection = connection;
	locConn.timeout = locTimeout;
	locConn.startTime = clock();
	locConn.index = locIndex;
#ifdef TEST_NETWORK_MESSAGES
	//debug_net_printf("handleConnections_mt.lock - locConn.index = locIndex;\n");
#endif //TEST_NETWORK_MESSAGES
	handleConnections_mt.lock();
	handleConnections.push_back(locConn);
	handleConnections_mt.unlock();
	/*
	if (connection->ncb_command_0 == 0x95)
	{
		bool runAgain = true;
		while (true)
		{
			runAgain = false;
#ifdef TEST_NETWORK_MESSAGES
			//debug_net_printf("handleConnections_mt.lock - for(connectionTime actConn: handleConnections)\n");
#endif //TEST_NETWORK_MESSAGES
			handleConnections_mt.lock();
			for(connectionTime actConn: handleConnections)
				if ((actConn.connection->ncb_command_0 == connection->ncb_command_0) &&
					(memcpy(actConn.connection->ncb_name_26, connection->ncb_name_26, sizeof(connection->ncb_name_26))))
				{
					runAgain=true;
				}
			handleConnections_mt.unlock();
		}
	}*/
#ifdef TEST_NETWORK_MESSAGES
	debug_net_printf("makeNetwork - SET CONNECTION\n");
#endif //TEST_NETWORK_MESSAGES
}

void printState(myNCB** connections) {
#ifdef TEST_NETWORK_MESSAGES
	debug_net_printf("NetworkGetState: %d %p %d %s\n", 0, connections[0], connections[0]->ncb_lsn_2, (!connections[0]->ncb_cmd_cplt_49) ? "true" : "false");
	debug_net_printf("NetworkGetState: %d %p %d %s\n", 1, connections[1], connections[1]->ncb_lsn_2, (!connections[1]->ncb_cmd_cplt_49) ? "true" : "false");
	debug_net_printf("NetworkGetState: %d %p %d %s\n", 2, connections[2], connections[2]->ncb_lsn_2, (!connections[2]->ncb_cmd_cplt_49) ? "true" : "false");
#endif //TEST_NETWORK_MESSAGES
}

void printState2(char* text) {
#ifdef TEST_NETWORK_MESSAGES
	debug_net_printf("%s", text);
#endif //TEST_NETWORK_MESSAGES
}

void timeState(bool start, const char* text) {
#ifdef TEST_TIME_NETWORK_MESSAGES
	if (start||(timest_index==0))timest_timer = clock();
	char buff[100];
	snprintf(buff, sizeof(buff), "%s | %d", text, clock()- timest_timer);
	timest_buffer[timest_index].assign(buff, strlen(buff));
	timest_index++;
	if (timest_index > timest_max_mess) {
		ofstream ofs("net_time_messages_log.txt", std::ofstream::out);
		for (int i = 0; i < timest_max_mess; i++) {

			ofs << timest_buffer[i] << endl; // I also tried replacing endl with a "\n"
		}
		ofs.close();
		exit(0);
	}
#endif //TEST_TIME_NETWORK_MESSAGES
}

void InitMyNetLib(bool iam_server, char* ip, int networkPort, int serverPort)
{
	if (iam_server)networkPort = serverPort;
	locNetworkClass = new MyNetworkLib::NetworkClass(iam_server, ip, networkPort, serverPort,true);
}

void EndMyNetLib() {
	singleThreadSleep(500);
	delete locNetworkClass;
}

