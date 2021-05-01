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
#include <boost/asio.hpp>
#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"

//const short multicast_port = 30001;
const int max_message_count = 10;

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

void BroadcastAll(char* message)
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

			size_t request_length = strlen(message);
			socket.send_to(boost::asio::buffer(message, request_length), senderEndpoint);
			socket.close(error);
		}
		/*using boost::asio::ip::tcp;
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
				std::size_t found = sClientIp.find('.');
				found = sClientIp.find('.', found+1);
				found = sClientIp.find('.', found+1);
				sClientIp = sClientIp.substr(0, found)+".255";

				//-------------------------
				boost::system::error_code error;
				boost::asio::ip::udp::socket socket(io_service);
				//char* data = (char*)"test message";
				size_t request_length = strlen(message);


				socket.open(boost::asio::ip::udp::v4(), error);
				if (!error)
				{
					socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
					socket.set_option(boost::asio::socket_base::broadcast(true));

					boost::asio::ip::udp::endpoint senderEndpoint(boost::asio::ip::address_v4::broadcast(), MultiplayerPort);



					socket.send_to(boost::asio::buffer(message, request_length), senderEndpoint);
					//socket.close(error);
				}
				//-------------------------

				//boost::asio::io_service io_service;
				sender s(io_service, boost::asio::ip::address::from_string(sClientIp));
				io_service.run();
			}
			


		}*/

		/*sClientIp = addr.to_string();
sClientIp[10] = '2';
sClientIp[11] = '5';
sClientIp[12] = '5';*/
//-------------------------
		/*
		sClientIp = "ff02::1";

		boost::system::error_code error;
		boost::asio::ip::udp::socket socket(io_service);
		int port = 3490;
		//char* data = (char*)"test message";
		size_t request_length = strlen(message);


		socket.open(boost::asio::ip::udp::v4(), error);
		if (!error)
		{
			socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
			socket.set_option(boost::asio::socket_base::broadcast(true));

			boost::asio::ip::udp::endpoint senderEndpoint(boost::asio::ip::address_v4::broadcast(), port);



			socket.send_to(boost::asio::buffer(message, request_length), senderEndpoint);
		}
		//-------------------------

		//boost::asio::io_service io_service;
		sender s(io_service, boost::asio::ip::address::from_string(sClientIp));
		io_service.run();

		//std::cout << addr.to_string() << std::endl;

		//ff02::1

		*/
	}
}

bool NetworkGetInitInfoFromServer(char* serverIP) {
	return false;
};
char* NetworkListenForClients() {
	return NULL;
};

int lastnetworkname = 0;
int lastnetworklisten = 0;

void AddName(myNCB* connection) {
	/*char message[26];
	sprintf_s(message,26,"REMC2MESG%s", connection->ncb_name_26);
	for (int i = 9 + strlen(connection->ncb_name_26); i < 26; i++)
		message[i] = 0;
	BroadcastAll(message);*/
};

//(int a1@<eax>, int a2, int a3, int a4, int a5)
void makeNetwork(int irg, REGS* v7x, REGS* v10x, SREGS* v12x, type_v2x* v2x, myNCB* connection) {
	//_int386x((_DWORD*)a4, a5, a3, a2);
	v10x->esi = 0;
	switch (connection->ncb_command_0) {
	case 0x7f: {//? 
		connection->ncb_retcode_1 = 0x03;
		connection->ncb_cmd_cplt_49 = 0x03;
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
	case 0x90: {//CALL
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_lsn_2 = 0xd8;
		connection->ncb_cmd_cplt_49 = 0xff;
		connection->ncb_reserved_50[4] = 0x7d;
		connection->ncb_reserved_50[5] = 0x27;
		connection->ncb_reserved_50[6] = 0x1f;
		connection->ncb_reserved_50[7] = 0x16;
		connection->ncb_reserved_50[8] = 0x68;
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
		break;
	}
	case 0x95: {//RECEIVE
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_lsn_2 = 0xd9;
		connection->ncb_cmd_cplt_49 = 0xff;
		connection->ncb_reserved_50[6] = 0xf3;
		connection->ncb_reserved_50[7] = 0x1B;
		connection->ncb_reserved_50[8] = 0x6c;
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
};

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
	}

	void handle_receive_from(const boost::system::error_code& error,
		size_t bytes_recvd)
	{
		if (!error && bytes_recvd > 0)
		{
			socket_.async_send_to(
				boost::asio::buffer(data_, bytes_recvd), sender_endpoint_,
				boost::bind(&server::handle_send_to, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}
		else
		{
			socket_.async_receive_from(
				boost::asio::buffer(data_, max_length), sender_endpoint_,
				boost::bind(&server::handle_receive_from, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
		}
	}

	void handle_send_to(const boost::system::error_code& /*error*/,
		size_t /*bytes_sent*/)
	{
		socket_.async_receive_from(
			boost::asio::buffer(data_, max_length), sender_endpoint_,
			boost::bind(&server::handle_receive_from, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred));
	}


private:
	boost::asio::ip::udp::socket socket_;
	boost::asio::ip::udp::endpoint sender_endpoint_;
	enum { max_length = 1024 };
	char data_[max_length];
};

void fake_network_interupt(myNCB* connection) {
	/*if (argc != 2)
	{
		std::cerr << "Usage: blocking_udp_echo_server <port>\n";
		return 1;
	}*/

	/*boost::asio::io_context io_context;

	server(io_context, MultiplayerPort);*/

	boost::asio::io_context io_context;

	using namespace std; // For atoi.
	server s(io_context, MultiplayerPort);
	//boost::asio::deadline_timer timer(io_context, boost::posix_time::seconds(5));

	//boost::asio::deadline_timer timer(io_context);
	//timer.expires_from_now(boost::posix_time::seconds(5));
	//timer.async_wait(std::bind(&s::timeout, this));

	//Performe all Async operations
	//ioService.run();
	//ioService.reset();
	//boost::asio::steady_timer t(io, boost::asio::chrono::seconds(5));
	io_context.run();

    /*boost::asio::io_service io_service;
    receiver r(io_service,
		boost::asio::ip::make_address("255.255.255.255"),
		boost::asio::ip::make_address("255.255.255.255"));
    io_service.run();
	*/
	/*
	enum { max_length = 1024 };
	boost::asio::io_context io_context;

	boost::asio::ip::udp::socket s(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0));

	boost::asio::ip::udp::resolver resolver(io_context);
	boost::asio::ip::udp::resolver::results_type endpoints =
		resolver.resolve(boost::asio::ip::udp::v4(), argv[1], argv[2]);

	using namespace std; // For strlen.
	std::cout << "Enter message: ";
	char request[max_length];
	std::cin.getline(request, max_length);
	size_t request_length = strlen(request);
	s.send_to(boost::asio::buffer(request, request_length), *endpoints.begin());

	char reply[max_length];
	boost::asio::ip::udp::endpoint sender_endpoint;
	size_t reply_length = s.receive_from(
		boost::asio::buffer(reply, max_length), sender_endpoint);
	std::cout << "Reply is: ";
	std::cout.write(reply, reply_length);
	std::cout << "\n";*/

	/*WSADATA wsaData;
	int portno;
	portno = 5001;
	const int BufLen = 1024;
	//-------------------------

	int address_family = AF_INET;
	int type = SOCK_DGRAM;
	int protocol = IPPROTO_UDP;
	SOCKET sock = socket(address_family, type, protocol);

	if (sock == INVALID_SOCKET)
	{
		printf("socket failed: %d", WSAGetLastError());
		return;
	}
	//---------------------
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return;// 1;
	}
	//--------------------------
	SOCKADDR_IN local_address;
	local_address.sin_family = AF_INET;
	local_address.sin_port = htons(9999);
	local_address.sin_addr.s_addr = INADDR_ANY;
	if (bind(sock, (SOCKADDR*)&local_address, sizeof(local_address)) == SOCKET_ERROR)
	{
		printf("bind failed: %d", WSAGetLastError());
		return;
	}
	//----------------------------
	char buffer[BufLen];
	int flags = 0;
	SOCKADDR_IN from;
	int from_size = sizeof(from);
	int bytes_received = recvfrom(sock, buffer, BufLen, flags, (SOCKADDR*)&from, &from_size);

	if (bytes_received == SOCKET_ERROR)
	{
		printf("recvfrom returned SOCKET_ERROR, WSAGetLastError() %d", WSAGetLastError());
	}
	else
	{
		buffer[bytes_received] = 0;
		printf("%d.%d.%d.%d:%d - %s",
			from.sin_addr.S_un.S_un_b.s_b1,
			from.sin_addr.S_un.S_un_b.s_b2,
			from.sin_addr.S_un.S_un_b.s_b3,
			from.sin_addr.S_un.S_un_b.s_b4,
			from.sin_port,
			buffer);
	}
	//---------------------------------
	SOCKADDR_IN server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(portno);
	server_address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	char message[BufLen];
	gets_s(message, BufLen);

	int flagsx = 0;
	if (sendto(sock, message, strlen(message), flagsx, (SOCKADDR*)&server_address, sizeof(server_address)) == SOCKET_ERROR)
	{
		printf("sendto failed: %d", WSAGetLastError());
		return;
	}
	//-------------------------------------------
	*/

	//NetworkTestServer();

	connection->ncb_cmd_cplt_49 = 0;
}
