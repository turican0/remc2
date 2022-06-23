#include "../NetworkLib/Factory.h"
#include "CppUnitTest.h"
#include <memory>
#include <thread>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
namespace chrono = std::chrono;

namespace Multiorb_UnitTests
{
	TEST_CLASS(NetworkTests)
	{
		static std::unique_ptr<NetworkLib::IServer> CreateServer()
		{
			return NetworkLib::Factory::CreateServer(12345);
		};

		static std::unique_ptr<NetworkLib::IClient> CreateClient()
		{
			return NetworkLib::Factory::CreateClient("localhost", 12345, 0);
		};

		static void Sleep()
		{
			std::this_thread::sleep_for(chrono::milliseconds(5));
		}

	public:
		TEST_METHOD(ServerConstructorShouldWork)
		{
			auto server = CreateServer();
		};

		TEST_METHOD(ServerShouldHaveNoMessagesWhenCreated)
		{
			auto server = CreateServer();
			Assert::IsFalse(server->HasMessages());
		};

		TEST_METHOD(ServerShouldHaveZeroClientsWhenCreated)
		{
			auto server = CreateServer();
			Assert::IsFalse(server->GetClientCount() > 0);
		};


		TEST_METHOD(ClientConstructorShouldWork)
		{
			auto client = CreateClient();
		};

		TEST_METHOD(ClientShouldHaveNoMessagesWhenCreated)
		{
			auto client = CreateClient();
			Assert::IsFalse(client->HasMessages());
		};

		TEST_METHOD(ServerShouldCountMultipleClients)
		{
			auto server = CreateServer();
			std::vector<std::unique_ptr<NetworkLib::IClient>> clients;
			for (int i = 0; i < 5; i++)
				clients.emplace_back(CreateClient());
			Sleep();
			Assert::IsTrue(server->GetClientCount() == clients.size());
		}

		//TEST_METHOD(ClientShouldDisconnectFromServerWhenDestroyed)
		//{
		//	auto server = CreateServer();
		//	{
		//		std::vector<std::unique_ptr<NetworkLib::IClient>> clients;
		//		for (int i = 0; i < 5; i++)
		//			clients.emplace_back(CreateClient());
		//		Sleep();
		//		Assert::IsTrue(server->GetClientCount() == clients.size());
		//	}
		//	Sleep();
		//	Assert::IsTrue(server->GetClientCount() == 0);
		//}

		TEST_METHOD(SendMessageFromClientToServerShouldProduceSameMessage)
		{
			auto server = CreateServer();
			auto client = CreateClient();
			std::string message("Test message");

			// Send client->server
			client->Send(message);

			Sleep();

			Assert::IsTrue(server->HasMessages());
			Assert::IsFalse(client->HasMessages());

			auto receivedMessage = server->PopMessage().first;
			Assert::IsTrue(message == receivedMessage);

			Assert::IsFalse(server->HasMessages());
			Assert::IsFalse(client->HasMessages());
		}

		TEST_METHOD(SendMessageFromServerToClientShouldProduceSameMessage)
		{
			auto server = CreateServer();
			auto client = CreateClient();
			std::string message("Test message");

			// Sleep for a bit so that server has time to 
			// receive client announcement message
			Sleep();

			// Send from server to client
			// TODO: get client ID from server itself
			Assert::IsTrue(server->GetClientCount() == 1);
			server->SendToClient(message, server->GetClientIdByIndex(0));
			Sleep();

			Assert::IsFalse(server->HasMessages());

			auto receivedMessage = client->PopMessage();
			Assert::IsTrue(message == receivedMessage);

			Assert::IsFalse(server->HasMessages());
			Assert::IsFalse(client->HasMessages());
		}
	};
}
