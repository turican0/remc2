#include "port_net.h"

/*
int sockInit(void)
{
#ifdef _WIN32
	WSADATA wsa_data;
	return WSAStartup(MAKEWORD(1, 1), &wsa_data);
#else
	return 0;
#endif
}

int sockQuit(void)
{
#ifdef _WIN32
	return WSACleanup();
#else
	return 0;
#endif
}

int sockClose(SOCKET sock)
{

	int status = 0;

#ifdef _WIN32
	//status = shutdown(sock, SD_BOTH);
	status = shutdown(sock, 2);
	if (status == 0) { status = closesocket(sock); }
#else
	status = shutdown(sock, SHUT_RDWR);
	if (status == 0) { status = close(sock); }
#endif

	return status;

}
*/
using namespace cppnet;

#ifdef _WIN32 || _WIN64
#include <winsock2.h>
void SetNoDelay(const uint64_t& sock) {
	int opt = 1;
	int ret = setsockopt(sock, SOL_SOCKET, TCP_NODELAY, (const char*)&opt, sizeof(opt));
}
#else
#include <netinet/tcp.h>
#include <netinet/in.h>
void SetNoDelay(const uint64_t& sock) {
	int optval = 1;
	setsockopt(sock, IPPROTO_TCP, TCP_NODELAY,
		&optval, static_cast<socklen_t>(sizeof(optval)));
}
#endif

static std::atomic_int count;

void OnConnection(const Handle& handle, uint32_t error) {

	count++;
	if (error == CEC_SUCCESS) {
		std::cout << " accept a socket. count: " << count << std::endl;
		//SetNoDelay(handle);
	}
}

void OnMessage(const Handle& handle, std::shared_ptr<cppnet::Buffer> data, uint32_t) {
	char buff[65535];

	while (data->GetCanReadLength()) {
		int ret = data->Read(buff, 65535);
		handle->Write(buff, ret);
	}
}

void NetworkTestServer()
{
	cppnet::CppNet net;
	net.Init(4);

	net.SetAcceptCallback(OnConnection);
	net.SetReadCallback(OnMessage);

	net.ListenAndAccept("0.0.0.0", 8921);

	net.Join();
}