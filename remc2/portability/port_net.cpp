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

static const int NetworkBufferSize = 4096;

//#define TEST_NETWORK_MESSAGES

//#define USE_CONFIRM_MESSAGES //can not use for now


using namespace std;

myNCB* lastconnection_shared;

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
	}
	return (char*)"";
}

#pragma pack (1)
typedef struct message_info{
	uint32_t size;
	int32_t message;
	myNCB messNCB;
	char data[2048*30];
};
#pragma pack (16)

std::string DataToString(message_info messInfo)
{
	std::string output;
	for (int i = 0; i < messInfo.size + sizeof(myNCB) + 8; i++)
	{
		output += ((char*)&messInfo)[i];
	}
	return output;
}

std::string Pack_Message(uint32 message, myNCB locNCB, char* data = NULL, int size_of_data = 0) {
	message_info locmessage_info;
	locmessage_info.message = message;
	locmessage_info.size = size_of_data;
	locmessage_info.messNCB = locNCB;
	if(data)
		memcpy(locmessage_info.data, data, size_of_data);
	return DataToString(locmessage_info);
}

message_info Unpack_Message(std::string data) {
	message_info output;
	for (int i = 0; i < sizeof(myNCB) + 8; i++)
		((char*)(&output))[i] = data[i];
	for (int i = 12; i < output.size + sizeof(myNCB) + 8; i++)
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
std::vector<std::string> clientID;

std::vector<std::string> ListenName;
std::vector<std::string> clientListenID;
std::vector<std::string> ListenName2;
std::vector<std::string> clientListenID2;

std::vector<myNCB*> clientConnection;

bool receiveServerAddName = false;

bool ReceiveServerAddName() {
	return receiveServerAddName;
}

std::string GetIdNetworkFromName(std::string name) {
	for (int i = 0; i < NetworkName.size(); i++)
		if (!NetworkName[i].compare(name))
			return clientID[i];
	return "x999";
}

std::string GetNameNetworkFromId(std::string ip) {
	for (int i = 0; i < NetworkName.size(); i++)
		if (clientID[i]==ip)
			return NetworkName[i];
	return "";
}

std::string GetNameNetwork(std::string name) {
	for (std::string locNetworkName : NetworkName)	
		if (!name.compare(locNetworkName))
			return locNetworkName;
	return "";
}

int GetIndexNetworkId(std::string ip) {
	for (int i = 0; i < clientID.size(); i++)
		if (ip==clientID[i])
			return i;
	return -1;
}

int GetNameNetworkIndex(std::string name) {
	for (int i = 0; i < NetworkName.size(); i++)
		if (!name.compare(NetworkName[i]))
			return i;
	return -1;
}

void AddNetworkName(std::string name, std::string ip) {
	if (!GetNameNetwork(name).compare(""))
	{
		NetworkName.push_back(name);
		clientID.push_back(ip);
	}
#ifdef TEST_NETWORK_MESSAGES
	debug_net_printf("AddNetworkName - net name added:%s %s\n", name.c_str(), ip.c_str());
#endif //TEST_NETWORK_MESSAGES
}

std::string GetOtherSide(std::string id) {
	for (int i = 0; i < clientListenID.size(); i++)
		if (clientListenID[i]== id)
			return clientListenID2[i];
	for (int i = 0; i < clientListenID2.size(); i++)
		if (clientListenID2[i] == id)
			return clientListenID[i];
	return "x1000";
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
			clientListenID.push_back(GetIdNetworkFromName(connection->ncb_callName_10));
			ListenName2.push_back(connection->ncb_name_26);
			clientListenID2.push_back(GetIdNetworkFromName(connection->ncb_name_26));
		#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("AddListenName - listen name added:%s %s\n", connection->ncb_callName_10, connection->ncb_name_26);
		#endif //TEST_NETWORK_MESSAGES
		}
}

bool AddListenName2(myNCB* connection){
	std::string id1 = GetIdNetworkFromName(connection->ncb_callName_10);
	if(id1=="x999")return false;
	std::string id2 = GetIdNetworkFromName(connection->ncb_name_26);
	if (id2 == "x999")return false;
	//fix it
	int indexid = GetNameListenIndex(connection->ncb_name_26);
	if(indexid==-1)return false;
	ListenName[indexid] = connection->ncb_callName_10;
	clientListenID[indexid] = id1;
	ListenName2[indexid]= connection->ncb_name_26;
	clientListenID2[indexid] = id2;
#ifdef TEST_NETWORK_MESSAGES
	debug_net_printf("listen accepted:%s %d,%s %d\n", connection->ncb_callName_10, indexid, connection->ncb_name_26, id2);
#endif //TEST_NETWORK_MESSAGES
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
	clientID.erase(clientID.begin() + index);
}

bool TestAddName(std::string name, uint32_t clientID) {
	if(GetNameNetwork(name).compare(""))
		;
	return false;
}

long oldtime_shared;
int networkTimeout_shared = 10000;

std::mutex lastconnection_mt;
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

int networkTimeout()
{
	int result;
	networkTimeout_mt.lock();
	result = networkTimeout_shared;
	networkTimeout_mt.unlock();
	return result;
}

void networkTimeout(int input)
{
	networkTimeout_mt.lock();
	networkTimeout_shared = input;
	networkTimeout_mt.unlock();
#ifdef TEST_NETWORK_MESSAGES
	debug_net_printf("networkTimeout - change timeout %d\n", input);
#endif //TEST_NETWORK_MESSAGES
}

void resetTimeout()
{
	networkTimeout_mt.lock();
	oldtime_shared = clock();
	networkTimeout_mt.unlock();
}

long oldtime()
{
	long result;
	networkTimeout_mt.lock();
	result = oldtime_shared;
	networkTimeout_mt.unlock();
	return result;
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


enum { max_length = 1024 };

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
		thread sender_thread;
		thread sendComfirm_thread;
		bool HandleReceiverOn;
		bool HandleSenderOn;
		bool HandleSendconfirmOn;
		unsigned short clPort;
		std::string clHost;
		bool clIam_server = false;
		typeConfirmedMessage* confirmedMessage[maxConfirmedMessage];
		int indexConfirmedMessage = 0;
		//void HandleSender();
		//void HandleReceiver_TCP();//old
		//void HandleReceiver_UDP();//UDP
		//void HandleReceiver_TCP2();//old
		//void session_TCP(asio::ip::tcp::socket sock);
		std::string MyUniqueIp;
		std::vector<std::string> confirmations;

		unsigned int stamp = 0;

		asio::io_service io_service_TCP;
		asio::ip::tcp::socket* sockptr_TCP;

		std::mutex confirmedMessageLock;

		bool serverAddname = false;

		void Receiver();
		void Sender();
		bool IsGiveIPType(std::string buffer);
		bool IsReceivedType(std::string buffer);
		bool IsConfirmationType(std::string buffer);
		bool IsGetIPType(std::string buffer);
		void SplitReceiveType(std::string* bufferStr, std::string* splitBuffer, std::size_t* bytes_transferred);
		void SplitConfirmType(std::string* bufferStr, std::string* splitBuffer, std::size_t* bytes_transferred);
		bool InConfirmations(std::string splitBuffer);
		void DeleteOldConfirmations(std::string splitBuffer);
		void SendGiveIPMessage(asio::ip::udp::endpoint sender);
		void SendConfirmMessage(asio::ip::udp::endpoint sender, std::string token, std::string data);
		std::string CreateGiveIpStr(std::string token);
		std::string CreateConfirmStr(std::string token);
		void RemoveMessageFromConfirmedMessage(std::string token);

		bool Registered(std::string ip);
		void Register(std::string ip);

		void ProcessEnd();

	public:
		NetworkClass(bool iam_server, std::string host, unsigned short port, bool IAmServer);
		~NetworkClass();
		//void StartSender();
		//void StopSender();
		void StartReceiver();
		void StopReceiver();
		void StartSender();
		void StopSender();
		void StartSendConfirm();
		void StopSendConfirm();

		void SendToServer(const std::string& message);
		void SendToClient(const std::string& message, std::string adress);

		void SendConfirm();

		//void SetMyIP();
		//void SendTo_TCP(char* data, int size);
		//void SendTo_UDP(char* data, int size);
		bool SendMessage_UDP(const std::string& message, const std::string& destination_ip, const unsigned short port);
		void SendConfirmedMessage_UDP(const std::string& message, const std::string& destination_ip, const unsigned short port);
		
		std::vector<std::string> registered;
	};

	void NetworkClass::ProcessEnd() {

		if (MyUniqueIp == "")
		{
			SendMessage_UDP("qqqq", clHost, clPort);
			singleThreadSleep(500);
			return;
		}

		//new
		if (!receiveServerAddName)
		{
			myNCB nullNCB;
			nullNCB.ncb_command_0 = 999;
			SendToServer(Pack_Message(MESS_CLIENT_SERVER_NAME_ADDED, nullNCB));
			singleThreadSleep(1000);
		}
		//new

		lastconnection_mt.lock();
		if (lastconnection_shared)
			if (clock() > oldtime() + networkTimeout())
			{
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("processEnd: WAITING FOR MESSAGE TIMEOUT:%x\n", lastconnection_shared->ncb_command_0);
#endif //TEST_NETWORK_MESSAGES			
				switch (lastconnection_shared->ncb_command_0)
				{
				case 0x35: {//CANCEL
					lastconnection_shared->ncb_retcode_1 = 0x0;
					strcpy(lastconnection_shared->ncb_name_26, "");
					strcpy(lastconnection_shared->ncb_callName_10, "");
					lastconnection_shared->ncb_cmd_cplt_49 = 0x0;
					lastconnection_shared = NULL;
#ifdef TEST_NETWORK_MESSAGES
					debug_net_printf("processEnd: lastconnection set to NULL CANCEL\n");
#endif //TEST_NETWORK_MESSAGES
					break;
				}
				case 0x7F: {//INIT
					lastconnection_shared->ncb_cmd_cplt_49 = 0;
					lastconnection_shared = NULL;
#ifdef TEST_NETWORK_MESSAGES
					debug_net_printf("processEnd: lastconnection set to NULL INIT\n");
#endif //TEST_NETWORK_MESSAGES
					break;
				}
				case 0x90: {//CALL(opposite listen)
								//connection->ncb_retcode_1= 0xb;
								//connection->ncb_cmd_cplt_49 = 0xb;
					/*if (netstate() == NETI_CALL)
					{
						lastconnection_shared->ncb_cmd_cplt_49 = 0xb;
					}*/
					lastconnection_shared = NULL;
#ifdef TEST_NETWORK_MESSAGES
					debug_net_printf("processEnd: lastconnection set to NULL CALL\n");
#endif //TEST_NETWORK_MESSAGES
					break;
				}
				case 0x91: {//LISTEN(opposite call)
					//if (netstate() != NETI_LISTEN_ACCEPT)
					//	lastconnection_shared->ncb_lsn_2 = 0x00;
					//lastconnection_shared->ncb_cmd_cplt_49 = 0;
					lastconnection_shared = NULL;
#ifdef TEST_NETWORK_MESSAGES
					debug_net_printf("processEnd: lastconnection set to NULL LISTEN\n");
#endif //TEST_NETWORK_MESSAGES
					break;
				}
				case 0x94: {//SEND(opposite receive)
					lastconnection_shared->ncb_cmd_cplt_49 = 0;
					lastconnection_shared = NULL;

#ifdef TEST_NETWORK_MESSAGES
					debug_net_printf("processEnd: lastconnection set to NULL SEND\n");
#endif //TEST_NETWORK_MESSAGES
					break;
				}
						 /*case 0x95: {//RECEIVE(opposite send)
							 if (GetRecSize() > 0) {
								 std::string tempstr = GetRecMess();
								 StringToBin(&lastconnection_shared->ncb_buffer_4.p, &lastconnection_shared->ncb_bufferLength_8, &tempstr);
								 lastconnection_shared->ncb_cmd_cplt_49 = 0x0;
								 lastconnection_shared = NULL;
			 #ifdef TEST_NETWORK_MESSAGES
								 debug_net_printf("lastconnection set to NULL RECEIVE\n");
			 #endif //TEST_NETWORK_MESSAGES
							 }
							 break;
						 }*/
				case 0xb0: {//ADD_NAME
					if (netstate() == NETI_ADD_NAME_REJECT)
					{
						lastconnection_shared->ncb_cmd_cplt_49 = 22;
					}
					if (netstate() == NETI_ADD_NAME_OK)
					{
						IHaveNameStrP = lastconnection_shared->ncb_name_26;
						lastconnection_shared->ncb_cmd_cplt_49 = 0;
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("processEnd: NETI_ADD_NAME_OK\n");
#endif //TEST_NETWORK_MESSAGES
					}
					else
					{
						lastconnection_shared->ncb_cmd_cplt_49 = 22;
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("processEnd: NETI_ADD_NAME_NOTOK\n");
#endif //TEST_NETWORK_MESSAGES
					}
					lastconnection_shared = NULL;
#ifdef TEST_NETWORK_MESSAGES
					debug_net_printf("processEnd: lastconnection set to NULL ADD_NAME\n");
#endif //TEST_NETWORK_MESSAGES
					break;
				}
				case 0xb1: {//DELETE_NAME 
					lastconnection_shared->ncb_cmd_cplt_49 = 0;
					lastconnection_shared = NULL;
#ifdef TEST_NETWORK_MESSAGES
					debug_net_printf("processEnd: lastconnection set to NULL INIT\n");
#endif //TEST_NETWORK_MESSAGES
					break;
				}
						 /*case 0x95: {//RECEIVE
							 if (GetRecSize() > 0) {
								 std::string tempstr = GetRecMess();
								 StringToBin(&lastconnection_shared->ncb_buffer_4.p, &lastconnection_shared->ncb_bufferLength_8, &tempstr);
								 lastconnection_shared->ncb_cmd_cplt_49 = 0x0;
								 lastconnection_shared = NULL;
			 #ifdef TEST_NETWORK_MESSAGES
								 debug_net_printf("lastconnection set to NULL RECEIVE\n");
			 #endif //TEST_NETWORK_MESSAGES
							 }
							 break;
						 }*/

				default: {
					lastconnection_shared->ncb_cmd_cplt_49 = 0;
				}
				}
				//lastconnection = NULL;
			}
		//else
		//{
			uint8_t	oldlastconnection_shared;
			if (lastconnection_shared)
				oldlastconnection_shared = lastconnection_shared->ncb_command_0;
		if (GetRecCount() > 0)
			if (lastconnection_shared)
				switch (lastconnection_shared->ncb_command_0)
				{
				case 0x95: {//RECEIVE(opposite send)//fix it!!!!!!!!!!!!!!!!!!!
					//std::string tempstr = GetRecMess();
					message_info unpacked_message = Unpack_Message(GetRecMess());
					if (unpacked_message.message != MESS_SERVER_SEND)
					{
						AddRecMess(Pack_Message(unpacked_message.message, unpacked_message.messNCB, unpacked_message.data, unpacked_message.size));
						break;
					}
					if (lastconnection_shared->ncb_bufferLength_8 != unpacked_message.size)
					{
						int a = 1;//this is code only for hunt errors
						int b = 0;
						int c = a / b;
					}
					lastconnection_shared->ncb_bufferLength_8 = unpacked_message.size;
					memcpy(lastconnection_shared->ncb_buffer_4.p, unpacked_message.data, unpacked_message.size);
					//StringToBin(&lastconnection_shared->ncb_buffer_4.p, &lastconnection_shared->ncb_bufferLength_8, &tempstr);
					lastconnection_shared->ncb_cmd_cplt_49 = 0x0;
#ifdef TEST_NETWORK_MESSAGES
					debug_net_printf("processEnd: CONVERT FROM MESSAGE:%d:%d\n", lastconnection_shared->ncb_bufferLength_8, 10/*strlen(tempstr.c_str())*/);
					debug_net_printf("processEnd: lastconnection set to NULL RECEIVE\n");
#endif //TEST_NETWORK_MESSAGES
					lastconnection_shared = NULL;
					break;
				}
				}
		//}
		lastconnection_mt.unlock();
	}

	NetworkClass::NetworkClass(bool iam_server, std::string host, unsigned short port, bool IAmServer) {
		//sender = NULL;
		clPort = port;
		clHost = host;
		clIam_server = iam_server;
		/*asio::io_context io_context;
		asio::ip::udp::endpoint receiver(asio::ip::udp::v4(), port);
		asio::ip::udp::socket socket(io_context, receiver);
		//thread t1(task1, "Hello");
		//t1.join();
		for (;;)
		{
			char buffer[65536];
			asio::ip::udp::endpoint sender;
			std::size_t bytes_transferred = socket.receive_from(asio::buffer(buffer), sender);
		}*/
		//StartSender();
		//StartReceiver();
		//SetMyIP();

		StartReceiver();
		StartSender();
		StartSendConfirm();

	};

	NetworkClass::~NetworkClass() {
		StopReceiver();
		StopSender();
		StopSendConfirm();
	};

	//namespace ip = sio::ip;
	/*void NetworkClass::SetMyIP() {
		asio::io_service ioService;
		asio::ip::tcp::resolver resolver(ioService);

		MyUniqueIp = resolver.resolve(asio::ip::host_name(), "")->endpoint().address().to_string();
	}*/

	std::string fixedLength(unsigned int uvalue, int digits) {
		std::string result;
		while (digits-- > 0) {
			result += ('0' + uvalue % 10);
			uvalue /= 10;
		}
		std::reverse(result.begin(), result.end());
		return result;
	}
	
	void NetworkClass::SendConfirmedMessage_UDP(const std::string& message, const std::string& destination_ip,
		const unsigned short port) {
#ifndef USE_CONFIRM_MESSAGES
		SendMessage_UDP(message, destination_ip, port);
#else
		std::string locStamp = MyUniqueIp + ";" + fixedLength(stamp, 10);
		stamp++;
		if (stamp > 1000000000) stamp = 0;
		SendMessage_UDP("xxxx" + locStamp + "|" + message, destination_ip, port);
		indexConfirmedMessage = 0;
		confirmedMessageLock.lock();
		while (confirmedMessage[indexConfirmedMessage])
		{
			indexConfirmedMessage++;
			if (indexConfirmedMessage == maxConfirmedMessage)
			{
				singleThreadSleep(20000);//Connection lost and many lost messages
				myprintf("\nConnection lost!!!!!");
				exit(500);
			}
		}
		confirmedMessage[indexConfirmedMessage] = new typeConfirmedMessage("xxxx" + locStamp + "|" + message, destination_ip, port);
		for (indexConfirmedMessage = 0; indexConfirmedMessage < maxConfirmedMessage; indexConfirmedMessage++)
		{
			if (confirmedMessage[indexConfirmedMessage])
			{
				std::chrono::seconds monoSecondsElapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - confirmedMessage[indexConfirmedMessage]->timestamp);
				if (monoSecondsElapsed > std::chrono::seconds(30))//Connection lost 30 second, but try continue with deleting messages
				{

					free(confirmedMessage[indexConfirmedMessage]);
					confirmedMessage[indexConfirmedMessage] = NULL;
				}
			}
		}	
		confirmedMessageLock.unlock();
#endif
	}

	bool NetworkClass::SendMessage_UDP(const std::string& message, const std::string& destination_ip,
		const unsigned short port) {
		if (MyUniqueIp == "")
			if(!IsGiveIPType(message) && !IsGetIPType(message))
				return false;

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

	void NetworkClass::StartReceiver() {
		HandleReceiverOn = true;
		receiver_thread = std::thread(&NetworkClass::Receiver, this);
	}

	void NetworkClass::StopReceiver() {
		HandleReceiverOn = false;
		SendMessage_UDP("Xend", "127.0.0.1", clPort);
		singleThreadSleep(500);
		receiver_thread.join();
	};

	void NetworkClass::StartSender() {
		HandleSenderOn = true;
		sender_thread = std::thread(&NetworkClass::Sender, this);
	}

	void NetworkClass::StopSender() {
		HandleSenderOn = false;
		singleThreadSleep(500);
		sender_thread.join();
	};

	void NetworkClass::StartSendConfirm() {
		HandleSendconfirmOn = true;
		sendComfirm_thread = std::thread(&NetworkClass::SendConfirm, this);
	}

	void NetworkClass::StopSendConfirm() {
		HandleSendconfirmOn = false;
		singleThreadSleep(500);
		sendComfirm_thread.join();
	};

	void NetworkClass::SendConfirm() {
		while (HandleSendconfirmOn) {
			confirmedMessageLock.lock();
			for (int i = 0; i < maxConfirmedMessage; i++)
			{
				if (confirmedMessage[i])
				{
					SendMessage_UDP(confirmedMessage[i]->cxMessage, confirmedMessage[i]->cxDestination_ip, confirmedMessage[i]->cxPort);
				}
			}
			confirmedMessageLock.unlock();
			singleThreadSleep(3);
		}
	}

	bool NetworkClass::IsGiveIPType(std::string buffer) {
		if (buffer.length() >= 4)
		{
			if (buffer[0] != 'i')return false;
			if (buffer[1] != 'i')return false;
			if (buffer[2] != 'i')return false;
			if (buffer[3] != 'i')return false;
			return true;
		}
		return false;
	}

	bool NetworkClass::IsGetIPType(std::string buffer) {
		if (buffer.length() >= 4)
		{
			if (buffer[0] != 'q')return false;
			if (buffer[1] != 'q')return false;
			if (buffer[2] != 'q')return false;
			if (buffer[3] != 'q')return false;
			return true;
		}
		return false;
	}

	bool NetworkClass::IsReceivedType(std::string buffer) {
		if (buffer.length() >= 4)
		{
			if (buffer[0] != 'x')return false;
			if (buffer[1] != 'x')return false;
			if (buffer[2] != 'x')return false;
			if (buffer[3] != 'x')return false;
			return true;
		}
		return false;
	}

	bool NetworkClass::IsConfirmationType(std::string buffer) {
		if (buffer.length() >= 4)
		{
			if (buffer[0] != 'y')return false;
			if (buffer[1] != 'y')return false;
			if (buffer[2] != 'y')return false;
			if (buffer[3] != 'y')return false;
			return true;
		}
		return false;
	}

	void NetworkClass::SplitReceiveType(std::string* bufferStr, std::string* splitBuffer, std::size_t* bytes_transferred) {
		std::string tempStr = *bufferStr;
		int tempPos = tempStr.find_first_of('|');
		*splitBuffer = tempStr.substr(4, tempPos-4);
		(*bytes_transferred) -= tempPos + 1;
		*bufferStr = tempStr.substr(tempPos + 1, (*bytes_transferred));
	}

	void NetworkClass::SplitConfirmType(std::string* bufferStr, std::string* splitBuffer, std::size_t* bytes_transferred) {
		std::string tempStr = *bufferStr;
		int tempPos = tempStr.find_first_of('|');
		*splitBuffer = tempStr.substr(4, tempPos-4);
		(*bytes_transferred) -= tempPos + 1;
		*bufferStr = tempStr.substr(tempPos + 1, (*bytes_transferred));
	}

	bool NetworkClass::InConfirmations(std::string splitBuffer) {
		for (std::string actobj : confirmations)
		{
			if (actobj == splitBuffer)return true;
		}
		return false;
	}

	void NetworkClass::DeleteOldConfirmations(std::string splitBuffer) {
		bool nextRun = true;
		while (nextRun)
		{
			nextRun = false;
			int i = 0;
			for (std::string actobj : confirmations)
			{
				int splitBufferDel = splitBuffer.find(';');
				std::string splitBufferToken1 = splitBuffer.substr(0, splitBufferDel);
				std::string splitBufferToken2 = splitBuffer.substr(splitBufferDel + 1, splitBuffer.length() - splitBufferDel - 1);
				int actobjDel = actobj.find(';');
				std::string actobjDelToken1 = actobj.substr(0, actobjDel);
				std::string actobjDelToken2 = actobj.substr(actobjDel + 1, actobj.length() - actobjDel - 1);
				if (splitBufferToken1 == actobjDelToken1)
				{
					if (stoi(splitBufferToken2) > stoi(actobjDelToken2) + 50)
					{
						confirmations.erase(confirmations.begin() + i);
						nextRun = true;
						continue;
					}
				}
				i++;
			}
		}
	}

	std::string NetworkClass::CreateConfirmStr(std::string token) {
		std::string output;
		for (std::string actobj : confirmations)
		{
			if (token == actobj)
				output += "yyyy" + token + "|";
		}
		return output;
	}

	void NetworkClass::RemoveMessageFromConfirmedMessage(std::string token) {
		for (int i = 0; i < maxConfirmedMessage; i++)
		{
			confirmedMessageLock.lock();
			if (confirmedMessage[i])
			{
				std::string mesg = confirmedMessage[i]->cxMessage;
				int splitBufferDel = mesg.find('|');
				std::string splitBufferToken1 = mesg.substr(4, splitBufferDel-4);
				//std::string splitBufferToken2 = mesg.substr(splitBufferDel + 1, mesg.length() - splitBufferDel - 1);
				if (splitBufferToken1 == token)
				{
					free(confirmedMessage[i]);
					confirmedMessage[i] = NULL;
				}
			}
			confirmedMessageLock.unlock();
		}
	}

	void NetworkClass::SendGiveIPMessage(asio::ip::udp::endpoint sender) {
		SendMessage_UDP("iiii|" + sender.address().to_string(), sender.address().to_string(), clPort);
	};

	void NetworkClass::SendConfirmMessage(asio::ip::udp::endpoint sender, std::string token, std::string data) {
		SendMessage_UDP(CreateConfirmStr(token) + data, sender.address().to_string(), clPort);
	};

	bool NetworkClass::Registered(std::string ip) {
		for (int i = 0; i < registered.size(); i++)
			if (registered[i] == ip)
				return true;
		return false;
	}

	void NetworkClass::Register(std::string ip) {
		registered.push_back(ip);
	}

	void NetworkClass::Sender() {
		std::string splitBuffer;
		while (HandleSenderOn)
		{
			//client
			ProcessEnd();
			//socket.send_to(asio::buffer(buffer, bytes_transferred), sender);
			//std::string message = "hellow";
			singleThreadSleep(3);
		}
	}

	void NetworkClass::Receiver() {
		asio::io_context io_context;
		//asio::ip::udp::endpoint receiver(asio::ip::udp::v4(), port);
		//asio::ip::udp::socket socket(io_context, receiver);
		asio::ip::udp adress_type = asio::ip::udp::v6();
		if (asio::ip::address::from_string(clHost).is_v4())
			asio::ip::udp adress_type = asio::ip::udp::v4();
		asio::ip::udp::socket socket(io_context, asio::ip::udp::endpoint(adress_type, clPort));
		while (HandleReceiverOn)
		{
			char buffer[65536];
			asio::ip::udp::endpoint sender;
			std::size_t bytes_transferred = socket.receive_from(asio::buffer(buffer), sender);
			std::string bufferStr;
			bufferStr.assign(buffer, bytes_transferred);
			std::string splitBuffer;
			bool handleMessage = true;

#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("Receive message: %s ip: %s port: %d\n", bufferStr.c_str(), sender.address().to_string().c_str(), clPort);
#endif //TEST_NETWORK_MESSAGES

			//std::string tempbufferStr= bufferStr;
			if (bufferStr == "Xend")continue;

			if (IsGetIPType(bufferStr))
			{
				SendGiveIPMessage(sender);
				continue;
			}

			if (IsReceivedType(bufferStr))
			{
				SplitReceiveType(&bufferStr, &splitBuffer, &bytes_transferred);
				if (InConfirmations(splitBuffer))
				{
					handleMessage = false;
					SendConfirmMessage(sender, splitBuffer, "");
				}
				else
				{
					confirmations.push_back(splitBuffer);
					DeleteOldConfirmations(splitBuffer);
				}
			}

			if (IsGiveIPType(bufferStr))
			{
				SplitConfirmType(&bufferStr, &splitBuffer, &bytes_transferred);
				MyUniqueIp = bufferStr;
				continue;
			}

			if (IsConfirmationType(bufferStr))
			{
				SplitConfirmType(&bufferStr, &splitBuffer, &bytes_transferred);
				RemoveMessageFromConfirmedMessage(splitBuffer);
				handleMessage = false;
			}

			if (handleMessage)
			{
				if (InConfirmations(splitBuffer))
				{
					SendConfirmMessage(sender, splitBuffer, "");
				}
				/*StringSize tempStringSize;
				tempStringSize.locString = bufferStr;
				tempStringSize.locSize = bytes_transferred;
				tempStringSize.locHandleMessage = handleMessage;
				tempStringSize.locSender = sender;
				ReceivedMessages.push(tempStringSize);*/

				//if (!ReceivedMessages.empty())
				{
					/*StringSize tempStringSize = ReceivedMessages.pop();
					std::string bufferStr = tempStringSize.locString;
					std::size_t bytes_transferred = tempStringSize.locSize;
					bool handleMessage = tempStringSize.locHandleMessage;
					asio::ip::udp::endpoint  sender = tempStringSize.locSender;*/

					if (clIam_server)
					{
#ifdef TEST_NETWORK_MESSAGES
						//debug_net_printf("Server Get Message:%s\n", messages[0].c_str());
#endif //TEST_NETWORK_MESSAGES

	//new
						message_info unpacked_message = Unpack_Message(bufferStr);

						//myprintf("Server: message - %s\n", MessageIndexToText(unpacked_message.message));
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("Server: message - %s\n", MessageIndexToText(unpacked_message.message));
#endif //TEST_NETWORK_MESSAGES

						if (unpacked_message.message == MESS_CLIENT_SERVER_NAME_ADDED)
						{
							if (!Registered(sender.address().to_string()))
							{
								Register(sender.address().to_string());
							}
							if (serverAddname)
							{
								myNCB nullNCB;
								nullNCB.ncb_command_0 = 999;
								SendToClient(Pack_Message(MESS_SERVER_SERVER_NAME_ADDED, nullNCB), sender.address().to_string());
#ifdef TEST_NETWORK_MESSAGES
								debug_net_printf("Client %s can AddName\n", sender.address().to_string());
#endif //TEST_NETWORK_MESSAGES
							}
						}

						else if (unpacked_message.message == MESS_CLIENT_TESTADDNAME) {
							// server->SendToAll(messages[0]+std::string(";")+messages[1]+std::string(";")+ std::to_string(receivedMessage.second));
							// //"MESSAGE_TESTADDNAME;NETH200        ;1"
							if ((!GetNameNetwork(unpacked_message.data).compare("")))
							{
								AddNetworkName(unpacked_message.data, sender.address().to_string());
								myNCB nullNCB;
								nullNCB.ncb_command_0 = 999;
								SendToClient(Pack_Message(MESS_SERVER_TESTADDNAME_OK, nullNCB), sender.address().to_string());
#ifdef TEST_NETWORK_MESSAGES
								debug_net_printf("Server: MESSAGE_TESTADDNAME OK:%s %d\n", unpacked_message.data, sender.address().to_string());
#endif //TEST_NETWORK_MESSAGES
								if (!strcmp(unpacked_message.data, "NETH210        "))//is Server
									serverAddname = true;
							}
							else
							{
								myNCB nullNCB;
								nullNCB.ncb_command_0 = 999;
								SendToClient(Pack_Message(MESS_SERVER_TESTADDNAME_REJECT, nullNCB), sender.address().to_string());
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
							debug_net_printf("Server: MESSAGE_LISTEN:%s ; %s %s\n", ((myNCB*)unpacked_message.data)->ncb_callName_10, ((myNCB*)unpacked_message.data)->ncb_name_26, sender.address().to_string().c_str());
#endif //TEST_NETWORK_MESSAGES
						}
						else if (unpacked_message.message == MESS_CLIENT_MESSAGE_CALL)
						{
							if (AddListenName2((myNCB*)unpacked_message.data))
							{
								SendToClient(Pack_Message(MESS_SERVER_CALL_ACCEPT, unpacked_message.messNCB), sender.address().to_string());
								SendToClient(Pack_Message(MESS_SERVER_LISTEN_ACCEPT, unpacked_message.messNCB, unpacked_message.data, unpacked_message.size), GetIdNetworkFromName(((myNCB*)unpacked_message.data)->ncb_callName_10));
								//"NETH200        "
#ifdef TEST_NETWORK_MESSAGES
								debug_net_printf("Server: NETI_LISTEN_CONNECTED:%s %s %s\n", ((myNCB*)unpacked_message.data)->ncb_callName_10, ((myNCB*)unpacked_message.data)->ncb_name_26, sender.address().to_string().c_str());
#endif //TEST_NETWORK_MESSAGES
							}
							else
							{
								SendToClient(Pack_Message(MESS_SERVER_CALL_REJECT, unpacked_message.messNCB), sender.address().to_string());
								SendToClient(Pack_Message(MESS_SERVER_LISTEN_REJECT, unpacked_message.messNCB), GetIdNetworkFromName(((myNCB*)unpacked_message.data)->ncb_name_26));
								//"NETH200        "
#ifdef TEST_NETWORK_MESSAGES
								debug_net_printf("Server: NETI_LISTEN_REJECT:%s %s %s\n", ((myNCB*)unpacked_message.data)->ncb_callName_10, ((myNCB*)unpacked_message.data)->ncb_name_26, sender.address().to_string().c_str());
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
							CleanMessages(&unpacked_message.messNCB);
							RemoveNetworkName(unpacked_message.data);
#ifdef TEST_NETWORK_MESSAGES
							debug_net_printf("Server: MESSAGE_DELETE:%s %s\n", unpacked_message.data, sender.address().to_string().c_str());
#endif //TEST_NETWORK_MESSAGES
						}
						else if (unpacked_message.message == MESS_CLIENT_SEND)
						{
							std::string otherid = GetOtherSide(sender.address().to_string());
							if (otherid != "x1000")
							{
								SendToClient(Pack_Message(MESS_SERVER_SEND, unpacked_message.messNCB, unpacked_message.data, unpacked_message.size), otherid);
								SendToClient(Pack_Message(MESS_SERVER_SEND_OK, unpacked_message.messNCB), sender.address().to_string());
							}
#ifdef TEST_NETWORK_MESSAGES
							debug_net_printf("Server: MESSAGE_SEND:%s %s %s\n", unpacked_message.data, otherid.c_str(), sender.address().to_string().c_str());
#endif //TEST_NETWORK_MESSAGES
						}
						//new
						//processEnd();
					}
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
						networkTimeout(500);
						resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("Client: NETI_ADD_NAME_OK:\n");
#endif //TEST_NETWORK_MESSAGES
					}
					else if (unpacked_message.message == MESS_SERVER_TESTADDNAME_REJECT)
					{
						netstate(NETI_ADD_NAME_REJECT);
						networkTimeout(500);
						resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("CLIENT NETI_ADD_NAME_REJECT:\n");
#endif //TEST_NETWORK_MESSAGES
					}
					else if (unpacked_message.message == MESS_SERVER_CALL_ACCEPT)
					{
						netstate(NETI_CALL_ACCEPT);

						lastconnection_mt.lock();
						//lastconnection_shared->ncb_lsn_2 = 20;

						myNCB* mypointer = lastconnection_shared;

						lastconnection_shared->ncb_retcode_1 = 0x00;
						lastconnection_shared->ncb_cmd_cplt_49 = 0x00;


						lastconnection_mt.unlock();

						networkTimeout(0);
						resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("CLIENT MESSAGE_CALL_ACCEPT: %p|%s|%s|\n", mypointer, mypointer->ncb_callName_10, mypointer->ncb_name_26);
#endif //TEST_NETWORK_MESSAGES
					}
					else if (unpacked_message.message == MESS_SERVER_CALL_REJECT)
					{
						netstate(NETI_CALL_REJECT);
						networkTimeout(0);
						resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("CLIENT MESSAGE_CALL_REJECT:\n");
#endif //TEST_NETWORK_MESSAGES
					}
					else if (unpacked_message.message == MESS_SERVER_LISTEN_ACCEPT)
					{
						//netstate(NETI_CALL_ACCEPT);

						//lastconnection_mt.lock();
						//lastconnection_shared->ncb_lsn_2 = 20;
						//lastconnection_mt.unlock();

						//networkTimeout(0);

						//netstate(NETI_LISTEN_ACCEPT);
						//networkTimeout(0);
						setListen((myNCB*)unpacked_message.data);
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("CLIENT MESSAGE_LISTEN_ACCEPT:\n");
#endif //TEST_NETWORK_MESSAGES
					}
					else if (unpacked_message.message == MESS_SERVER_LISTEN_REJECT)
					{
						//netstate(NETI_LISTEN_REJECT);
						//networkTimeout(0);
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("CLIENT MESSAGE_LISTEN_REJECT:\n");
#endif //TEST_NETWORK_MESSAGES
					}
					else if (unpacked_message.message == MESS_SERVER_SEND)
					{
						AddRecMess(Pack_Message(MESS_SERVER_SEND, unpacked_message.messNCB, unpacked_message.data, unpacked_message.size));
						networkTimeout(0);
						resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("CLIENT MESSAGE_SEND:\n");
#endif //TEST_NETWORK_MESSAGES
					}
					else if (unpacked_message.message == MESS_SERVER_SEND_OK)
					{
						networkTimeout(0);
						resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("CLIENT MESSAGE_SEND OK:\n");
#endif //TEST_NETWORK_MESSAGES
					}
				}
			}
			singleThreadSleep(3);
		}
	}

	void NetworkClass::SendToServer(const std::string& message) {
		SendConfirmedMessage_UDP(message, clHost, clPort);
		//		std::string clDestination_ip;
		//unsigned short clPort;
	};

	void NetworkClass::SendToClient(const std::string& message, std::string adress) {
		SendConfirmedMessage_UDP(message, adress, clPort);
		//		std::string clDestination_ip;
		//unsigned short clPort;
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

void CleanMessages(myNCB* locNCB) {
	std::string result;
	RecMess_mt.lock();
	for (auto it = recMessages.begin(); it != recMessages.end();)
	{
		message_info unpackedMess = Unpack_Message(*it);
		if (strncpy(unpackedMess.messNCB.ncb_name_26, locNCB->ncb_name_26,sizeof(locNCB->ncb_name_26)-1)) {
			it = recMessages.erase(it);
		}
		else {
			++it;
		}
	}
	RecMess_mt.unlock();
}

void setListenConnection(myNCB* connection) {
	clientConnection_mt.lock();
	bool result=false;
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

void deleteListenConnection(myNCB* connection) {
	clientConnection_mt.lock();
	for (int i = 0; i < clientConnection.size(); i++)
		if (clientConnection[i]==connection)
		{
			clientConnection.erase(clientConnection.begin() + i);
			break;
		}
	clientConnection_mt.unlock();
};

int lastnetworkname = 0;

int messTypeAddSize = 9 + 8 + 4 + 4 + 20;

bool NetworkGetInitInfoFromServer(char* serverIP) {
	return false;
};
char* NetworkListenForClients() {
	return NULL;
};



void AddName(myNCB* connection) {
	locNetworkClass->SendToServer(Pack_Message(MESS_CLIENT_TESTADDNAME, *connection, connection->ncb_name_26, sizeof(connection->ncb_name_26)));
};

void AddNameReceive(bool nameIsOk, uint32_t id)
{
	if (nameIsOk)
		;// AddNetworkName(std::string name, uint32_t id)
	else
		;//xx
}

void CancelNetwork(myNCB* connection) {
	locNetworkClass->SendToServer(Pack_Message(MESS_CLIENT_CANCEL, *connection, connection->ncb_name_26, sizeof(connection->ncb_name_26)));
	deleteListenConnection(connection);
}

void DeleteNetwork(myNCB* connection) {
	locNetworkClass->SendToServer(Pack_Message(MESS_CLIENT_DELETE, *connection, connection->ncb_name_26, sizeof(connection->ncb_name_26)));
}

void CallNetwork(myNCB* connection) {
	locNetworkClass->SendToServer(Pack_Message(MESS_CLIENT_MESSAGE_CALL, *connection, (char*)connection, sizeof(*connection)));// +connection->ncb_name_26 + std::string(";") + connection->ncb_callName_10);
};

void ListenNetwork(myNCB* connection) {
	locNetworkClass->SendToServer(Pack_Message(MESS_CLIENT_MESSAGE_LISTEN, *connection, (char*)connection, sizeof(*connection)));/*->ncb_callName_10 + ';' + connection->ncb_name_26*/
	setListenConnection(connection);
};

void SendNetwork(myNCB* connection) {
	locNetworkClass->SendToServer(Pack_Message(MESS_CLIENT_SEND, *connection, (char*)connection->ncb_buffer_4.p, connection->ncb_bufferLength_8));
#ifdef TEST_NETWORK_MESSAGES
	debug_net_printf("CONVERT TO MESSAGE:%d:%d\n", connection->ncb_bufferLength_8, connection->ncb_bufferLength_8);
#endif //TEST_NETWORK_MESSAGES
};

void ReceiveNetwork(myNCB* connection) {

};



void makeNetwork(myNCB* connection) {
	//v10x->esi = 0;
	switch (connection->ncb_command_0) {
	case 0x35: {//CANCEL
		networkTimeout(10000);
		resetTimeout();
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
		CancelNetwork(connection);

		break;
	}
	case 0x7f: {//? 
		networkTimeout(10000);
		resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("makeNetwork - SET NETWORK INIT\n");
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0x03;
		connection->ncb_cmd_cplt_49 = 0x03;
		break;
	}
	case 0x90: {//CALL
		networkTimeout(600000);
		resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("makeNetwork - SET NETWORK CALL %p|%s|%s|\n", connection,connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_lsn_2 = 0xe8;
		connection->ncb_cmd_cplt_49 = 0xff;
		CallNetwork(connection);
		netstate(NETI_CALL);
		break;
	}
	case 0x91: {//LISTEN
		networkTimeout(10000);
		resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("makeNetwork - SET NETWORK LISTEN %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_cmd_cplt_49 = 0xff;
		ListenNetwork(connection);
		netstate(NETI_LISTEN);
		break;
	}
	case 0x92: {//HANG UP
		;
	}
	case 0x94: {//SEND
		networkTimeout(10000);
		resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("makeNetwork - SET NETWORK SEND %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_cmd_cplt_49 = 0xff;

		SendNetwork(connection);
		break;
	}
	case 0x95: {//RECEIVE
		networkTimeout(1000000);
		resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("makeNetwork - SET NETWORK RECEIVE %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_cmd_cplt_49 = 0xff;

		ReceiveNetwork(connection);
		netstate(NETI_RECEIVE);

		break;
	}
	case 0xb0: {//ADD_NAME 
		networkTimeout(15000);
		resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("makeNetwork - SET NETWORK ADD_NAME %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_num_3 = lastnetworkname + 0x02;
		lastnetworkname++;
		connection->ncb_cmd_cplt_49 = 0xff;

		AddName(connection);


		netstate(NETI_ADD_NAME);

		break;
	}
	case 0xb1: {//DELETE_NAME 
		networkTimeout(10000);
		resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("makeNetwork - SET NETWORK DELETE_NAME %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		DeleteNetwork(connection);

		break;
	}
		 
	}
	//mySleep(5);
	lastconnection_mt.lock();
	lastconnection_shared = connection;
	lastconnection_mt.unlock();
	if(lastconnection_shared->ncb_command_0==0x95)
		while (lastconnection_shared);
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

void InitMyNetLib(bool iam_server, char* ip, int networkPort)
//void test_network(bool client = false)
{
	//std::string testPack = Pack_Message(0, "0011000101000000", 16);
	//std::string testPack = Pack_Message(0, "01", 2);
	//message_info unTestPack=Unpack_Message(testPack);

	locNetworkClass = new MyNetworkLib::NetworkClass(iam_server, ip, networkPort,true);
}

void EndMyNetLib() {
	singleThreadSleep(500);
	delete locNetworkClass;
}
