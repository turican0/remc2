#pragma once

#ifndef MAIN_NETWORK
#define MAIN_NETWORK

#include "engine_support.h"
#include "Basic.h"
#include "CommandLineParser.h"
#include "MenusAndIntros.h"
#include "../portability/port_net.h"

extern bool Iam_server;

// Which automated match is being played, counting from 0.  The menu auto-navigation latches
// key on this: they fire once per match rather than once per process, so a second match
// re-enters the network game instead of sitting in the main menu for ever.
extern int g_autotest_match;

// Bit i is set when slot i currently holds a live connection; our own bit is always set.
//
// Not the same as the lobby's x_WORD_17DEFE, which counts slots that merely have an entry:
// straight after a server hand-over that still includes peers this node no longer has a
// session with.  Anything that must wait for the others to really be reachable again - the
// automated test starting a level, above all - has to ask this instead, or it acts while it
// is still alone and nobody hears it.
//
// A mask rather than a count, because the count alone cannot see a hand-over: losing the
// old server and gaining the other survivor leaves it at two throughout, so a caller
// watching for things to settle would never notice anything had happened.
int NetworkConnectedMask();

// True when every node the transport still lists as being in the session is also a slot this
// node holds a session with.
//
// Counting connections cannot answer that question.  The count includes ourselves, and for
// the seconds between a peer being killed and its NCB being marked closed it also includes a
// node that is already gone - so "me plus a corpse" and "me plus a live player" are the same
// number.  A host that starts the level on the count can therefore start while the player it
// is really waiting for has not finished connecting, and that player is then left in the
// lobby exchanging menu-sized records at a node that has moved on to turn-sized ones.
//
// False while the transport has no membership list yet: with nothing to compare against there
// is nothing to assert, and the caller should keep waiting rather than assume the best.
bool NetworkAllRosterPeersConnected();

// Re-aims this node's connections after the server role has moved; see the definition.
void NetworkRewireAfterServerChange();
extern bool Iam_client;
extern int NetworkPort;
extern int ServerPort;
extern char serverIP[256];
extern uint8_t x_BYTE_E131C[8];
extern uint8_t connected_E12A6;

void NetworkDisallocation_72D04();
// int sub_72DDE(signed __int16 *a1, int a2);
int sub_72E70(signed __int16* a3);//not used
signed int NetworkTestCall_72FBB();
void NetworkListenAll_7302E();
// int NetworkInitConnection_7308F(signed __int16 *a1, int a2, __int16 a3);
void NetworkCanceling_73669(__int16 a1);

// Hand the current session back, if we are in one, so a NEW match can be joined.
//
// NetworkInitConnection_7308F refuses outright while the game still believes it is in a
// session, and it does so before sending anything - so a second network game is turned away
// with no traffic at all to show for it.  That is the reported "cannot start another network
// game after finishing one".  Ending a multiplayer level has to call this; it is the same
// teardown the game already performs when the local player leaves.
void NetworkLeaveSession();
void NetworkEvent_7373D(int16_t a1);
void NetworkRemoveClient_739AD(__int16 a1);
void NetworkSomeChange_73AA1(__int16 a1);
void NetworkEnd_73D11(__int16 a1);
void NetworkSendMessage2_74006(unsigned __int16 a1, uint8_t* a2, unsigned int a3);
void NetworkReceiveMessage2_7404E(unsigned __int16 a1, uint8_t* a2, unsigned int a3);
void ReceiveSendAll_7438A(uint8_t* a1, unsigned int a2);
extern bool g_inGameLoop;
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

int FindFreeColorIndex_7D230(char searchBackward, uint8 startColor, uint8 excludeSlot);

void SetPaletteColor_7CDC0(uint8 colorIndex, uint8 playerIndex);

uint8_t NetworkAddName_74767(/*signed __int16* a1,*/ myNCB* a2x, char* a3);

void NetworkDisallocation2_5C450();

void UpdateNetInfo();

void RemoveDeadClients();

void InitNetworkInfo();

#endif //MAIN_NETWORK