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

#define NETWORK_USETCP

char* IHaveNameStrP = NULL;
char IHaveNameStr[16];

bool listenerOn = true;
bool serverIPNotAdded = true;

int NetworkInitWait = 20;

#ifdef TEST_NETWORK_MESSAGES
FILE* debug_net_output;
const char* debug_net_filename1 = "net_messages_log.txt";
std::string debug_net_filename2 = {};

//std::string net_path = {};
bool debug_net_first = true;

void debug_net_printf(const char* format, ...) {
	char prbuffer[1024];
	va_list arg;
	int done;
	va_start(arg, format);
	done = vsprintf(prbuffer, format, arg);
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

#define IMESSAGE_SENDINFO 10
#define IMESSAGE_RECVINFO 11

//const short multicast_port = 30001;
const int max_message_count = 10;

int networkTimeout = 10000;

char compid[9];

bool useBroadcast = true;

int lastnetworkname = 0;
int lastnetworklisten = 0;

int messTypeAddSize = 9 + 8 + 4 + 4 + 20;

messType messageStr;

void BroadcastAllUDP()
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


#ifdef NETWORK_USETCP
void SendToIp(boost::asio::ip::address_v4 ip)
{
	boost::asio::io_context io_context;

	boost::asio::ip::tcp::resolver resolver(io_context);
	boost::asio::ip::tcp::socket s(io_context);

	boost::asio::ip::tcp::resolver::results_type endpoints =
		resolver.resolve(boost::asio::ip::tcp::v4(), ip.to_string(), std::to_string(MultiplayerPort));

	boost::asio::connect(s, endpoints);
	boost::asio::write(s, boost::asio::buffer((char*)&messageStr, messTypeAddSize + messageStr.lenght));
}
#else
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
#endif// NETWORK_USETCP
std::vector<std::string> gameIP;

void preBroadcastAll() {
#ifndef NETWORK_USETCP
	if (useBroadcast)
		BroadcastAllUDP();
	else
#endif// NETWORK_USETCP
		for (std::string locIP : gameIP) {
			SendToIp(boost::asio::ip::make_address_v4(locIP));
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
	preBroadcastAll();
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

bool isInIPList(std::string nextIP) {
	for (std::string locIP : gameIP) {
		if (locIP.compare(nextIP)==0)
			return true;
	}
	return false;
};

void AddtoIPList(std::string nextIP) {
	if (isInIPList(nextIP))
		return;
	gameIP.push_back(nextIP);
};

void processInMessages(char* data_, size_t bytes_recvd, boost::asio::ip::address remAdress)
{
	if (bytes_recvd >= 9 + 8) {
		bool same1 = true;
		for (int i = 0; i < 9; i++)
			if (REMC2MESG[i] != data_[i])same1 = false;
		if (same1)
		{
			bool same2 = true;
			for (int i = 0; i < 8; i++)
				if (compid[i] != data_[i + 9])same2 = false;
			if (!same2)
			{
				messType* inMessage = (messType*)data_;
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("inmessage:%x\n", inMessage->type);
#endif //TEST_NETWORK_MESSAGES
				switch (inMessage->type) {
				case IMESSAGE_SENDINFO: {
					AddtoIPList(remAdress.to_string());
					CreateMessage(IMESSAGE_RECVINFO, (uint8_t*)"", 1 + strlen(""));
					SendToIp(remAdress.to_v4());
					break;
				}
				case IMESSAGE_RECVINFO: {
					AddtoIPList(remAdress.to_string());
					serverIPNotAdded = false;
					break;
				}
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
						SendToIp(boost::asio::ip::make_address_v4(remAdress.to_string()));
						return;
					}
					break;
				}
				case MESSAGE_WINADDNAME: {
					AddNetworkName((char*)inMessage->mesg, (char*)remAdress.to_string().c_str());
					//CreateMessage(MESSAGE_NAMEREJECT, (uint8_t*)"", 1 + strlen(""));
					//SendToIp(boost::asio::ip::make_address_v4(lastIp), messageStr);
					//AddNetworkName(connection->ncb_name_26, lastIp);
				}
				default:
				{
					WriteMessage(inMessage, remAdress);
					break;
				}
				}
			}
		}
	}
}

#ifdef NETWORK_USETCP
class sessionTCP
{
public:
	sessionTCP(boost::asio::io_context& io_context)
		: socket_(io_context)
	{
	}

	boost::asio::ip::tcp::socket& socket()
	{
		return socket_;
	}

	void start()
	{
		socket_.async_read_some(boost::asio::buffer(data_, sizeof(messType)),
			boost::bind(&sessionTCP::handle_read, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}

private:
	void handle_read(const boost::system::error_code& error,
		size_t bytes_transferred)
	{
		if (!error)
		{
			//printf("%s\n", data_);
			processInMessages(data_, bytes_transferred, socket_.remote_endpoint().address());
		}
		else
		{
			delete this;
		}
	}

	boost::asio::ip::tcp::socket socket_;
	//boost::asio::ip::udp::endpoint sender_endpoint_;
	//enum { max_length = 1024 };
	//char data_[max_length];
	char data_[sizeof(messType)];
};

class serverTCP
{
public:
	serverTCP(boost::asio::io_context& io_context, short port)
		: io_context_(io_context),
		acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port))
	{
		start_accept();
	}

private:
	void start_accept()
	{
		sessionTCP* new_session = new sessionTCP(io_context_);
		acceptor_.async_accept(new_session->socket(),
			boost::bind(&serverTCP::handle_accept, this, new_session,
				boost::asio::placeholders::error));
	}

	void handle_accept(sessionTCP* new_session,
		const boost::system::error_code& error)
	{
		if (!error)
		{
			new_session->start();
		}
		else
		{
			delete new_session;
		}

		start_accept();
	}

	boost::asio::io_context& io_context_;
	boost::asio::ip::tcp::acceptor acceptor_;
};

#else
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
//#ifndef TEST_NETWORK_FAKECOMM1			
		}
		else
		{
//#endif //TEST_NETWORK_FAKECOMM1
			processInMessages(data_,bytes_recvd, sender_endpoint_.address());
		}
	}


private:
	boost::asio::ip::udp::socket socket_;
	boost::asio::ip::udp::endpoint sender_endpoint_;
	char data_[sizeof(messType)];
};

#endif// NETWORK_USETCP

void ListenService() {
	a.lock();
	boost::asio::io_context io_context;

	using namespace std; // For atoi.
#ifdef NETWORK_USETCP
	serverTCP s(io_context, MultiplayerPort);
#else
	server s(io_context, MultiplayerPort);
#endif// NETWORK_USETCP
	io_context.run();
	a.unlock();
}

void myDelay(long locTimeout) {
	long locoldtime = clock();
	while (clock() < locoldtime + locTimeout);
};

void NetworkInit() {
	// Creation
	listenThread = new std::thread(ListenService);
	if (strcmp(serverIP, "000.000.000.000"))
	{
		useBroadcast = false;
		myDelay(1000);
		//detect all REMC2 IPs
		int waitcount = 0;
		while ((waitcount< NetworkInitWait)&&(serverIPNotAdded)) {
			CreateMessage(IMESSAGE_SENDINFO, (uint8_t*)"", 1 + strlen(""));
			SendToIp(boost::asio::ip::make_address_v4(serverIP));
			myDelay(1000);
			waitcount++;
		}
	}
};

void NetworkEnd() {
	// Cleanup
	//listenThread->interrupt();
	listenerOn = false;
	networkTimeout = 100;
	myDelay(3000);
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
	networkTimeout = 100;
	myDelay(3000);
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
		long compidlong = (long)rand() + (long)rand()+ (long)rand() + (long)rand();
		sprintf(compid, "%08X", compidlong);
		//NetworkInit();
		NetworkInitG();
		networkTimeout = 10000;
		break;
	}
	case 0xb0: {//ADD_NAME 
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("\n*SET NETWORK ADD_NAME %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
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
		debug_net_printf("\n*SET NETWORK DELETE_NAME %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
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
		debug_net_printf("\n*SET NETWORK CALL %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_lsn_2 = 0xe8;
		connection->ncb_retcode_1 = 0x00;
		connection->ncb_cmd_cplt_49 = 0xff;
		connection->ncb_reserved_50[4] = 0x7d;
		connection->ncb_reserved_50[5] = 0x27;
		connection->ncb_reserved_50[6] = 0x1f;
		connection->ncb_reserved_50[7] = 0x17;
		connection->ncb_reserved_50[8] = 0x69;
		connection->ncb_reserved_50[10] = 0xff;
		/*connection->ncb_retcode_1 = 0x0;
		connection->ncb_lsn_2 = 0x0;
		connection->ncb_cmd_cplt_49 = 0x0;
		connection->ncb_reserved_50[7] = 0x4b;
		connection->ncb_reserved_50[8] = 0x0e;
		connection->ncb_reserved_50[9] = 0x66;*/
		CallNetwork(connection);
		networkTimeout = 10000;
		break;
	}
	case 0x91: {//LISTEN
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("\n*SET NETWORK LISTEN %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
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
		debug_net_printf("\n*SET NETWORK SEND %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
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
		debug_net_printf("\n*SET NETWORK RECEIVE %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
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
		debug_net_printf("\n*SET NETWORK CANCEL %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0x00;
		connection->ncb_cmd_cplt_49 = 0x00;
		connection->ncb_reserved_50[2] = 0x6f;
		connection->ncb_reserved_50[3] = 0x32;
		connection->ncb_reserved_50[6] = 0x85;
		connection->ncb_reserved_50[7] = 0x17;
		connection->ncb_reserved_50[8] = 0x6a;

		myNCB* secondcon = (myNCB*)connection->ncb_buffer_4.p;
		secondcon->ncb_retcode_1 = 0x0b;
		secondcon->ncb_cmd_cplt_49 = 0x0b;
		secondcon->ncb_reserved_50[2] = 0x6f;
		secondcon->ncb_reserved_50[3] = 0x32;
		secondcon->ncb_reserved_50[6] = 0x71;
		secondcon->ncb_reserved_50[7] = 0x17;
		secondcon->ncb_reserved_50[8] = 0x66;

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
		if (fake_index > 10)return;
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("send fake message:%d ", fake_index);
#endif //TEST_NETWORK_MESSAGES
		switch (fake_index) {
			case 0:
			{
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("%s\n", "MESSAGE_WINADDNAME NETH200");
#endif //TEST_NETWORK_MESSAGES
				//memcpy(fake_data.ip, (char*)"198.198.198.198\0", 16);
				CreateFakeMessage(MESSAGE_WINADDNAME, (uint8_t*)"NETH200", 1 + strlen("NETH200"));
				/*sprintf(message, "REMC2MESG%s%s",compid, connection->ncb_name_26);
				for (int i = 9 + 8 + strlen(connection->ncb_name_26); i < 34; i++)
					message[i] = 0;*/
				preBroadcastAll();
				//WriteMessage((messType*)&fake_data, boost::asio::ip::address::from_string("198.198.198.198"));
				break;
			}
			case 1:
			{
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("%s\n", "MESSAGE_NAMEREJECT");
#endif //TEST_NETWORK_MESSAGES
				//memcpy(fake_data.ip, (char*)"198.198.198.198\0", 16);
				CreateFakeMessage(MESSAGE_NAMEREJECT, (uint8_t*)"", 1 + strlen(""));
				preBroadcastAll();
				//WriteMessage((messType*)&fake_data, boost::asio::ip::address::from_string("198.198.198.198"));
				break;
			}
#ifdef TEST_NETWORK_MESSAGES
			default:
				debug_net_printf("\n");
#endif //TEST_NETWORK_MESSAGES
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
					AddNetworkName(connection->ncb_name_26, (char*)"127.0.0.1");
					CreateMessage(MESSAGE_WINADDNAME, (uint8_t*)connection->ncb_name_26, 1 + strlen(connection->ncb_name_26));
					preBroadcastAll();
					memcpy(IHaveNameStr, connection->ncb_name_26,16);
					IHaveNameStrP = IHaveNameStr;
					//AddNetworkName(connection->ncb_name_26,lastIp);
					//NetworkEnd();
					connection->ncb_cmd_cplt_49 = 0;
					break;
				}
				case 0x90: {//CALL
					//connection->ncb_retcode_1= 0xb;
					//connection->ncb_cmd_cplt_49 = 0xb;
					break;
				}
				case 0x35: {//CANCEL
					connection->ncb_retcode_1 = 0x0;
					strcpy(connection->ncb_name_26,"");
					strcpy(connection->ncb_callName_10, "");
					connection->ncb_cmd_cplt_49 = 0x0;
					break;
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