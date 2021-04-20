#ifndef PORT_NETWORK
#define PORT_NETWORK


#ifdef _WIN32
  /* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501  /* Windows XP. */
#endif
#include <windows.h>
//#include <winsock2.h>
//#include <Ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")
#else
  /* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */
#endif

int sockInit(void);

#endif //PORT_TIME
