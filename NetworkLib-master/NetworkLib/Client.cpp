#include "Client.h"
#include "Log.h"

namespace NetworkLib {
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
		socket.async_receive_from(asio::buffer(recv_buffer), remote_endpoint,
			[this](std::error_code ec, std::size_t bytes_recvd){ this->handle_receive(ec, bytes_recvd); });
	}

	void Client::handle_receive(const std::error_code& error, std::size_t bytes_transferred)
	{
		if (!error)
		{
			std::string message(recv_buffer.data(), recv_buffer.data() + bytes_transferred);
			incomingMessages.push(message);
			statistics.RegisterReceivedMessage(bytes_transferred);
		}
		else
		{
			Log::Error("Client::handle_receive:", error);
		}

		start_receive();
	}

	void Client::Send(const std::string& message)
	{
		socket.send_to(asio::buffer(message), server_endpoint);
		statistics.RegisterSentMessage(message.size());
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
}