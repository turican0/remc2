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


//CreateMessage
#define MESSAGE_TESTADDNAME 1
#define MESSAGE_NAMEREJECT 2
#define MESSAGE_WINADDNAME 3
#define MESSAGE_MAKECONNECT 4
#define MESSAGE_SEND 5

//const short multicast_port = 30001;
const int max_message_count = 10;

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

const int MaxMessageSize = 20000;
//uint8_t message[MaxMessageSize];

#pragma pack (1)
typedef struct {
	uint8_t stamp[9];
	uint8_t compid[8];
	uint32_t type;
	uint32_t lenght;
	char ip[20];
	uint8_t mesg[MaxMessageSize];
}
messType;
#pragma pack (16)
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

std::mutex a;


const int MaxMessageCount = 20;
messType messages[MaxMessageCount];
//char ipMessage[MaxMessageCount][20];
//char* lastIp;

int MessageReadIndex = 0;
int MessageWriteIndex = 0;

void WriteMessage(messType* message, boost::asio::ip::address ip) {
	if (MessageWriteIndex % MaxMessageCount != (MessageReadIndex + 1) % MaxMessageCount)
	{
		messages[MessageWriteIndex] = *message;
		strcpy(messages[MessageWriteIndex].ip, ip.to_string().c_str());
		MessageWriteIndex = (MessageWriteIndex + 1) % MaxMessageCount;
	}
}
messType* ReadMessage() {
	if (MessageReadIndex == MessageWriteIndex)return NULL;
	int oldindex = MessageReadIndex;
	MessageReadIndex = (MessageReadIndex + 1) % MaxMessageCount;
	return &messages[oldindex];
}

char* REMC2MESG = "REMC2MESG";

class server
{
public:
	server(boost::asio::io_context& io_context, short port)
		: socket_(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port))
	{
		socket_.async_receive_from(
			boost::asio::buffer(data_, max_length), sender_endpoint_,
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
			if (bytes_recvd >= 10) {
				bool same = true;
				for (int i = 0; i < 9; i++)
					if (REMC2MESG[i] != data_[i])same = false;
				if (same)
					WriteMessage((messType*)data_, sender_endpoint_.address());
			}
		}
		else
		{
			/*socket_.async_receive_from(
				boost::asio::buffer(data_, max_length), sender_endpoint_,
				boost::bind(&server::handle_receive_from, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
			printf("dddd");*/
			if (bytes_recvd >= 10) {
				bool same = true;
				for (int i = 0; i < 9; i++)
					if (REMC2MESG[i] != data_[i])same = false;
				if (same)
					WriteMessage((messType*)data_, sender_endpoint_.address());
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
	enum { max_length = 1024 };
	char data_[max_length];
};

void ListenService() {
	a.lock();
	boost::asio::io_context io_context;

	using namespace std; // For atoi.
	server s(io_context, MultiplayerPort);
	//printf("bbbbbbbbbb");

	io_context.run();
	//std::cout << "Worker thread " << i << std::endl;
	a.unlock();
}

void NetworkInit() {
	// Creation
	listenThread = new std::thread(ListenService);
};

void NetworkEnd() {
	// Cleanup
	//listenThread->interrupt();
	listenThread->join();
	delete listenThread;
}


void NetworkRestart() {
	// Cleanup
	listenThread->join();
	delete listenThread;
	// Creation
	listenThread = new std::thread(ListenService);
}

//(int a1@<eax>, int a2, int a3, int a4, int a5)
void makeNetwork(int irg, REGS* v7x, REGS* v10x, SREGS* v12x, type_v2x* v2x, myNCB* connection) {
	//_int386x((_DWORD*)a4, a5, a3, a2);
	v10x->esi = 0;
	switch (connection->ncb_command_0) {
	case 0x7f: {//? 
		connection->ncb_retcode_1 = 0x03;
		connection->ncb_cmd_cplt_49 = 0x03;
		long compidlong = (long)rand() + (long)rand();
		sprintf(compid, "%08X", compidlong);
		NetworkInit();
		break;
	}
	case 0xb0: {//ADD_NAME 
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
		break;
	}
	case 0xb1: {//DELETE_NAME 
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
		break;
	}
	case 0x90: {//CALL
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_lsn_2 = 0xd8;
		connection->ncb_cmd_cplt_49 = 0xff;
		connection->ncb_reserved_50[4] = 0x7d;
		connection->ncb_reserved_50[5] = 0x27;
		connection->ncb_reserved_50[6] = 0x1f;
		connection->ncb_reserved_50[7] = 0x16;
		connection->ncb_reserved_50[8] = 0x68;
		CallNetwork(connection);
		break;
	}
	case 0x91: {//LISTEN
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_lsn_2 = lastnetworklisten + 0xe8;
		lastnetworklisten++;
		connection->ncb_cmd_cplt_49 = 0xff;
		connection->ncb_reserved_50[6] = 0x85;
		connection->ncb_reserved_50[7] = 0x17;
		connection->ncb_reserved_50[8] = 0x6a;
		ListenNetwork(connection);
		break;
	}
	case 0x94: {//SEND
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_cmd_cplt_49 = 0xff;
		connection->ncb_reserved_50[4] = 0x7d;
		connection->ncb_reserved_50[5] = 0x27;
		connection->ncb_reserved_50[6] = 0x8d;
		connection->ncb_reserved_50[7] = 0x1B;
		connection->ncb_reserved_50[8] = 0x6B;
		SendNetwork(connection);
		break;
	}
	case 0x95: {//RECEIVE
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_lsn_2 = 0xd9;
		connection->ncb_cmd_cplt_49 = 0xff;
		connection->ncb_reserved_50[6] = 0xf3;
		connection->ncb_reserved_50[7] = 0x1B;
		connection->ncb_reserved_50[8] = 0x6c;
		ReceiveNetwork(connection);
		break;
	}
	case 0x35: {//CANCEL
		connection->ncb_retcode_1 = 0x0b;
		connection->ncb_cmd_cplt_49 = 0x0b;
		connection->ncb_reserved_50[2] = 0x6f;
		connection->ncb_reserved_50[3] = 0x32;
		connection->ncb_reserved_50[6] = 0x85;
		connection->ncb_reserved_50[7] = 0x17;
		connection->ncb_reserved_50[8] = 0x6a;
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





int networkTimeout = 1000000;
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
			inrun = false;
			connection->ncb_cmd_cplt_49 = 0;
			AddNetworkName(connection->ncb_name_26, "localhost");
			CreateMessage(MESSAGE_WINADDNAME, (uint8_t*)connection->ncb_name_26, 1 + strlen(connection->ncb_name_26));
			BroadcastAll();
			//AddNetworkName(connection->ncb_name_26,lastIp);
			//NetworkEnd();
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
	if (inMessage) {
		NetworkRestart();

		bool same = true;
		for (int i = 0; i < 8; i++)
			if (compid[i] != inMessage->stamp[i])same = false;
		if (same)return;
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
		switch (inMessage->type) {
		case MESSAGE_TESTADDNAME: {//RECV ADD_NAME
			inrun = false;
			if (strcmp(connection->ncb_name_26, (char*)inMessage->mesg)) {
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
		}
		case MESSAGE_NAMEREJECT: {//REJECT ADDNAME
			inrun = false;
			connection->ncb_cmd_cplt_49 = 22;
			//AddNetworkName(connection->ncb_name_26, "localhost");
			//NetworkEnd();
			return;
			break;
		}
		case MESSAGE_WINADDNAME: {
			AddNetworkName((char*)inMessage->mesg, inMessage->ip);
			//CreateMessage(MESSAGE_NAMEREJECT, (uint8_t*)"", 1 + strlen(""));
			//SendToIp(boost::asio::ip::make_address_v4(lastIp), messageStr);
			//AddNetworkName(connection->ncb_name_26, lastIp);
		}
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