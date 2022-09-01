#ifndef PORT_NETWORK
#define PORT_NETWORK

#include <cstdlib>
#include <iostream>
#include <fstream>

#include "../engine/defs.h"
#include "../portability/port_outputs.h"

extern const char* debug_net_filename1;
extern std::string debug_net_filename2;

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
	//p64align ncb_post_44;// not used
	//uint8_t ncb_lana_num_48;// not used
	uint8_t ncb_cmd_cplt_49;//lock for end command
	//uint8_t ncb_reserved_50[16];// not used
}
myNCB;

typedef struct {//lenght 66(changed to 70)
	uint8_t ncb_command_0;
	uint8_t ncb_retcode_1;
	uint8_t ncb_lsn_2;
	uint8_t ncb_num_3;
	uint32_t ncb_buffer_4;
	uint16_t ncb_bufferLength_8;
	char ncb_callName_10[16];
	char ncb_name_26[16];
	uint8_t ncb_rto_42;
	uint8_t ncb_sto_43;
	//p64align ncb_post_44;// not used
	//uint8_t ncb_lana_num_48;// not used
	uint8_t ncb_cmd_cplt_49;//lock for end command
	//uint8_t ncb_reserved_50[16];// not used
}
shadow_myNCB;
#pragma pack (16)

void makeNetwork(myNCB* connection);

void EndMyNetLib();

void AddRecMess(std::string message);
std::string GetRecMess();
unsigned int GetRecCount();
void CleanMessages(myNCB locNCB);

void printState(myNCB** connections);
void printState2(char* text);

bool ReceiveServerAddName();

void InitMyNetLib(bool iam_server, char* ip, int networkPort, int serverPort);

void debug_net_printf(const char* format, ...);

void timeState(bool start, const char* text);

#endif //PORT_NETWORK
