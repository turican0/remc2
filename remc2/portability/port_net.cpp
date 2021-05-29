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

myNCB* lastconnection_shared;

int netstate_shared = -1;

#define NETI_ADD_NAME 0
#define NETI_ADD_NAME_REJECT 1
#define NETI_ADD_NAME_OK 2
#define NETI_CALL 3
#define NETI_CALL_ACCEPT 4
#define NETI_CALL_REJECT 5
#define NETI_LISTEN 6
#define NETI_LISTEN_ACCEPT 7
#define NETI_LISTEN_REJECT 8
#define NETI_RECEIVE 9

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

std::mutex print_mt;

void debug_net_printf(const char* format, ...) {
	print_mt.lock();
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
	fprintf(debug_net_output, "%d|%s", clock(),prbuffer);
	fclose(debug_net_output);
#ifdef DEBUG_PRINT_DEBUG_TO_SCREEN
	printf(prbuffer);
#endif
	print_mt.unlock();
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

std::vector<myNCB*> clientConnection;

uint32_t GetIdNetworkFromName(std::string name) {
	for (int i = 0; i < NetworkName.size(); i++)
		if (!NetworkName[i].compare(name))
			return clientID[i];
	return 999;
}

std::string GetNameNetworkFromId(uint32_t id) {
	for (int i = 0; i < NetworkName.size(); i++)
		if (clientID[i]==id)
			return NetworkName[i];
	return "";
}

std::string GetNameNetwork(std::string name) {
	for (std::string locNetworkName : NetworkName)	
		if (!name.compare(locNetworkName))
			return locNetworkName;
	return "";
}

int GetIndexNetworkId(uint32_t id) {
	for (int i = 0; i < clientID.size(); i++)
		if (id==clientID[i])
			return i;
	return -1;
}

int GetNameNetworkIndex(std::string name) {
	for (int i = 0; i < NetworkName.size(); i++)
		if (!name.compare(NetworkName[i]))
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

uint32_t GetOtherSide(uint32_t id) {
	for (int i = 0; i < clientListenID.size(); i++)
		if (clientListenID[i]== id)
			return clientListenID2[i];
	return 1000;
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

void AddListenName(std::string name, std::string name2) {
	//if (GetNameNetwork(name).compare(""))
		if (!GetListenNetwork(name).compare(""))
		{
			ListenName.push_back(name);
			clientListenID.push_back(GetIdNetworkFromName(name));
			ListenName2.push_back(name2);
			clientListenID2.push_back(GetIdNetworkFromName(name2));
		#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("listen name added:%s %s\n", name.c_str(), name2.c_str());
		#endif //TEST_NETWORK_MESSAGES
		}
}

bool AddListenName2(std::string name, std::string name2){
	uint32_t id1 = GetIdNetworkFromName(name);
	if(id1==999)return false;
	uint32_t id2 = GetIdNetworkFromName(name2);
	if (id2 == 999)return false;
	//fix it
	int indexid = GetNameListenIndex(name);
	if(indexid==-1)return false;
	ListenName[indexid] = name;
	clientListenID[indexid] = id1;
	ListenName2[indexid]= name2;
	clientListenID2[indexid] = id2;
#ifdef TEST_NETWORK_MESSAGES
	debug_net_printf("listen accepted:%s %d,%s %d\n", name.c_str(), indexid, name2.c_str(), id2);
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

std::thread* listenThreadServer;
std::thread* listenThreadClient;
//std::thread* guardThread;

std::mutex aServer;
std::mutex aClient;
//std::mutex b;

//bool inrun = false;
long oldtime_shared;
int networkTimeout_shared = 10000;

std::mutex lastconnection_mt;
std::mutex netstate_mt;
std::mutex networkTimeout_mt;

int netstate()
{
	int result;
	netstate_mt.lock();
	result = netstate_shared;
	netstate_mt.unlock();
	return result;
}

void netstate(int input)
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
	debug_net_printf("change timeout %d\n", input);
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

void processEnd() {
	lastconnection_mt.lock();
	if(lastconnection_shared)
		if (clock() > oldtime() + networkTimeout())
		{
#ifdef TEST_NETWORK_MESSAGES
			debug_net_printf("WAITING FOR MESSAGE TIMEOUT:%x\n", lastconnection_shared->ncb_command_0);
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
				debug_net_printf("lastconnection set to NULL CANCEL\n");
#endif //TEST_NETWORK_MESSAGES
				break;
			}
			case 0x7F: {//INIT
				lastconnection_shared->ncb_cmd_cplt_49 = 0;
				lastconnection_shared = NULL;
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("lastconnection set to NULL INIT\n");
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
				debug_net_printf("lastconnection set to NULL CALL\n");
#endif //TEST_NETWORK_MESSAGES
				break;
			}
			case 0x91: {//LISTEN(opposite call)
				//if (netstate() != NETI_LISTEN_ACCEPT)
				//	lastconnection_shared->ncb_lsn_2 = 0x00;
				//lastconnection_shared->ncb_cmd_cplt_49 = 0;
				lastconnection_shared = NULL;
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("lastconnection set to NULL LISTEN\n");
#endif //TEST_NETWORK_MESSAGES
				break;
			}
			case 0x94: {//SEND(opposite receive)
				lastconnection_shared->ncb_cmd_cplt_49 = 0;
				lastconnection_shared = NULL;

#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("lastconnection set to NULL SEND\n");
#endif //TEST_NETWORK_MESSAGES
				break;
			}
			case 0x95: {//RECEIVE(opposite send)
				/*lastconnection_shared->ncb_cmd_cplt_49 = 0x0;
				lastconnection_shared = NULL;
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("lastconnection set to NULL RECEIVE,exit\n");
#endif //TEST_NETWORK_MESSAGES*/

				if (GetRecSize() > 0) {
					std::string tempstr = GetRecMess();

					/*lastconnection_shared->ncb_bufferLength_8 = 11;
					lastconnection_shared->ncb_buffer_4.p[0] = 1;
					lastconnection_shared->ncb_buffer_4.p[1] = 0;
					lastconnection_shared->ncb_buffer_4.p[2] = 2;
					BinToString(&lastconnection_shared->ncb_buffer_4.p, &lastconnection_shared->ncb_bufferLength_8, &tempstr);*/

					SendNetwork(lastconnection_shared);

					StringToBin(&lastconnection_shared->ncb_buffer_4.p, &lastconnection_shared->ncb_bufferLength_8, &tempstr);
					lastconnection_shared->ncb_cmd_cplt_49 = 0x0;
					lastconnection_shared = NULL;
#ifdef TEST_NETWORK_MESSAGES
					debug_net_printf("lastconnection set to NULL RECEIVE\n");
#endif //TEST_NETWORK_MESSAGES
				}
				//exit(0);
				break;
			}
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
					debug_net_printf("NETI_ADD_NAME_OK\n");
#endif //TEST_NETWORK_MESSAGES
				}
				else
				{
					lastconnection_shared->ncb_cmd_cplt_49 = 22;
#ifdef TEST_NETWORK_MESSAGES
					debug_net_printf("NETI_ADD_NAME_NOTOK\n");
#endif //TEST_NETWORK_MESSAGES
				}
				lastconnection_shared = NULL;
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("lastconnection set to NULL ADD_NAME\n");
#endif //TEST_NETWORK_MESSAGES
				break;
			}
			case 0xb1: {//DELETE_NAME 
				lastconnection_shared->ncb_cmd_cplt_49 = 0;
				lastconnection_shared = NULL;
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("lastconnection set to NULL INIT\n");
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
		/*else
		{
			switch (lastconnection_shared->ncb_command_0)
			{
				case 0x95://RECEIVE
				{
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
				}
			}
		}*/
	lastconnection_mt.unlock();
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

			if (!messages[0].compare("MESSAGE_CANCEL"))
			{
				if(messages[1].compare(""))
					RemoveListenName(messages[1]);
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("SERVER MESSAGE_CANCEL:%s %d\n", messages[1].c_str(), receivedMessage.second);
#endif //TEST_NETWORK_MESSAGES
			}
			else if (!messages[0].compare("MESSAGE_DELETE"))
			{
				if (messages[1].compare(""))
					RemoveNetworkName(messages[1]);
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("SERVER MESSAGE_DELETE:%s %d\n", messages[1].c_str(), receivedMessage.second);
#endif //TEST_NETWORK_MESSAGES
			}
			else if (!messages[0].compare("MESSAGE_TESTADDNAME"))
			{
				/*server->SendToAll(messages[0]+std::string(";")+messages[1]+std::string(";")+ std::to_string(receivedMessage.second));
				//"MESSAGE_TESTADDNAME;NETH200        ;1"*/
				if (!GetNameNetwork(messages[1]).compare(""))
				{
					AddNetworkName(messages[1], receivedMessage.second);
					server->SendToClient(std::string("MESSAGE_TESTADDNAME_OK;"), receivedMessage.second);
#ifdef TEST_NETWORK_MESSAGES
					debug_net_printf("SERVER MESSAGE_TESTADDNAME OK:%s %d\n", messages[1].c_str(), receivedMessage.second);
#endif //TEST_NETWORK_MESSAGES
				}
				else
				{
					server->SendToClient(std::string("MESSAGE_TESTADDNAME_REJECT;"), receivedMessage.second);
#ifdef TEST_NETWORK_MESSAGES
					debug_net_printf("SERVER MESSAGE_TESTADDNAME REJECT:%s %d\n", messages[1].c_str(), receivedMessage.second);
#endif //TEST_NETWORK_MESSAGES
				}
			}
			else if (!messages[0].compare("MESSAGE_LISTEN"))
			{
				AddListenName(messages[1], messages[2]);
				//"NETH200        "
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("SERVER MESSAGE_LISTEN:%s %d\n", messages[1].c_str(), receivedMessage.second);
#endif //TEST_NETWORK_MESSAGES
			}
			else if (!messages[0].compare("MESSAGE_SEND"))
			{
				uint32_t otherid= GetOtherSide(receivedMessage.second);
				if(otherid!=1000)
					server->SendToClient(messages[0] + std::string(";") + messages[1], otherid);
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("SERVER MESSAGE_SEND:%s %d %d\n", messages[1].c_str(), otherid, receivedMessage.second);
#endif //TEST_NETWORK_MESSAGES
			}
			/*else if (!messages[0].compare("MESSAGE_RECEIVE"))
			{

			}*/
			else if (!messages[0].compare("MESSAGE_CALL"))
			{
				if (AddListenName2(messages[1], messages[2]))
				{
					server->SendToClient(std::string("NETI_CALL_ACCEPT;"), receivedMessage.second);
					server->SendToClient(std::string("NETI_LISTEN_ACCEPT;"+ messages[1]), GetIdNetworkFromName(messages[2]));
					//"NETH200        "
	#ifdef TEST_NETWORK_MESSAGES
					debug_net_printf("SERVER NETI_LISTEN_CONNECTED:%s %s %d\n", messages[1].c_str(), messages[2].c_str(), receivedMessage.second);
	#endif //TEST_NETWORK_MESSAGES
				}
				else
				{
					server->SendToClient(std::string("NETI_CALL_REJECT;"), receivedMessage.second);
					server->SendToClient(std::string("NETI_LISTEN_REJECT;"), GetIdNetworkFromName(messages[2]));
				//"NETH200        "
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("SERVER NETI_LISTEN_REJECT:%s %s %d\n", messages[1].c_str(), messages[2].c_str(), receivedMessage.second);
#endif //TEST_NETWORK_MESSAGES
				}
			}
			
			//processEnd();
		}
		mySleep(1);
	}
	aServer.unlock();
}

std::vector<std::string> recMessages;
void AddRecMess(std::string message){
	recMessages.push_back(message);
};

unsigned int GetRecSize() {
	return recMessages.size();
};

std::string GetRecMess() {
	std::string result;
	result = recMessages.back();
	recMessages.pop_back();
	return result;
};

std::mutex clientConnection_mt;

bool setListen(std::string name) {
	clientConnection_mt.lock();
	bool result=false;
	for (int i = 0; i < clientConnection.size(); i++)
		if (name.compare(clientConnection[i]->ncb_name_26))
		{
			clientConnection[i]->ncb_lsn_2 = 20;
			clientConnection[i]->ncb_cmd_cplt_49 = 0;
			result = true;
			break;
		}
	clientConnection_mt.unlock();
	return result;
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

			/*
								server->SendToClient(std::string("MESSAGE_TESTADDNAME_OK;"), receivedMessage.second);
				}
				else
					server->SendToClient(std::string("MESSAGE_TESTADDNAME_REJECT;"), receivedMessage.second);
			*/
			if (!messages[0].compare("MESSAGE_TESTADDNAME_OK"))
			{
				netstate(NETI_ADD_NAME_OK);
				networkTimeout(500);
				resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("CLIENT NETI_ADD_NAME_OK:\n");
#endif //TEST_NETWORK_MESSAGES
			}
			else if (!messages[0].compare("MESSAGE_TESTADDNAME_REJECT"))
			{
				netstate(NETI_ADD_NAME_REJECT);
				networkTimeout(500);
				resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("CLIENT NETI_ADD_NAME_REJECT:\n");
#endif //TEST_NETWORK_MESSAGES
			}
			else if (!messages[0].compare("NETI_CALL_ACCEPT"))
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
			else if (!messages[0].compare("NETI_CALL_REJECT"))
			{
				netstate(NETI_CALL_REJECT);
				networkTimeout(0);
				resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("CLIENT MESSAGE_CALL_REJECT:\n");
#endif //TEST_NETWORK_MESSAGES
			}

			else if (!messages[0].compare("NETI_LISTEN_ACCEPT"))
			{
				//netstate(NETI_CALL_ACCEPT);

				//lastconnection_mt.lock();
				//lastconnection_shared->ncb_lsn_2 = 20;
				//lastconnection_mt.unlock();

				//networkTimeout(0);

				//netstate(NETI_LISTEN_ACCEPT);
				//networkTimeout(0);
				setListen(messages[1]);
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("CLIENT MESSAGE_LISTEN_ACCEPT:\n");
#endif //TEST_NETWORK_MESSAGES
			}
			else if (!messages[0].compare("NETI_LISTEN_REJECT"))
			{
				//netstate(NETI_LISTEN_REJECT);
				//networkTimeout(0);
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("CLIENT MESSAGE_LISTEN_REJECT:\n");
#endif //TEST_NETWORK_MESSAGES
			}

			else if (!messages[0].compare("MESSAGE_SEND"))
			{
				AddRecMess(messages[1]);
				networkTimeout(0);
				resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
				debug_net_printf("CLIENT MESSAGE_SEND:\n");
#endif //TEST_NETWORK_MESSAGES
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
//int lastnetworklisten = 0;

int messTypeAddSize = 9 + 8 + 4 + 4 + 20;

//messType messageStr;

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

void CancelNetwork(myNCB* connection) {
	client->Send(std::string("MESSAGE_CANCEL;") + connection->ncb_name_26);
	deleteListenConnection(connection);
}

void DeleteNetwork(myNCB* connection) {
	client->Send(std::string("MESSAGE_DELETE;") + connection->ncb_name_26);
}

void CallNetwork(myNCB* connection) {
	client->Send(std::string("MESSAGE_CALL;") + connection->ncb_name_26 + std::string(";") + connection->ncb_callName_10);
	/*CreateMessage(MESSAGE_MAKECONNECT, (uint8_t*)&connection, sizeof(myNCB));
	makeConnection(connection->ncb_callName_10);
	SendToIp(boost::asio::ip::make_address_v4(GetIpNetwork(connection->ncb_callName_10)));*/
};

void ListenNetwork(myNCB* connection) {
	//SendToIp(boost::asio::ip::make_address_v4(lastIp), messageStr);
	client->Send(std::string("MESSAGE_LISTEN;")+ connection->ncb_callName_10+ std::string(";") + connection->ncb_name_26);
	setListenConnection(connection);
};

void BinToString(uint8_t** buffer, uint16_t* lenght, std::string* str) {
	std::stringstream ss;
	for (int i = 0; i < *lenght; i++)
	{
		uint8_t locchar = (*buffer)[i];
		ss << (char)('A'+ locchar/16) << (char)('A' + locchar % 16);
	}
	ss << 'Z';
	*str=ss.str();
}

void StringToBin(uint8_t** buffer, uint16_t* lenght, std::string* str) {
	*lenght = 0;
	while(str->at(0 + (*lenght) * 2)!='Z')
	{
		(*buffer)[*lenght]= (str->at(0 + (*lenght) * 2)-'A')*16 + (str->at(1 + (*lenght) * 2)-'A');
		(*lenght)++;
	}
}

void SendNetwork(myNCB* connection) {
	std::string tempstr;
	BinToString(&connection->ncb_buffer_4.p, &connection->ncb_bufferLength_8, &tempstr);
	client->Send(std::string("MESSAGE_SEND;") + tempstr);
	/*if (!getConnection())return;
	CreateMessage(MESSAGE_SEND, (uint8_t*)connection->ncb_buffer_4.p, connection->ncb_bufferLength_8);
	SendToIp(boost::asio::ip::make_address_v4(GetIpNetwork(getConnection())));*/
};

void ReceiveNetwork(myNCB* connection) {

};



void makeNetwork(int irg, REGS* v7x, REGS* v10x, SREGS* v12x, type_v2x* v2x, myNCB* connection) {
	//_int386x((_DWORD*)a4, a5, a3, a2);
	v10x->esi = 0;
	switch (connection->ncb_command_0) {
	case 0x35: {//CANCEL
		networkTimeout(10000);
		resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("\n*SET NETWORK CANCEL %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
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
		debug_net_printf("SET NETWORK INIT\n");
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0x03;
		connection->ncb_cmd_cplt_49 = 0x03;
		//long compidlong = (long)rand() + (long)rand()+ (long)rand() + (long)rand();
		//sprintf(compid, "%08X", compidlong);
		//NetworkInit();
		//FakeTests();
		//NetworkInitG();
		break;
	}
	case 0x90: {//CALL
		networkTimeout(10000);
		resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("\n*SET NETWORK CALL %p|%s|%s|\n", connection,connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_lsn_2 = 0xe8;
		connection->ncb_cmd_cplt_49 = 0xff;

		/*connection->ncb_retcode_1 = 0x0;
		connection->ncb_lsn_2 = 0x0;
		connection->ncb_cmd_cplt_49 = 0x0;
		connection->ncb_reserved_50[7] = 0x4b;
		connection->ncb_reserved_50[8] = 0x0e;
		connection->ncb_reserved_50[9] = 0x66;*/
		CallNetwork(connection);
		netstate(NETI_CALL);
		break;
	}
	case 0x91: {//LISTEN
		networkTimeout(10000);
		resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("\n*SET NETWORK LISTEN %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		//connection->ncb_lsn_2 = 0x01;
		//lastnetworklisten++;
		connection->ncb_cmd_cplt_49 = 0xff;
		ListenNetwork(connection);
		netstate(NETI_LISTEN);
		break;
	}
	case 0x94: {//SEND
		networkTimeout(10000);
		resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("\n*SET NETWORK SEND %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		connection->ncb_cmd_cplt_49 = 0xff;

		SendNetwork(connection);
		break;
	}
	case 0x95: {//RECEIVE
		networkTimeout(100000);
		resetTimeout();
#ifdef TEST_NETWORK_MESSAGES
		debug_net_printf("\n*SET NETWORK RECEIVE %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
#endif //TEST_NETWORK_MESSAGES
		connection->ncb_retcode_1 = 0xff;
		//connection->ncb_lsn_2 = 0xd9;
		connection->ncb_cmd_cplt_49 = 0xff;

		ReceiveNetwork(connection);
		netstate(NETI_RECEIVE);

		break;
	}
	case 0xb0: {//ADD_NAME 
#ifdef TEST_NETWORK_MESSAGES
		networkTimeout(15000);
		resetTimeout();

		debug_net_printf("\n*SET NETWORK ADD_NAME %s %s\n", connection->ncb_name_26, connection->ncb_callName_10);
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
		DeleteNetwork(connection);

		break;
	}
		 
	}
	//mySleep(5);
	lastconnection_mt.lock();
	lastconnection_shared = connection;
	lastconnection_mt.unlock();
#ifdef TEST_NETWORK_MESSAGES
	debug_net_printf("SET CONNECTION\n");
#endif //TEST_NETWORK_MESSAGES
}

void printState(myNCB** connections) {
#ifdef TEST_NETWORK_MESSAGES
	debug_net_printf("NetworkGetState: %d %p %d %s\n", 0, connections[0], connections[0]->ncb_lsn_2, (!connections[0]->ncb_cmd_cplt_49) ? "true" : "false");
	debug_net_printf("NetworkGetState: %d %p %d %s\n", 1, connections[1], connections[1]->ncb_lsn_2, (!connections[1]->ncb_cmd_cplt_49) ? "true" : "false");
	debug_net_printf("NetworkGetState: %d %p %d %s\n", 2, connections[2], connections[2]->ncb_lsn_2, (!connections[2]->ncb_cmd_cplt_49) ? "true" : "false");
#endif //TEST_NETWORK_MESSAGES
}

