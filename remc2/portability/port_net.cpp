#define _CRT_SECURE_NO_WARNINGS
#include "port_net.h"

//
// sender.cpp
// ~~~~~~~~~~
//
// Copyright (c) 2003-2010 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <sstream>
#include <string>
//#include <boost/thread.hpp>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"

#define TEST_NETWORK_MESSAGES
#define TEST_NETWORK_FAKECOMM1

#ifdef TEST_NETWORK_MESSAGES
FILE* debug_net_output;
const char* debug_net_filename = "c:/prenos/remc2-dev/net_messages_log.txt";
std::string net_path = {};
bool debug_net_first = true;

char* IHaveNameStrP = NULL;
char IHaveNameStr[16];

bool listenerOn = true;

void debug_net_printf(const char* format, ...) {
	char prbuffer[1024];
	va_list arg;
	int done;
	va_start(arg, format);
	done = vsprintf(prbuffer, format, arg);
	va_end(arg);

	//std::string exepath = get_exe_path();
	//path = exepath + "/" + debug_net_filename;
	net_path = debug_net_filename;

	if (debug_net_first)
	{
		debug_net_output = fopen(net_path.c_str(), "wt");
		debug_net_first = false;
	}
	else
		debug_net_output = fopen(net_path.c_str(), "at");
	fprintf(debug_net_output, "%s", prbuffer);
	fclose(debug_net_output);
#ifdef DEBUG_PRINT_DEBUG_TO_SCREEN
	printf(prbuffer);
#endif
}
#endif// TEST_NETWORK_MESSAGES

//CreateMessage
#define MESSAGE_TESTADDNAME 1
#define MESSAGE_NAMEREJECT 2
#define MESSAGE_WINADDNAME 3
#define MESSAGE_MAKECONNECT 4
#define MESSAGE_SEND 5

//const short multicast_port = 30001;
const int max_message_count = 10;

int networkTimeout = 10000;

char compid[9];

class sender
{
public:
	sender(boost::asio::io_service& io_service,
		const boost::asio::ip::address& multicast_address)
		: endpoint_(multicast_address, MultiplayerPort),
		socket_(io_service, endpoint_.protocol()),
		timer_(io_service),
		message_count_(0)
	{
		std::ostringstream os;
		os << "Message " << message_count_++;
		message_ = os.str();

		socket_.async_send_to(
			boost::asio::buffer(message_), endpoint_,
			boost::bind(&sender::handle_send_to, this,
				boost::asio::placeholders::error));
	}

	void handle_send_to(const boost::system::error_code& error)
	{
		if (!error && message_count_ < max_message_count)
		{
			timer_.expires_from_now(boost::posix_time::seconds(1));
			timer_.async_wait(
				boost::bind(&sender::handle_timeout, this,
					boost::asio::placeholders::error));
		}
	}

	void handle_timeout(const boost::system::error_code& error)
	{
		if (!error)
		{
			std::ostringstream os;
			os << "Message " << message_count_++;
			message_ = os.str();

			socket_.async_send_to(
				boost::asio::buffer(message_), endpoint_,
				boost::bind(&sender::handle_send_to, this,
					boost::asio::placeholders::error));
		}
	}

private:
	boost::asio::ip::udp::endpoint endpoint_;
	boost::asio::ip::udp::socket socket_;
	boost::asio::deadline_timer timer_;
	int message_count_;
	std::string message_;
};

void NetworkTestServer()
{
	//try
	{
		using boost::asio::ip::tcp;
		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		tcp::resolver::query query(boost::asio::ip::host_name(), "");
		tcp::resolver::iterator it = resolver.resolve(query);
		std::string sClientIp;
		while (it != tcp::resolver::iterator())
		{
			boost::asio::ip::address addr = (it++)->endpoint().address();
			if (addr.is_v6())
			{
				std::cout << "ipv6 address: ";
			}
			else
			{
				std::cout << "ipv4 address: ";
				sClientIp = addr.to_string();
			}

			std::cout << addr.to_string() << std::endl;

		}
		//--------------------
		/*boost::asio::io_service my_io_service;
		boost::asio::ip::tcp::socket socket(my_io_service);

		boost::asio::ip::tcp::endpoint remote_ep = socket.remote_endpoint();
		boost::asio::ip::address remote_ad = remote_ep.address();
		std::string sClientIp2 = remote_ad.to_string();

		std::string sClientIp = socket.remote_endpoint().address().to_string();
		unsigned short uiClientPort = socket.remote_endpoint().port();*/


		/*if (argc != 2)
		{
			std::cerr << "Usage: sender <multicast_address>\n";
			std::cerr << "  For IPv4, try:\n";
			std::cerr << "    sender 239.255.0.1\n";
			std::cerr << "  For IPv6, try:\n";
			std::cerr << "    sender ff31::8000:1234\n";
			return 1;
		}*/
		//-------------------

		sClientIp[10] = '2';
		sClientIp[11] = '5';
		sClientIp[12] = '5';
		//-------------------------
		boost::system::error_code error;
		boost::asio::ip::udp::socket socket(io_service);
		int port = 3490;
		char* data = (char*)"test message";
		size_t request_length = strlen(data);


		socket.open(boost::asio::ip::udp::v4(), error);
		if (!error)
		{
			socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
			socket.set_option(boost::asio::socket_base::broadcast(true));

			boost::asio::ip::udp::endpoint senderEndpoint(boost::asio::ip::address_v4::broadcast(), port);



			socket.send_to(boost::asio::buffer(data, request_length), senderEndpoint);
			/*socket.close(error);*/
		}
		//-------------------------

		//boost::asio::io_service io_service;
		sender s(io_service, boost::asio::ip::address::from_string(sClientIp));
		io_service.run();
	}
	/*catch (std::exception & e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}*/

	//return 0;
}

int lastnetworkname = 0;
int lastnetworklisten = 0;

int messTypeAddSize = 9 + 8 + 4 + 4 + 20;

messType messageStr;

void BroadcastAll()
{
	{
		boost::system::error_code error;
		boost::asio::io_service io_service;
		boost::asio::ip::udp::socket socket(io_service);

		socket.open(boost::asio::ip::udp::v4(), error);
		if (!error)
		{
			socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
			socket.set_option(boost::asio::socket_base::broadcast(true));

			boost::asio::ip::udp::endpoint senderEndpoint(boost::asio::ip::address_v4::broadcast(), MultiplayerPort);

			//size_t request_length = strlen(message);
			socket.send_to(boost::asio::buffer((char*)&messageStr, messTypeAddSize + messageStr.lenght), senderEndpoint);
			socket.close(error);
		}
	}
}

void SendToIp(boost::asio::ip::address_v4 ip)
{
	{
		boost::system::error_code error;
		boost::asio::io_service io_service;
		boost::asio::ip::udp::socket socket(io_service);

		socket.open(boost::asio::ip::udp::v4(), error);
		if (!error)
		{
			socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
			socket.set_option(boost::asio::socket_base::broadcast(true));

			boost::asio::ip::udp::endpoint senderEndpoint(ip, MultiplayerPort);

			socket.send_to(boost::asio::buffer((char*)&messageStr, messTypeAddSize + messageStr.lenght), senderEndpoint);
			socket.close(error);
		}
	}
}

bool NetworkGetInitInfoFromServer(char* serverIP) {
	return false;
};
char* NetworkListenForClients() {
	return NULL;
};

const int MaxNames = 20;
char NetworkName[MaxNames][30];
char NetworkIp[MaxNames][30];
int lastindex = 0;

char* GetIpNetwork(char* ip) {
	for (int i = 0; i < lastindex; i++)
		if (strcmp(ip, NetworkIp[i]))
			return NetworkIp[i];
	return NULL;
}

char* GetNameNetwork(char* name) {
	for (int i = 0; i < lastindex; i++)
		if (strcmp(name, NetworkName[i]))
			return NetworkName[i];
	return NULL;
}

int GetIpNetworkIndex(char* ip) {
	for (int i = 0; i < lastindex; i++)
		if (strcmp(ip, NetworkIp[i]))
			return i;
	return -1;
}

int GetNameNetworkIndex(char* name) {
	for (int i = 0; i < lastindex; i++)
		if (strcmp(name, NetworkName[i]))
			return i;
	return -1;
}

void AddNetworkName(char* name, char* Ip) {
	if (lastindex >= MaxNames)return;
	if (!GetNameNetwork(name))
	{
		strcpy(NetworkName[lastindex], name);
		strcpy(NetworkIp[lastindex], Ip);
		lastindex++;
	}
#ifdef TEST_NETWORK_MESSAGES
	debug_net_printf("net name added:%s %s\n", name, Ip);
#endif //TEST_NETWORK_MESSAGES
}

void RemoveNetworkName(char* name) {
	int index = GetNameNetworkIndex(name);
	if (index == -1)return;
	for (int i = lastindex - 1; i > index; i--)
	{
		strcpy(NetworkName[i], NetworkName[i + 1]);
		strcpy(NetworkIp[i], NetworkIp[i + 1]);
	}
	lastindex--;
}


void CreateMessage(int type, uint8_t* mesg, int lenght) {
	memcpy(messageStr.stamp, "REMC2MESG", 9);
	memcpy(messageStr.compid, compid, 8);
	messageStr.type = type;
	messageStr.lenght = lenght;
	memcpy(messageStr.mesg, mesg, lenght);
#ifdef TEST_NETWORK_MESSAGES
	char showstr[81];
	memcpy(showstr, mesg, 40);
	showstr[80] = 0;
	debug_net_printf("SEND MESSAGE: %s\n", showstr);
#endif //TEST_NETWORK_MESSAGES
}

void CreateFakeMessage(int type, uint8_t* mesg, int lenght) {
	memcpy(messageStr.stamp, "REMC2MESG", 9);
	memcpy(messageStr.compid, "12345678", 8);
	messageStr.type = type;
	messageStr.lenght = lenght;
	memcpy(messageStr.mesg, mesg, lenght);
}

void AddName(myNCB* connection) {
	CreateMessage(MESSAGE_TESTADDNAME, (uint8_t*)connection->ncb_name_26, 1 + strlen(connection->ncb_name_26));
	/*sprintf(message, "REMC2MESG%s%s",compid, connection->ncb_name_26);
	for (int i = 9 + 8 + strlen(connection->ncb_name_26); i < 34; i++)
		message[i] = 0;*/
	BroadcastAll();
};

char connectionCompName[80] = "";

void makeConnection(char* newconnection) {
	strcpy(connectionCompName, newconnection);
};
void hangupConnection() {
	strcpy(connectionCompName, "");
};
char* getConnection() {
	if (strlen(connectionCompName) == 0)return NULL;
	return connectionCompName;
};

void CallNetwork(myNCB* connection) {
	CreateMessage(MESSAGE_MAKECONNECT, (uint8_t*)&connection, sizeof(myNCB));
	makeConnection(connection->ncb_callName_10);
	SendToIp(boost::asio::ip::make_address_v4(GetIpNetwork(connection->ncb_callName_10)));
};

void ListenNetwork(myNCB* connection) {
	//SendToIp(boost::asio::ip::make_address_v4(lastIp), messageStr);
};

void SendNetwork(myNCB* connection) {
	if (!getConnection())return;
	CreateMessage(MESSAGE_SEND, (uint8_t*)connection->ncb_buffer_4.p, connection->ncb_bufferLength_8);
	SendToIp(boost::asio::ip::make_address_v4(GetIpNetwork(getConnection())));
};

void ReceiveNetwork(myNCB* connection) {
};

std::thread* listenThread;
std::thread* guardThread;

std::mutex a;
std::mutex b;


const int MaxMessageCount = 20;
messType messages[MaxMessageCount];
//char ipMessage[MaxMessageCount][20];
//char* lastIp;

int MessageReadIndex = 0;
int MessageWriteIndex = 0;

void WriteMessage(messType* message, boost::asio::ip::address ip) {
	if (!(MessageWriteIndex % MaxMessageCount != (MessageReadIndex + 1) % MaxMessageCount))
	{
		MessageReadIndex = (MessageReadIndex + 1) % MaxMessageCount;//rewrite message
		printf("warning, message rewrited!!\n");
	}
	messages[MessageWriteIndex] = *message;
	strcpy(messages[MessageWriteIndex].ip, ip.to_string().c_str());
	MessageWriteIndex = (MessageWriteIndex + 1) % MaxMessageCount;
}
messType* ReadMessage() {
	if (MessageReadIndex == MessageWriteIndex)return NULL;
	int oldindex = MessageReadIndex;
	MessageReadIndex = (MessageReadIndex + 1) % MaxMessageCount;
	return &messages[oldindex];
}

char* REMC2MESG = (char*)"REMC2MESG";

class server
{
public:
	server(boost::asio::io_context& io_context, short port)
		: socket_(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
	{
		socket_.async_receive_from(
			boost::asio::buffer(data_, sizeof(messType)), sender_endpoint_,
			boost::bind(&server::handle_receive_from, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		//printf("fffff");
	}

	void handle_receive_from(const boost::system::error_code& error,
		size_t bytes_recvd)
	{
		if (!error && bytes_recvd > 0)
		{
			/*socket_.async_send_to(
				boost::asio::buffer(data_, bytes_recvd), sender_endpoint_,
				boost::bind(&server::handle_send_to, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
			printf("eeeee");*/
			//boost::asio::ip::udp::endpoint sendere=sender_endpoint_;
			//boost::asio::ip::address x=sendere.address();
			/*if (bytes_recvd >= 10) {
				bool same = true;
				for (int i = 0; i < 9; i++)
					if (REMC2MESG[i] != data_[i])same = false;
				if (same)
					WriteMessage((messType*)data_, sender_endpoint_.address());
			}*/
#ifndef TEST_NETWORK_FAKECOMM1			
		}
		else
		{
#endif //TEST_NETWORK_FAKECOMM1
			/*socket_.async_receive_from(
				boost::asio::buffer(data_, max_length), sender_endpoint_,
				boost::bind(&server::handle_receive_from, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
			printf("dddd");*/
			if (bytes_recvd >= 10) {
				bool same1 = true;
				for (int i = 0; i < 9; i++)
					if (REMC2MESG[i] != data_[i])same1 = false;				
				if (same1)
				{
					bool same2 = true;
					for (int i = 0; i < 9; i++)
						if (compid[i] != data_[i + 9])same2 = false;
					if (!same2)
					{
						messType* inMessage= (messType*)data_;
#ifdef TEST_NETWORK_MESSAGES
						debug_net_printf("inmessage:%x\n", inMessage->type);
#endif //TEST_NETWORK_MESSAGES
						switch (inMessage->type) {
						case MESSAGE_TESTADDNAME: {//RECV ADD_NAME
							if ((IHaveNameStrP == NULL) || (strcmp(IHaveNameStrP, (char*)inMessage->mesg)))
							{
								//connection->ncb_cmd_cplt_49 = 0;
								//NetworkEnd();
								return;
							}
							else//name is same
							{
								CreateMessage(MESSAGE_NAMEREJECT, (uint8_t*)"", 1 + strlen(""));
								SendToIp(boost::asio::ip::make_address_v4(sender_endpoint_.address().to_string()));
								return;
							}
							break;
						}
												/*
						case MESSAGE_NAMEREJECT: {//REJECT ADDNAME
							inrun = false;
							connection->ncb_cmd_cplt_49 = 22;
							//AddNetworkName(connection->ncb_name_26, "localhost");
							//NetworkEnd();
							return;
							break;
						}*/
						case MESSAGE_WINADDNAME: {
							AddNetworkName((char*)inMessage->mesg, inMessage->ip);
							//CreateMessage(MESSAGE_NAMEREJECT, (uint8_t*)"", 1 + strlen(""));
							//SendToIp(boost::asio::ip::make_address_v4(lastIp), messageStr);
							//AddNetworkName(connection->ncb_name_26, lastIp);
						}/*
						case MESSAGE_MAKECONNECT: {
							myNCB* tempNCB = (myNCB*)inMessage->mesg;
							if (!strcmp(connection->ncb_name_26, tempNCB->ncb_callName_10))
								makeConnection((char*)tempNCB->ncb_name_26);
						}
						case MESSAGE_SEND: {
							connection->ncb_buffer_4.p = inMessage->mesg;
							connection->ncb_bufferLength_8 = inMessage->lenght;
						}*/
						//else
						default:
						{
							WriteMessage((messType*)data_, sender_endpoint_.address());
							break;
						}
						}
					}
				}
			}

		}
	}
	/*
	void handle_send_to(const boost::system::error_code& error,
		size_t bytes_sent)
	{
		socket_.async_receive_from(
			boost::asio::buffer(data_, max_length), sender_endpoint_,
			boost::bind(&server::handle_receive_from, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
		printf("cccc");
		strcpy(lastmessage, "aaa");
	}*/


private:
	boost::asio::ip::udp::socket socket_;
	boost::asio::ip::udp::endpoint sender_endpoint_;
	char data_[sizeof(messType)];
};

void ListenService() {
	a.lock();
	boost::asio::io_context io_context;

	using namespace std; // For atoi.
	server s(io_context, MultiplayerPort);
	io_context.run();
	a.unlock();
}

void NetworkInit() {
	// Creation
	listenThread = new std::thread(ListenService);
};

void NetworkEnd() {
	// Cleanup
	//listenThread->interrupt();
	listenerOn = false;
	long locoldtime = clock();
	long locTimeout=3000;
	networkTimeout = 100;
	while (clock() > locoldtime + locTimeout);
	listenThread->join();
	delete listenThread;
}


void NetworkRestart() {
	if (a.try_lock())
	{
		a.unlock();
		// Cleanup
		listenThread->join();
		delete listenThread;
		// Creation
		listenThread = new std::thread(ListenService);
	}
}

void GuardService() {
	b.lock();
	NetworkInit();
	while (listenerOn) {
		NetworkRestart();
	}
	//std::cout << "Worker thread " << i << std::endl;
	b.unlock();
}

void NetworkInitG() {
	// Creation
	guardThread = new std::thread(GuardService);
};

void NetworkEndG() {
	// Cleanup
	//listenThread->interrupt();
	long locoldtime = clock();
	long locTimeout = 3000;
	networkTimeout = 100;
	while (clock() > locoldtime + locTimeout);
	guardThread->join();
	delete guardThread;
}


void NetworkRestartG() {
	if (a.try_lock())
	{
		a.unlock();
		// Cleanup
		guardThread->join();
		delete guardThread;
		// Creation
		guardThread = new std::thread(GuardService);
	}
}


//(int a1@<eax>, int a2, int a3, int a4, int a5)
void makeNetwork(int irg, REGS* v7x, REGS* v10x, SREGS* v12x, type_v2x* v2x, myNCB* connection) {
	//_int386x((_DWORD*)a4, a5, a3, a2);
	v10x->esi = 0;
	switch (connection->ncb_command_0) {
	case 0x7f: {//? 
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("SET NETWORK INIT\n");
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0x03;
		connection->ncb_cmd_cplt_49 = 0x03;
		long compidlong = (long)rand() + (long)rand();
		sprintf(compid, "%08X", compidlong);
		//NetworkInit();
		NetworkInitG();
		networkTimeout = 10000;
		break;
	}
	case 0xb0: {//ADD_NAME 
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("*SET NETWORK ADD_NAME %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_num_3 = lastnetworkname + 0x02;
		lastnetworkname++;
		connection->ncb_cmd_cplt_49 = 0xff;
		connection->ncb_reserved_50[4] = 0x7d;
		connection->ncb_reserved_50[5] = 0x27;
		connection->ncb_reserved_50[6] = 0x4b;
		connection->ncb_reserved_50[7] = 0x0e;
		connection->ncb_reserved_50[8] = 0x67;
		AddName(connection);
		networkTimeout = 10000;
		break;
	}
	case 0xb1: {//DELETE_NAME 
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("*SET NETWORK DELETE_NAME %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		/*connection->ncb_retcode_1 = 0xff;
		connection->ncb_num_3 = lastnetworkname + 0x02;
		lastnetworkname++;
		connection->ncb_cmd_cplt_49 = 0xff;
		connection->ncb_reserved_50[4] = 0x7d;
		connection->ncb_reserved_50[5] = 0x27;
		connection->ncb_reserved_50[6] = 0x4b;
		connection->ncb_reserved_50[7] = 0x0e;
		connection->ncb_reserved_50[8] = 0x67;
		AddName(connection);*/
		connection->ncb_retcode_1 = 0xff;
		networkTimeout = 100;
		break;
	}
	case 0x90: {//CALL
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("*SET NETWORK CALL %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_lsn_2 = 0xd8;
		connection->ncb_cmd_cplt_49 = 0xff;
		connection->ncb_reserved_50[4] = 0x7d;
		connection->ncb_reserved_50[5] = 0x27;
		connection->ncb_reserved_50[6] = 0x1f;
		connection->ncb_reserved_50[7] = 0x16;
		connection->ncb_reserved_50[8] = 0x68;
		CallNetwork(connection);
		networkTimeout = 100;
		break;
	}
	case 0x91: {//LISTEN
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("*SET NETWORK LISTEN %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_lsn_2 = lastnetworklisten + 0xe8;
		lastnetworklisten++;
		connection->ncb_cmd_cplt_49 = 0xff;
		connection->ncb_reserved_50[6] = 0x85;
		connection->ncb_reserved_50[7] = 0x17;
		connection->ncb_reserved_50[8] = 0x6a;
		ListenNetwork(connection);
		networkTimeout = 100;
		break;
	}
	case 0x94: {//SEND
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("*SET NETWORK SEND %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_cmd_cplt_49 = 0xff;
		connection->ncb_reserved_50[4] = 0x7d;
		connection->ncb_reserved_50[5] = 0x27;
		connection->ncb_reserved_50[6] = 0x8d;
		connection->ncb_reserved_50[7] = 0x1B;
		connection->ncb_reserved_50[8] = 0x6B;
		SendNetwork(connection);
		networkTimeout = 100;
		break;
	}
	case 0x95: {//RECEIVE
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("*SET NETWORK RECEIVE %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_lsn_2 = 0xd9;
		connection->ncb_cmd_cplt_49 = 0xff;
		connection->ncb_reserved_50[6] = 0xf3;
		connection->ncb_reserved_50[7] = 0x1B;
		connection->ncb_reserved_50[8] = 0x6c;
		ReceiveNetwork(connection);
		networkTimeout = 100;
		break;
	}
	case 0x35: {//CANCEL
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("*SET NETWORK CANCEL %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0x0b;
		connection->ncb_cmd_cplt_49 = 0x0b;
		connection->ncb_reserved_50[2] = 0x6f;
		connection->ncb_reserved_50[3] = 0x32;
		connection->ncb_reserved_50[6] = 0x85;
		connection->ncb_reserved_50[7] = 0x17;
		connection->ncb_reserved_50[8] = 0x6a;
		networkTimeout = 100;
		break;
	}			 
	}
}
/*
class receiver
{
public:
	receiver(boost::asio::io_context& io_context,
		const boost::asio::ip::address& listen_address,
		const boost::asio::ip::address& multicast_address)
		: socket_(io_context)
	{
		// Create the socket so that multiple may be bound to the same address.
		boost::asio::ip::udp::endpoint listen_endpoint(
			listen_address, MultiplayerPort);
		socket_.open(listen_endpoint.protocol());
		socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
		socket_.bind(listen_endpoint);

		// Join the multicast group.
		socket_.set_option(
			boost::asio::ip::multicast::join_group(multicast_address));

		do_receive();
	}

private:
	void do_receive()
	{
		socket_.async_receive_from(
			boost::asio::buffer(data_), sender_endpoint_,
			[this](boost::system::error_code ec, std::size_t length)
			{
				if (!ec)
				{
					std::cout.write(data_.data(), length);
					std::cout << std::endl;

					do_receive();
				}
			});
	}

	boost::asio::ip::udp::socket socket_;
	boost::asio::ip::udp::endpoint sender_endpoint_;
	std::array<char, 1024> data_;
};*/


#ifdef TEST_NETWORK_FAKECOMM1
int fake_index = 0;
void send_fakemess(int index) {
	switch (index) {
	case 1: {
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("send fake message:%d\n", index);
#endif //TEST_NETWORK_MESSAGES
		switch (fake_index) {
			case 0:
			{
				//memcpy(fake_data.ip, (char*)"198.198.198.198\0", 16);
				CreateFakeMessage(MESSAGE_WINADDNAME, (uint8_t*)"NETH200", 1 + strlen("NETH200"));
				/*sprintf(message, "REMC2MESG%s%s",compid, connection->ncb_name_26);
				for (int i = 9 + 8 + strlen(connection->ncb_name_26); i < 34; i++)
					message[i] = 0;*/
				BroadcastAll();
				//WriteMessage((messType*)&fake_data, boost::asio::ip::address::from_string("198.198.198.198"));
				break;
			}
			case 1:
			{
				//memcpy(fake_data.ip, (char*)"198.198.198.198\0", 16);
				CreateFakeMessage(MESSAGE_NAMEREJECT, (uint8_t*)"", 1 + strlen(""));
				BroadcastAll();
				//WriteMessage((messType*)&fake_data, boost::asio::ip::address::from_string("198.198.198.198"));
				break;
			}
		}
		break;
	}
	}
	fake_index++;
};
#endif//TEST_NETWORK_FAKECOMM1


bool inrun = false;
long oldtime;
void fake_network_interupt(myNCB* connection) {
	/*
	boost::asio::io_context io_context;
	using namespace std; // For atoi.
	server s(io_context, MultiplayerPort);
	io_context.run();
	*/
	messType* inMessage;
	//uint8_t outmessage[MaxMessageSize];
	if (inrun)
	{
		if (clock() > oldtime + networkTimeout)
		{
#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("WAITING FOR MESSAGE TIMEOUT:%x\n", connection->ncb_command_0);
#endif //TEST_NETWORK_MESSAGES
			inrun = false;
			switch (connection->ncb_command_0)
			{
				case 0xb0: {//ADD_NAME 
					connection->ncb_cmd_cplt_49 = 0;
					AddNetworkName(connection->ncb_name_26, (char*)"127.0.0.1");
					CreateMessage(MESSAGE_WINADDNAME, (uint8_t*)connection->ncb_name_26, 1 + strlen(connection->ncb_name_26));
					BroadcastAll();
					memcpy(IHaveNameStr, connection->ncb_name_26,16);
					IHaveNameStrP = IHaveNameStr;
					//AddNetworkName(connection->ncb_name_26,lastIp);
					//NetworkEnd();
				}
				default: {
					connection->ncb_cmd_cplt_49 = 0;
				}
			}			
			return;
		}
	}
	else
	{
		oldtime = clock();
		inrun = true;
		//NetworkInit();
	}
	inMessage = ReadMessage();
	#ifdef TEST_NETWORK_FAKECOMM1
	if (!inMessage) {
		send_fakemess(1);
		//inMessage = ReadMessage();
	}
	#endif//TEST_NETWORK_FAKECOMM1
	if (inMessage) {
		//NetworkRestart();

		/*char strtype[5];
		for (int i = 0; i < 4; i++)
			strtype[i] = message[i + 9+8];
		strtype[4] = 0;
		int type=atoi(strtype);*/
		/*for (int i = 0; i < 4; i++)
			strtype[i] = message[i + 9 + 8+4];
		strtype[4] = 0;
		int lenght = atoi(strtype);*/
		/*for (int i = 0; i < lenght; i++)
			outmessage[i], message[9 + 8 + 4 + 4 + i];*/
#ifdef TEST_NETWORK_MESSAGES
		char showstr[81];
		memcpy(showstr, inMessage, 40);
		showstr[80] = 0;
		debug_net_printf("RECEIVED MESSAGE:%s\n", showstr);
#endif //TEST_NETWORK_MESSAGES
		switch (inMessage->type) {			
		/*case MESSAGE_TESTADDNAME: {//RECV ADD_NAME
			inrun = false;
			if ((strcmp(connection->ncb_name_26, (char*)inMessage->mesg)) || !IHaveName)
			{
				//connection->ncb_cmd_cplt_49 = 0;
				//NetworkEnd();
				return;
			}
			else//name is same
			{
				connection->ncb_cmd_cplt_49 = 0;
				//NetworkEnd();
				CreateMessage(MESSAGE_NAMEREJECT, (uint8_t*)"", 1 + strlen(""));
				//SendToIp(boost::asio::ip::make_address_v4(lastIp),message);

				return;
			}
			break;
		}*/
		case MESSAGE_NAMEREJECT: {//REJECT ADDNAME
			inrun = false;
			connection->ncb_cmd_cplt_49 = 22;
			//AddNetworkName(connection->ncb_name_26, "localhost");
			//NetworkEnd();
			return;
			break;
		}
		/*case MESSAGE_WINADDNAME: {
			AddNetworkName((char*)inMessage->mesg, inMessage->ip);
			//CreateMessage(MESSAGE_NAMEREJECT, (uint8_t*)"", 1 + strlen(""));
			//SendToIp(boost::asio::ip::make_address_v4(lastIp), messageStr);
			//AddNetworkName(connection->ncb_name_26, lastIp);
		}*/
		case MESSAGE_MAKECONNECT: {
			myNCB* tempNCB = (myNCB*)inMessage->mesg;
			if (!strcmp(connection->ncb_name_26, tempNCB->ncb_callName_10))
				makeConnection((char*)tempNCB->ncb_name_26);
		}
		case MESSAGE_SEND: {
			connection->ncb_buffer_4.p = inMessage->mesg;
			connection->ncb_bufferLength_8 = inMessage->lenght;
		}
		}
	}
}