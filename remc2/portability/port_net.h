#ifndef PORT_NETWORK
#define PORT_NETWORK

#include <cstdlib>
#include <iostream>
//#include <boost/bind/bind.hpp>
//#include <boost/asio.hpp>

#include "../engine/types.h"

extern const char* debug_net_filename1;
extern std::string debug_net_filename2;

extern int MultiplayerPort;

typedef struct _REGS {
	uint32 eax;
	uint32 ebx;
	uint32 ecx;
	uint32 edx;
	uint32 esi;
	uint32 edi;
	uint32 cflag;
} REGS;

struct SREGS {
	unsigned short es;
	unsigned short ds;
	unsigned short fs;
	unsigned short gs;
	unsigned short cs;
	unsigned short ss;
};

typedef struct {//lenght 10
	char byte_0[50];
}
type_v2x;

typedef union {
	uint8_t* p;
	uint32_t align;
} p64align;

#pragma pack (1)
typedef struct {//lenght 66(changed to 70)
	uint8_t ncb_command_0;
	uint8_t ncb_retcode_1;
	uint8_t ncb_lsn_2;
	uint8_t ncb_num_3;
	p64align ncb_buffer_4;
	uint16_t ncb_bufferLength_8;
	char ncb_callName_10[16];
	char ncb_name_26[16];
	uint8_t ncb_rto_42;
	uint8_t ncb_sto_43;
	p64align ncb_post_44;
	uint8_t ncb_lana_num_48;
	uint8_t ncb_cmd_cplt_49;//lock for end command
	uint8_t ncb_reserved_50[16];
}
myNCB;
#pragma pack (16)

#define MaxMessageSize 20000

#pragma pack (1)
typedef struct {
	uint8_t stamp[9];
	uint8_t compid[8];
	uint32_t type;
	uint32_t lenght;
	char ip[20];
	uint8_t mesg[MaxMessageSize];
}
messType;
#pragma pack (16)


void NetworkTestServer();


bool NetworkGetInitInfoFromServer(char* serverIP);
char* NetworkListenForClients();

void makeNetwork(int irg, REGS* v7x, REGS* v10x, SREGS* v12x, type_v2x* v2x, myNCB* connection);
void fake_network_interupt(myNCB* connection);


#endif //PORT_TIME
