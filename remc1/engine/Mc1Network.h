#pragma once

#include <cstdint>

#include "../../remc2/portability/port_net.h"

// MC1 network game over NetBIOS (int 5Ch), port: the NCBs go to port_net of remc2 (SetNetbios_75044), the same
// emulation as MC2; the computers are the names <session><index> (TESTER0.. before the session is set)

extern myNCB* networkNcbs_93990_93990[8]; // the session with each computer, own index: the calls of this one
extern char networkName_939B8_939B8[0x14]; // aTester: the session name, the prefix of the names

int32_t InitNetwork_43270_435B0();
void FreeNetwork_433C0_43700();
int16_t JoinNetworkSession_43430_43770(char* sessionName, uint16_t playerCount);
int32_t CancelNcb_42C70_42FB0(int16_t player);
int32_t DeleteName_42D40_43080(myNCB* ncb, const char* name);
int32_t HangUp_42DE0_43120(myNCB* ncb);
void UpdateNetworkActive_43150_43490();
void ReceiveFrom_438D0_43C10(uint16_t player, uint8_t* buffer, uint32_t size);
void SendTo_43870_43BB0(uint16_t player, uint8_t* buffer, uint32_t size);

// the menu item 4: the network game screen (the level, the session name, the number of the players)
void NetworkGameScreen_4D680_4D9C0();

// port: server <port> / client <ip> <server port> <own port> of the command line, as MC2 (with --network)
bool ParseNetworkOption(const char* option, int argc, char** argv, int& index);
void StartNetworkLibrary();
void StopNetworkLibrary();
