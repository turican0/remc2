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

const short multicast_port = 30001;
const int max_message_count = 10;

class sender
{
public:
	sender(boost::asio::io_service& io_service,
		const boost::asio::ip::address& multicast_address)
		: endpoint_(multicast_address, multicast_port),
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

bool NetworkGetInitInfoFromServer(char* serverIP) {
	return false;
};
char* NetworkListenForClients() {
	return NULL;
};
