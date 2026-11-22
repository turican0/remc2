#include "Mc1NotPorted.h"

#include "Mc1Game.h"

#include <set>
#include <string>

#include "../../remc2/portability/port_filesystem.h"

static void NotPorted(const char* name)
{
	static std::set<std::string> reported;
	if (reported.insert(name).second)
		Logger->warn("MC1: {} is not ported yet", name);
}

// a menu screen not ported yet behaves as if the player quit the game in it
static void MenuScreenNotPorted(const char* name)
{
	NotPorted(name);
	mc1State_AE400_AE3F0->players_0x340B_13323[mc1State_AE400_AE3F0->currentPlayer_0x08_8].exitGame_0x04_4 = 1;
	menuDone_96880_96880 = 1;
}

//!!!!! not ported yet
void sub_4D680_4D9C0() { MenuScreenNotPorted(__func__); }
//!!!!! not ported yet
//!!!!! not ported yet
//!!!!! not ported yet
void sub_433C0_43700() { NotPorted(__func__); }
//!!!!! not ported yet
void InitNetwork_43270_435B0() { NotPorted(__func__); }
//!!!!! not ported yet
int16_t JoinNetworkSession_43430_43770(char* /*sessionName*/, uint16_t /*playerCount*/) { NotPorted(__func__); return -1; }

// class functions (generated, gen_classes.py)
//!!!!! not ported yet
// 45C90: the sounds of the flight (HMI)

//!!!!! not ported yet
void sub_42C70_42FB0(int16_t /*player*/) { NotPorted(__func__); }
void sub_42DE0_43120(uint32_t /*handle*/) { NotPorted(__func__); }
void sub_42D40_43080(uint32_t /*handle*/, char* /*name*/) { NotPorted(__func__); }
void sub_43150_43490() { NotPorted(__func__); }
void sub_438D0_43C10(uint16_t /*player*/, uint8_t* /*buffer*/, uint32_t /*size*/) { NotPorted(__func__); }
void sub_43870_43BB0(uint16_t /*player*/, uint8_t* /*buffer*/, uint32_t /*size*/) { NotPorted(__func__); }
