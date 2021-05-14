#define _CRT_SECURE_NO_WARNINGS
#include "port_net.h"

#include <iostream>
#include <sstream>
#include <string>
//#include <boost/thread.hpp>
#include <thread>
#include <mutex>
#include <boost/asio.hpp>
#include "boost/bind.hpp"
#include "boost/date_time/posix_time/posix_time_types.hpp"

//#include "../networklib/Server.h"
//#include "../networklib/Client.h"
//#include "Factory.h"
//#include <memory>
//#include <thread>
//#include <vector>

#include <stdarg.h>     /* va_list, va_start, va_arg, va_end */

#include "../networklib/Constants.h"
//#include "../networklib/Statistics.h"

#include "../networklib/LockedQueue.h"
#include "../networklib/Log.h"

#include <array>
#include <map>
#include <thread>
#include <atomic>
//#include "IServer.h"

#include <array>
#include <thread>

using boost::asio::ip::udp;

typedef std::map<uint32_t, udp::endpoint> ClientList;
typedef ClientList::value_type LClient;

myNCB* lastconnection;

int netstate = -1;

#define ADD_NAME 0
#define ADD_NAME_REJECT 1
#define ADD_NAME_RECEIVE 2
#define ADD_NAME_RECEIVE_ACCEPT 3

namespace NetworkLib {
	class Client/* : public IClient*/ {
	public:
		Client(std::string host, unsigned short server_port, unsigned short local_port);
		/*virtual */~Client();

		void Send(const std::string& message)/* override*/;

		bool HasMessages()/* override*/;

		std::string PopMessage()/* override*/;

		//void BackMessage(std::string message);

	private:
		// Network send/receive stuff
		boost::asio::io_service io_service;
		udp::socket socket;
		udp::endpoint server_endpoint;
		udp::endpoint remote_endpoint;
		std::array<char, NetworkBufferSize> recv_buffer;
		std::thread service_thread;

		// Queues for messages
		LockedQueue<std::string> incomingMessages;

		void start_receive();
		void handle_receive(const std::error_code& error, std::size_t bytes_transferred);
		void run_service();

		Client(Client&); // block default copy constructor

		// Statistics
		//Statistics statistics;
	};

	typedef std::pair<std::string, uint32_t> ClientMessage;

	class Server/* : public IServer*/ {
	public:
		/*explicit */Server(unsigned short local_port);
		/*virtual */~Server();

		bool HasMessages()/* override*/;
		ClientMessage PopMessage()/* override*/;
		//void BackMessage(ClientMessage message);

		void SendToClient(const std::string& message, uint32_t clientID)/* override*/;
		void SendToAllExcept(const std::string& message, uint32_t clientID);
		void SendToAll(const std::string& message);

		size_t GetClientCount()/* override*/;
		uint32_t GetClientIdByIndex(size_t index)/* override*/;

		//const Statistics& GetStatistics() const { return statistics; };
		std::vector<std::function<void(uint32_t)>> clientDisconnectedHandlers;
	private:
		// Network send/receive stuff
		boost::asio::io_service io_service;
		udp::socket socket;
		udp::endpoint server_endpoint;
		udp::endpoint remote_endpoint;
		std::array<char, NetworkBufferSize> recv_buffer;
		std::thread service_thread;

		// Low-level network functions
		void start_receive();
		void handle_remote_error(const std::error_code error_code, const udp::endpoint remote_endpoint);
		void handle_receive(const std::error_code& error, std::size_t bytes_transferred);
		void handle_send(std::string /*message*/, const std::error_code& /*error*/, std::size_t /*bytes_transferred*/) {}
		void run_service();

		// Client management
		int32_t get_or_create_client_id(udp::endpoint endpoint);
		void on_client_disconnected(int32_t id);

		void send(const std::string& message, udp::endpoint target);

		// Incoming messages queue
		LockedQueue<ClientMessage> incomingMessages;

		// Clients of the server
		ClientList clients;
		std::atomic_int32_t nextClientID;

		Server(Server&); // block default copy constructor

		// Statistics
		//Statistics statistics;
	};

	Client::Client(std::string host, unsigned short server_port, unsigned short local_port) :
		socket(io_service, udp::endpoint(udp::v4(), local_port)),
		service_thread(&Client::run_service, this)
	{
		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), host, std::to_string(server_port));
		server_endpoint = *resolver.resolve(query);
		Client::Send("");
	}

	Client::~Client()
	{
		io_service.stop();
		service_thread.join();
	}

	void Client::start_receive()
	{
		socket.async_receive_from(boost::asio::buffer(recv_buffer), remote_endpoint,
			[this](std::error_code ec, std::size_t bytes_recvd) { this->handle_receive(ec, bytes_recvd); });
	}

	void Client::handle_receive(const std::error_code& error, std::size_t bytes_transferred)
	{
		if (!error)
		{
			std::string message(recv_buffer.data(), recv_buffer.data() + bytes_transferred);
			incomingMessages.push(message);
			//statistics.RegisterReceivedMessage(bytes_transferred);
		}
		else
		{
			Log::Error("Client::handle_receive:", error);
		}

		start_receive();
	}

	void Client::Send(const std::string& message)
	{
		socket.send_to(boost::asio::buffer(message), server_endpoint);
		//statistics.RegisterSentMessage(message.size());
	}

	bool Client::HasMessages()
	{
		return !incomingMessages.empty();
	}

	std::string Client::PopMessage()
	{
		if (incomingMessages.empty())
			throw std::logic_error("No messages to pop");
		return incomingMessages.pop();
	}

	/*void Client::BackMessage(std::string message) {
		incomingMessages.push(message);
	}*/

	void Client::run_service()
	{
		start_receive();
		while (!io_service.stopped()) {
			try {
				io_service.run();
			}
			catch (const std::exception& e) {
				Log::Warning("Client: network exception: ", e.what());
			}
			catch (...) {
				Log::Error("Unknown exception in client network thread");
			}
		}
	}

	Server::Server(unsigned short local_port) :
		socket(io_service, udp::endpoint(udp::v4(), local_port)),
		service_thread(&Server::run_service, this),
		nextClientID(0L)
	{
		Log::Info("Starting server on port ", local_port);
	};

	Server::~Server()
	{
		io_service.stop();
		service_thread.join();
	}

	void Server::start_receive()
	{
		socket.async_receive_from(boost::asio::buffer(recv_buffer), remote_endpoint,
			[this](std::error_code ec, std::size_t bytes_recvd) { this->handle_receive(ec, bytes_recvd); });
	}

	void Server::on_client_disconnected(int32_t id)
	{
		for (auto& handler : clientDisconnectedHandlers)
			if (handler)
				handler(id);
	}

	void Server::handle_remote_error(const std::error_code error_code, const udp::endpoint remote_endpoint)
	{
		bool found = false;
		int32_t id;
		for (const auto& client : clients)
			if (client.second == remote_endpoint) {
				found = true;
				id = client.first;
				break;
			}
		if (found == false)
			return;

		clients.erase(id);
		on_client_disconnected(id);
	}

	void Server::handle_receive(const std::error_code& error, std::size_t bytes_transferred)
	{
		if (!error)
		{
			try {
				auto message = ClientMessage(std::string(recv_buffer.data(), recv_buffer.data() + bytes_transferred), get_or_create_client_id(remote_endpoint));
				if (!message.first.empty())
					incomingMessages.push(message);
				//statistics.RegisterReceivedMessage(bytes_transferred);
			}
			catch (std::exception ex) {
				Log::Error("handle_receive: Error parsing incoming message:", ex.what());
			}
			catch (...) {
				Log::Error("handle_receive: Unknown error while parsing incoming message");
			}
		}
		else
		{
			Log::Error("handle_receive: error: ", error.message(), " while receiving from address ", remote_endpoint);
			handle_remote_error(error, remote_endpoint);
		}

		start_receive();
	}

	void Server::send(const std::string& message, udp::endpoint target_endpoint)
	{
		socket.send_to(boost::asio::buffer(message), target_endpoint);
		//statistics.RegisterSentMessage(message.size());
	}

	void Server::run_service()
	{
		start_receive();
		while (!io_service.stopped()) {
			try {
				io_service.run();
			}
			catch (const std::exception& e) {
				Log::Error("Server: Network exception: ", e.what());
			}
			catch (...) {
				Log::Error("Server: Network exception: unknown");
			}
		}
		Log::Debug("Server network thread stopped");
	};

	int32_t Server::get_or_create_client_id(udp::endpoint endpoint)
	{
		for (const auto& client : clients)
			if (client.second == endpoint)
				return client.first;

		auto id = ++nextClientID;
		clients.insert(LClient(id, endpoint));
		return id;
	};

	void Server::SendToClient(const std::string& message, uint32_t clientID)
	{
		try {
			send(message, clients.at(clientID));
		}
		catch (std::out_of_range) {
			Log::Error("__FUNCTION__: Unknown client ID ");
		}
	};

	void Server::SendToAllExcept(const std::string& message, uint32_t clientID)
	{
		for (auto client : clients)
			if (client.first != clientID)
				send(message, client.second);
	};

	void Server::SendToAll(const std::string& message)
	{
		for (auto client : clients)
			send(message, client.second);
	}

	size_t Server::GetClientCount()
	{
		return clients.size();
	}

	uint32_t Server::GetClientIdByIndex(size_t index)
	{
		auto it = clients.begin();
		for (int i = 0; i < index; i++)
			++it;
		return it->first;
	};

	ClientMessage Server::PopMessage() {
		return incomingMessages.pop();
	}

	/*void Server::BackMessage(ClientMessage message) {
		incomingMessages.push(message);
	}*/

	bool Server::HasMessages()
	{
		return !incomingMessages.empty();
	};
}

void mySleep(int sleepMs)
{
#ifdef _WIN32
	Sleep(sleepMs);
#else
	usleep(sleepMs * 1000);   // usleep takes sleep time in us (1 millionth of a second)
#endif
}

#define TEST_NETWORK_MESSAGES
//#define TEST_NETWORK_FAKECOMM1

//#define NETWORK_USETCP

std::string IHaveNameStrP = "";
char IHaveNameStr[16];

bool listenerServerOn = true;
bool listenerClientOn = true;
bool serverIPNotAdded = true;

//int NetworkInitWait = 20;

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

NetworkLib::Client* client;
NetworkLib::Server* server;
void InitLibNetClient(char* ip, int serverport, int clientport) {
	client = new NetworkLib::Client(ip, serverport, clientport);
	NetworkInitClient();
}

void InitLibNetServer(int serverport) {
	server = new NetworkLib::Server(serverport);
	NetworkInitServer();
}

//const int MaxNames = 20;
std::vector<std::string> NetworkName;
std::vector<uint32_t> clientID;
//int lastindex = 0;

std::vector<std::string> ListenName;
std::vector<uint32_t> clientListenID;
std::vector<std::string> ListenName2;
std::vector<uint32_t> clientListenID2;
//int lastindexListen = 0;

uint32_t GetIpNetwork(uint32_t id) {
	for (uint32_t locclientID : clientID)
		if (id == locclientID)
			return locclientID;
	return -1;
}

std::string GetNameNetwork(std::string name) {
	for (std::string locNetworkName : NetworkName)	
		if (name.compare(locNetworkName))
			return locNetworkName;
	return "";
}

int GetIpNetworkIndex(uint32_t id) {
	for (int i = 0; i < clientID.size(); i++)
		if (id==clientID[i])
			return i;
	return -1;
}

int GetNameNetworkIndex(std::string name) {
	for (int i = 0; i < NetworkName.size(); i++)
		if (name.compare(NetworkName[i]))
			return i;
	return -1;
}

void AddNetworkName(std::string name, uint32_t id) {
	if (!GetNameNetwork(name).compare(""))
	{
		NetworkName.push_back(name);
		clientID.push_back(id);
	}
#ifdef TEST_NETWORK_MESSAGES
	debug_net_printf("net name added:%s %d\n", name.c_str(), id);
#endif //TEST_NETWORK_MESSAGES
}

std::string GetListenNetwork(std::string name) {
	for (std::string locListenName : ListenName)
		if (name.compare(locListenName))
			return locListenName;
	return "";
}

int GetNameListenIndex(std::string name) {
	for (int i = 0; i < ListenName.size(); i++)
		if (name.compare(ListenName[i]))
			return i;
	return -1;
}

void AddListenName(std::string name, uint32_t id) {
	if (!GetListenNetwork(name).compare(""))
	{
		ListenName.push_back(name);
		clientListenID.push_back(id);
		ListenName2.push_back("");
		clientListenID2.push_back(999);
	}
#ifdef TEST_NETWORK_MESSAGES
	debug_net_printf("listen name added:%s %d\n", name.c_str(), id);
#endif //TEST_NETWORK_MESSAGES
}

void AddListenName2(std::string name, std::string name2, uint32_t id2){
	uint32_t id = GetNameListenIndex(name);
	ListenName2[id]= name2;
	clientListenID2[id] = id2;
#ifdef TEST_NETWORK_MESSAGES
	debug_net_printf("listen accepted:%s %d,%s %d\n", name.c_str(), id, name2.c_str(), id2);
#endif //TEST_NETWORK_MESSAGES
}
void RemoveNetworkName(std::string name) {
	int index = GetNameNetworkIndex(name);
	NetworkName.erase(NetworkName.begin()+ index);
	clientID.erase(clientID.begin() + index);
}

bool TestAddName(std::string name, uint32_t clientID) {
	if(GetNameNetwork(name).compare(""))
		;
	return false;
}

std::thread* listenThreadServer;
std::thread* listenThreadClient;
//std::thread* guardThread;

std::mutex aServer;
std::mutex aClient;
//std::mutex b;

//bool inrun = false;
long oldtime;
int networkTimeout = 10000;

void processEnd() {
	if(lastconnection)
		if (clock() > oldtime + networkTimeout)
		{
#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("WAITING FOR MESSAGE TIMEOUT:%x\n", lastconnection->ncb_command_0);
#endif //TEST_NETWORK_MESSAGES			
			switch (lastconnection->ncb_command_0)
			{
			case 0xb0: {//ADD_NAME
				if (netstate == ADD_NAME_REJECT)
				{
					lastconnection->ncb_cmd_cplt_49 = 22;
				}
				else
				{
					/*AddNetworkName(lastconnection->ncb_name_26, (char*)"127.0.0.1");
					CreateMessage(MESSAGE_WINADDNAME, (uint8_t*)lastconnection->ncb_name_26, 1 + strlen(lastconnection->ncb_name_26));
					preBroadcastAll();
					memcpy(IHaveNameStr, lastconnection->ncb_name_26, 16);
					IHaveNameStrP = IHaveNameStr;*/
					IHaveNameStrP = lastconnection->ncb_name_26;
					//AddNetworkName(connection->ncb_name_26,lastIp);
					//NetworkEnd();
					lastconnection->ncb_cmd_cplt_49 = 0;
					client->Send(std::string("MESSAGE_WINADDNAME;") + lastconnection->ncb_name_26);
				}
				break;
			}
			case 0x90: {//CALL
				//connection->ncb_retcode_1= 0xb;
				//connection->ncb_cmd_cplt_49 = 0xb;
				break;
			}
			case 0x35: {//CANCEL
				lastconnection->ncb_retcode_1 = 0x0;
				strcpy(lastconnection->ncb_name_26, "");
				strcpy(lastconnection->ncb_callName_10, "");
				lastconnection->ncb_cmd_cplt_49 = 0x0;
				break;
			}
			case 0x95: {//RECEIVE
				if (netstate == ADD_NAME_RECEIVE_ACCEPT)
					lastconnection->ncb_cmd_cplt_49 = 0x0;
				else
					lastconnection->ncb_cmd_cplt_49 = 0x0;
				break;
			}
					
			default: {
				lastconnection->ncb_cmd_cplt_49 = 0;
			}
			}
			lastconnection = NULL;
		}
	/*else
	{
		oldtime = clock();
		inrun = true;
		//NetworkInit();
	}*/
	/*
	inMessage = ReadMessage();
#ifdef TEST_NETWORK_FAKECOMM1
	if (!inMessage) {
		send_fakemess(1);
		//inMessage = ReadMessage();
	}
#endif//TEST_NETWORK_FAKECOMM1
	if (inMessage) {
		//NetworkRestart();


#ifdef TEST_NETWORK_MESSAGES
		char showstr[81];
		memcpy(showstr, inMessage, 40);
		showstr[80] = 0;
		debug_net_printf("RECEIVED MESSAGE:%s\n", showstr);
#endif //TEST_NETWORK_MESSAGES
		switch (inMessage->type) {
		case MESSAGE_NAMEREJECT: {//REJECT ADDNAME
			inrun = false;
			connection->ncb_cmd_cplt_49 = 22;
			//AddNetworkName(connection->ncb_name_26, "localhost");
			//NetworkEnd();
			return;
			break;
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
	*/
}

void ListenerServer() {
	aServer.lock();
	while (listenerServerOn)
	{
		if (server->HasMessages())
		{
			NetworkLib::ClientMessage receivedMessage = server->PopMessage();
			std::string receivedMessageStr = receivedMessage.first;
			std::string delimiter = ";";

			size_t pos = 0;
			std::string token;
			std::vector<std::string> messages;
			while ((pos = receivedMessageStr.find(delimiter)) != std::string::npos) {
				token = receivedMessageStr.substr(0, pos);
				messages.push_back(token);
				receivedMessageStr.erase(0, pos + delimiter.length());
			}
			messages.push_back(receivedMessageStr);

			if (!messages[0].compare("MESSAGE_TESTADDNAME"))
			{
				/*if (!TestAddName(messages[1], receivedMessage.second))
					server->SendToClient("MESSAGE_NAMEREJECT", receivedMessage.second);*/
				server->SendToAll(messages[0]+std::string(";")+messages[1]+std::string(";")+ std::to_string(receivedMessage.second));
				//"MESSAGE_TESTADDNAME;NETH200        ;1"
			}
			else if (!messages[0].compare("MESSAGE_NAMEREJECT"))
			{
				/*if (!TestAddName(messages[1], receivedMessage.second))
					server->SendToClient("MESSAGE_NAMEREJECT", receivedMessage.second);*/
				server->SendToClient(messages[0] + std::string(";") + messages[1], std::stoi(messages[2]));
				//"MESSAGE_NAMEREJECT;NETH200        "
			}
			else if (!messages[0].compare("MESSAGE_WINADDNAME"))
			{
				AddNetworkName(messages[1], receivedMessage.second);
				//"NETH200        "
			}
			else if (!messages[0].compare("MESSAGE_LISTEN"))
			{
				AddListenName(messages[1], receivedMessage.second);
				//"NETH200        "
			}
			else if (!messages[0].compare("MESSAGE_RECEIVE"))
			{
				AddListenName2(messages[1], messages[2], receivedMessage.second);
				server->SendToClient(std::string("MESSAGE_RECEIVE_ACCEPT;"), receivedMessage.second);
				//"NETH200        "
			}
			
			//processEnd();
		}
		mySleep(1);
	}
	aServer.unlock();
}

void ListenerClient() {
	aClient.lock();
	while (listenerClientOn)
	{
		if (client->HasMessages())
		{
			std::string receivedMessage = client->PopMessage();
			std::string receivedMessageStr = receivedMessage;
			std::string delimiter = ";";

			size_t pos = 0;
			std::string token;
			std::vector<std::string> messages;
			while ((pos = receivedMessageStr.find(delimiter)) != std::string::npos) {
				token = receivedMessageStr.substr(0, pos);
				messages.push_back(token);
				receivedMessageStr.erase(0, pos + delimiter.length());
			}
			messages.push_back(receivedMessageStr);

			if (!messages[0].compare("MESSAGE_TESTADDNAME"))
			{
				if ((IHaveNameStrP == "") || (IHaveNameStrP.compare(messages[1])))
				{
					//client->Send(std::string("MESSAGE_TESTADDNAME;") + std::string("testname"));
					//return;
					//client->Send(std::string("MESSAGE_NAMEREJECT;") + messages[1] + std::string(";") + messages[2]);					
				}
				else//name is same
				{
					client->Send(std::string("MESSAGE_NAMEREJECT;") + messages[1]+ std::string(";")+ messages[2]);
					//"MESSAGE_NAMEREJECT;NETH200        ;1"
				}
				/*if (!TestAddName(messages[1], receivedMessage.second))
					server->SendToClient("MESSAGE_NAMEREJECT", receivedMessage.second);*/
				//server->SendToAll(messages[0] + std::string(";") + messages[1] + std::string(";") + std::to_string(receivedMessage.second));
			}
			else if (!messages[0].compare("MESSAGE_NAMEREJECT"))
			{
				//AddNetworkName((char*)inMessage->mesg, (char*)remAdress.to_string().c_str());
				netstate = ADD_NAME_REJECT;
				//client->Send(std::string("MESSAGE_WINADDNAME;") + messages[1]);
				//"MESSAGE_WINADDNAME;NETH200        "
			}
			else if (!messages[0].compare("MESSAGE_RECEIVE_ACCEPT"))
			{
				netstate = ADD_NAME_RECEIVE_ACCEPT;
			}

		}
		mySleep(1);
		processEnd();
	}
	aClient.unlock();
}

void NetworkInitServer() {
	// Creation
	listenThreadServer = new std::thread(ListenerServer);
	//FakeTestsClient();
};

void NetworkInitClient() {
	// Creation
	listenThreadClient = new std::thread(ListenerClient);
};

void NetworkEndServer() {
	// Cleanup
	//listenThread->interrupt();
	listenerServerOn = false;
	mySleep(3000);
	listenThreadServer->join();
	delete listenThreadServer;
}

void NetworkEndClient() {
	// Cleanup
	//listenThread->interrupt();
	listenerClientOn = false;
	mySleep(3000);
	listenThreadClient->join();
	delete listenThreadClient;
}

void EndLibNetClient() {
	NetworkEndClient();
	mySleep(2000);
	client->~Client();
}

void EndLibNetServer() {
	NetworkEndServer();
	mySleep(2000);
	server->~Server();	
}

void FakeTestsClient() {
	mySleep(2000);
	client = new NetworkLib::Client("127.0.0.1", ServerMPort, ClientMPort);
	mySleep(2000);
	NetworkInitClient();
	mySleep(2000);
	client->Send(std::string("MESSAGE_TESTADDNAME;") + std::string("testname"));
};

void testlib1() {
	//TEST_METHOD(SendMessageFromClientToServerShouldProduceSameMessage)
	{
		//auto server = NetworkLib::CreateServer();

		auto client = new NetworkLib::Client("127.0.0.1", ServerMPort, ClientMPort);
		//return std::unique_ptr<IClient>(client);

		auto server = new NetworkLib::Server(ServerMPort);
		//return std::unique_ptr<IServer>(server);

		//auto client = CreateClient();

		// Send client->server
		client->Send("Test message");

		mySleep(2000);

		debug_net_printf("server->HasMessages%d\n", server->HasMessages());
		debug_net_printf("client->HasMessages%d\n", client->HasMessages());

		std::string receivedMessage = server->PopMessage().first;
		
		debug_net_printf("server->HasMessages%d\n", server->HasMessages());
		debug_net_printf("client->HasMessages%d\n", client->HasMessages());

		debug_net_printf("server send to all\n");
		server->SendToAll("for all");

		mySleep(2000);

		debug_net_printf("server->HasMessages%d\n", server->HasMessages());
		debug_net_printf("client->HasMessages%d\n", client->HasMessages());

	}
}


/*
//CreateMessage
#define MESSAGE_TESTADDNAME 1
#define MESSAGE_NAMEREJECT 2
#define MESSAGE_WINADDNAME 3
#define MESSAGE_MAKECONNECT 4
#define MESSAGE_SEND 5

#define IMESSAGE_SENDINFO 10
#define IMESSAGE_RECVINFO 11
*/
//const short multicast_port = 30001;
//const int max_message_count = 10;

//char compid[9];

//bool useBroadcast = true;

int lastnetworkname = 0;
int lastnetworklisten = 0;

int messTypeAddSize = 9 + 8 + 4 + 4 + 20;

messType messageStr;
/*
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

			boost::asio::ip::udp::endpoint senderEndpoint(boost::asio::ip::address_v4::broadcast(), ServerMPort);

			//size_t request_length = strlen(message);
			socket.send_to(boost::asio::buffer((char*)&messageStr, messTypeAddSize + messageStr.lenght), senderEndpoint);
			socket.close(error);
		}
	}
}
*/

#ifdef NETWORK_USETCP
/*
void SendToIp(boost::asio::ip::address_v4 ip)
{
	//try
	{
		boost::asio::io_context io_context;

		boost::asio::ip::tcp::resolver resolver(io_context);
		boost::asio::ip::tcp::socket s(io_context);

		boost::asio::ip::tcp::resolver::results_type endpoints =
			resolver.resolve(boost::asio::ip::tcp::v4(), ip.to_string(), std::to_string(ServerMPort));

		boost::asio::connect(s, endpoints);
		boost::asio::write(s, boost::asio::buffer((char*)&messageStr, messTypeAddSize + messageStr.lenght));
		boost::system::error_code ec;
		s.close(ec);
	}
	//catch (std::exception& e)
	//{
		//std::cerr << "Exception: " << e.what() << "\n";
	//}
}
*/

#else
/*
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
}*/
#endif// NETWORK_USETCP
std::vector<std::string> gameIP;
/*
void preBroadcastAll() {
#ifndef NETWORK_USETCP
	if (useBroadcast)
		BroadcastAllUDP();
	else
#endif// NETWORK_USETCP
		for (std::string locIP : gameIP) {
			SendToIp(boost::asio::ip::make_address_v4(locIP));
		}
		
}*/

bool NetworkGetInitInfoFromServer(char* serverIP) {
	return false;
};
char* NetworkListenForClients() {
	return NULL;
};



/*
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
}*/

void AddName(myNCB* connection) {
	client->Send(std::string("MESSAGE_TESTADDNAME;") + std::string(connection->ncb_name_26));
};

void AddNameReceive(bool nameIsOk, uint32_t id)
{
	if (nameIsOk)
		;// AddNetworkName(std::string name, uint32_t id)
	else
		;//xx
}

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
	/*CreateMessage(MESSAGE_MAKECONNECT, (uint8_t*)&connection, sizeof(myNCB));
	makeConnection(connection->ncb_callName_10);
	SendToIp(boost::asio::ip::make_address_v4(GetIpNetwork(connection->ncb_callName_10)));*/
};

void ListenNetwork(myNCB* connection) {
	//SendToIp(boost::asio::ip::make_address_v4(lastIp), messageStr);
	client->Send(std::string("MESSAGE_LISTEN;")+ connection->ncb_callName_10);
};

void SendNetwork(myNCB* connection) {
	/*if (!getConnection())return;
	CreateMessage(MESSAGE_SEND, (uint8_t*)connection->ncb_buffer_4.p, connection->ncb_bufferLength_8);
	SendToIp(boost::asio::ip::make_address_v4(GetIpNetwork(getConnection())));*/
};

void ReceiveNetwork(myNCB* connection) {
	client->Send(std::string("MESSAGE_RECEIVE;") + connection->ncb_callName_10 + std::string(";") + connection->ncb_name_26);
};


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
/*
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
					CreateMessage(IMESSAGE_RECVINFO, (uint8_t*)"IMESSAGE_RECVINFO", 1 + strlen("IMESSAGE_RECVINFO"));
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
*/
/*
void ListenService() {
	a.lock();
	boost::asio::io_context io_context;

	using namespace std; // For atoi.
#ifdef NETWORK_USETCP
	serverTCP s(io_context, ServerMPort);
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
*/
/*void NetworkInit() {
	// Creation
	listenThread = new std::thread(ListenService);
	if (strcmp(serverIP, "000.000.000.000"))
	{
		useBroadcast = false;
		myDelay(1000);
		//detect all REMC2 IPs
		int waitcount = 0;
		while ((waitcount< NetworkInitWait)&&(serverIPNotAdded)) {
			CreateMessage(IMESSAGE_SENDINFO, (uint8_t*)"IMESSAGE_SENDINFO", 1 + strlen("IMESSAGE_SENDINFO"));
			SendToIp(boost::asio::ip::make_address_v4(serverIP));
			myDelay(1000);
			waitcount++;
		}
	}
};*/
/*
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
*/
/*void GuardService() {
	b.lock();
	NetworkInit();
	while (listenerOn) {
		NetworkRestart();
	}
	//std::cout << "Worker thread " << i << std::endl;
	b.unlock();
}*/

/*void NetworkInitG() {
	// Creation
	guardThread = new std::thread(GuardService);
};*/
/*
void NetworkEndG() {
	// Cleanup
	//listenThread->interrupt();
	networkTimeout = 100;
	myDelay(3000);
	guardThread->join();
	delete guardThread;
}
*/

/*void NetworkRestartG() {
	if (a.try_lock())
	{
		a.unlock();
		// Cleanup
		guardThread->join();
		delete guardThread;
		// Creation
		guardThread = new std::thread(GuardService);
	}
}*/


//(int a1@<eax>, int a2, int a3, int a4, int a5)
void makeNetwork(int irg, REGS* v7x, REGS* v10x, SREGS* v12x, type_v2x* v2x, myNCB* connection) {
	lastconnection = connection;
	//_int386x((_DWORD*)a4, a5, a3, a2);
	v10x->esi = 0;
	switch (connection->ncb_command_0) {
	case 0x7f: {//? 
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("SET NETWORK INIT\n");
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0x03;
		connection->ncb_cmd_cplt_49 = 0x03;
		//long compidlong = (long)rand() + (long)rand()+ (long)rand() + (long)rand();
		//sprintf(compid, "%08X", compidlong);
		//NetworkInit();
		//FakeTests();
		//NetworkInitG();
		networkTimeout = 10000;
		oldtime = clock();
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
		oldtime = clock();

		netstate = ADD_NAME;

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
		oldtime = clock();
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
		oldtime = clock();
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
		oldtime = clock();
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
		oldtime = clock();
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
		oldtime = clock();
		netstate = ADD_NAME_RECEIVE;
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
		oldtime = clock();
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

/*
bool inrun = false;
long oldtime;

void fake_network_interupt(myNCB* connection) {
	
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


#ifdef TEST_NETWORK_MESSAGES
		char showstr[81];
		memcpy(showstr, inMessage, 40);
		showstr[80] = 0;
		debug_net_printf("RECEIVED MESSAGE:%s\n", showstr);
#endif //TEST_NETWORK_MESSAGES
		switch (inMessage->type) {			
		case MESSAGE_NAMEREJECT: {//REJECT ADDNAME
			inrun = false;
			connection->ncb_cmd_cplt_49 = 22;
			//AddNetworkName(connection->ncb_name_26, "localhost");
			//NetworkEnd();
			return;
			break;
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
*/
