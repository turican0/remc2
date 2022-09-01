#pragma once

#ifndef MAIN_NETWORK
#define MAIN_NETWORK

#include "engine_support.h"
#include "Basic.h"
#include "CommandLineParser.h"
#include "MenusAndIntros.h"
#include "../portability/port_net.h"

extern bool Iam_server;
extern bool Iam_client;
extern int NetworkPort;
extern int ServerPort;
extern char serverIP[256];
extern uint8_t x_BYTE_E131C[8];

void NetworkDisallocation_72D04();
// int sub_72DDE(signed __int16 *a1, int a2);
int sub_72E70(int a1, int a2, signed __int16* a3);//not used
signed int NetworkTestCall_72FBB();
void NetworkListenAll_7302E();
// int NetworkInitConnection_7308F(signed __int16 *a1, int a2, __int16 a3);
void NetworkCanceling_73669(__int16 a1);
void NetworkEvent_7373D(int16_t a1);
void NetworkRemoveClient_739AD(__int16 a1);
void NetworkSomeChange_73AA1(__int16 a1);
void NetworkEnd_73D11(__int16 a1);
void NetworkSendMessage2_74006(unsigned __int16 a1, uint8_t* a2, unsigned int a3);
void NetworkReceiveMessage2_7404E(unsigned __int16 a1, uint8_t* a2, unsigned int a3);
void ReceiveSendAll_7438A(uint8_t* a1, unsigned int a2);
void NetworkCancelAll_7449C();
int16_t GetIndexNetwork_74536();
uint8_t NetworkAllocation_74556();
// signed int sub_74767(signed __int16 *a1, x_BYTE *a2, int a3);
void NetworkCall_74809(__int16 clientIndex);
signed int NetworkCancel_748F7(__int16 a1);
signed int NetworkInit_74A11();
void NetworkDeleteName_74A86(myNCB* a1x, char* a2);
void NetworkHangUp_74B19(myNCB* a1x);
signed int NetworkListen_74B75(__int16 a1);
int NetworkReceivePacket_74C9D(myNCB* a1x, uint8_t* a2x, int maxsize);
void NetworkReceiveMessage_74D41(myNCB* a1x, uint8_t* a2x, unsigned int a3);
//signed int NetworkReceivePacket_74DD4(myNCB* a1x, unsigned __int16 a3);
int NetworkSendPacket_74E6D(myNCB* a1x, uint8_t* a2, int a3);
void NetworkSendMessage_74EF1(myNCB* a1x, uint8_t* inbuffer, unsigned int size);
void NetworkUpdateConnections_74F76();
signed int NetworkGetState_74FE1(__int16 clientIndex);
int setNetbios_75044(myNCB* a1x);
void WaitToConnect_7C230(/*int a1,*/ /*int a2*//*, signed __int16* a3*/);

bool SetMultiplayerColors_7CE50();
void DrawNetworkLevelName_7D1F0();

signed int sub_7D230(char a1, unsigned __int8 a2, unsigned __int8 a3);

void SetPaletteColor_7CDC0(unsigned __int8 a1, unsigned __int8 a2);

#endif //MAIN_NETWORK