#ifndef PORT_NETWORK
#define PORT_NETWORK

#include <cstdlib>
#include <iostream>
//#include <boost/bind/bind.hpp>
//#include <boost/asio.hpp>



void NetworkTestServer();


bool NetworkGetInitInfoFromServer(char* serverIP);
char* NetworkListenForClients();


#endif //PORT_TIME
