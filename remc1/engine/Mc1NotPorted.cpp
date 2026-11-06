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

//!!!!! not ported yet
void CreateInstallDirectory_3EC90_3EFD0(char /*drive*/, const char* /*basePath*/, const char* /*subDirectory*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_34B40_34F00() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_3F1D0() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_3F510() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_58F90_594A0() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_41230() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_41570() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_44730_44A70() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_4A980_4ACC0() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_40440_40780(const char* /*path*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_44700_44A40(uint8_t* /*text*/, uint8_t** /*pointers*/, int32_t /*count*/) { NotPorted(__func__); }
//!!!!! not ported yet
void FadeInOut_61CC0_621D0(uint8_t* /*palette*/, uint8_t /*steps*/, int32_t /*mode*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_31A00_31A40() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_34C80() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_35040() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_41490_417D0() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_42530_42870(int32_t /*value*/) { NotPorted(__func__); }
//!!!!! not ported yet, behaves as if the player quit the game in the menu
void sub_4AB20()
{
	NotPorted(__func__);
	mc1State_AE400_AE3F0->players_0x340B_13323[mc1State_AE400_AE3F0->currentPlayer_0x08_8].exitGame_0x04_4 = 1;
	menuDone_96880_96880 = 1;
}
//!!!!! not ported yet, behaves as if the player quit the game in the menu
void sub_4AE60()
{
	NotPorted(__func__);
	mc1State_AE400_AE3F0->players_0x340B_13323[mc1State_AE400_AE3F0->currentPlayer_0x08_8].exitGame_0x04_4 = 1;
	menuDone_96880_96880 = 1;
}
//!!!!! not ported yet
void sub_5C05C_5C56C(uint8_t* /*table*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_34460_34820() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_34B00_34EC0() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_61610_61B20(const char* /*path*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_41540_41880() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_5D070_5D580(int32_t /*value*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_5CEF0_5D400(int32_t /*value*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_31A60_31AA0() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_407A0_40AE0() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_5D290_5D7A0(int16_t /*variant*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_59720_59C30() { NotPorted(__func__); }
//!!!!! not ported yet
void GameLoop_34610_349D0() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_5D010_5D520() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_20E60_20E60() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_448E0_44C20() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_408D0_40C10() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_358A0_35C60() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_34C60_35020() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_433C0_43700() { NotPorted(__func__); }
//!!!!! not ported yet
void InitNetwork_43270_435B0() { NotPorted(__func__); }
//!!!!! not ported yet, 0 = no VFX1 headset
int32_t InitVfx_5AF50_5B460() { NotPorted(__func__); return 0; }
//!!!!! not ported yet
void InitDigitalJoystick_3F7D0_3FB10() { NotPorted(__func__); }
//!!!!! not ported yet
void InitAnalogJoystick_3F820() { NotPorted(__func__); }
//!!!!! not ported yet
void InitAnalogJoystick_3FB60() { NotPorted(__func__); }
//!!!!! not ported yet
int16_t JoinNetworkSession_43430_43770(char* /*sessionName*/, uint16_t /*playerCount*/) { NotPorted(__func__); return -1; }
