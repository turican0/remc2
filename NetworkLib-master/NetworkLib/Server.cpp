#include "Server.h"
#include "Log.h"

namespace NetworkLib {
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
		socket.async_receive_from(asio::buffer(recv_buffer), remote_endpoint,
			[this](std::error_code ec, std::size_t bytes_recvd){ this->handle_receive(ec, bytes_recvd); });
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
				statistics.RegisterReceivedMessage(bytes_transferred);
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
		socket.send_to(asio::buffer(message), target_endpoint);
		statistics.RegisterSentMessage(message.size());
	}

	void Server::run_service()
	{
		start_receive();
		while (!io_service.stopped()){
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
		clients.insert(Client(id, endpoint));
		return id;
	};

	void Server::SendToClient(const std::string& message, uint32_t clientID)
	{
		try {
			send(message, clients.at(clientID));
		}
		catch (std::out_of_range) {
			Log::Error(__FUNCTION__": Unknown client ID ", clientID);
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

	bool Server::HasMessages()
	{
		return !incomingMessages.empty();
	};
}