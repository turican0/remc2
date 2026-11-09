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
void sub_34C80() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_35040() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_3FCA0_3FFE0(int32_t /*mode*/, int32_t /*minX*/, int32_t /*minY*/, int32_t /*maxX*/, int32_t /*maxY*/, int32_t /*stepX*/, int32_t /*stepY*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_4D680_4D9C0() { MenuScreenNotPorted(__func__); }
//!!!!! not ported yet
void sub_4E5B0_4E8F0() { MenuScreenNotPorted(__func__); }
//!!!!! not ported yet
void sub_4D560_4D8A0() { MenuScreenNotPorted(__func__); }
//!!!!! not ported yet
void sub_34460_34820() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_31600_31640() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_407A0_40AE0() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_59720_59C30() { NotPorted(__func__); }
//!!!!! not ported yet
void GameLoop_34610_349D0() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_448E0_44C20() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_408D0_40C10() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_58F00_59410() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_433C0_43700() { NotPorted(__func__); }
//!!!!! not ported yet
void InitNetwork_43270_435B0() { NotPorted(__func__); }
//!!!!! not ported yet, 0 = no VFX1 headset
int32_t InitVfx_5AF50_5B460() { NotPorted(__func__); return 0; }
//!!!!! not ported yet
void InitDigitalJoystick_3F7D0_3FB10(bitmap_pos_struct_t* /*sprite*/) { NotPorted(__func__); }
//!!!!! not ported yet
void InitAnalogJoystick_3F820(bitmap_pos_struct_t* /*sprite1*/, bitmap_pos_struct_t* /*sprite2*/, bitmap_pos_struct_t* /*sprite3*/, uint8_t /*flags*/) { NotPorted(__func__); }
//!!!!! not ported yet
void InitAnalogJoystick_3FB60(bitmap_pos_struct_t* /*sprite1*/, bitmap_pos_struct_t* /*sprite2*/, bitmap_pos_struct_t* /*sprite3*/, uint8_t /*flags*/) { NotPorted(__func__); }
//!!!!! not ported yet
int16_t JoinNetworkSession_43430_43770(char* /*sessionName*/, uint16_t /*playerCount*/) { NotPorted(__func__); return -1; }
