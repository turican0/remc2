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
// 3FCA0: the VFX1 head tracker (only with vfxTracking_9AD08, InitVfx_5AF50 finds none)
int32_t sub_5B210_5B720() { NotPorted(__func__); return 1; }
void sub_5B2B0_5B7C0() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_4D680_4D9C0() { MenuScreenNotPorted(__func__); }
//!!!!! not ported yet
void sub_4E5B0_4E8F0() { MenuScreenNotPorted(__func__); }
//!!!!! not ported yet
void sub_4D560_4D8A0() { MenuScreenNotPorted(__func__); }
//!!!!! not ported yet
//!!!!! not ported yet
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

// class functions (generated, gen_classes.py)
//!!!!! not ported yet
// 45C90: the sounds of the flight (HMI)

//!!!!! not ported yet
int32_t sub_3E350_3E690(int16_t /*movie*/) { NotPorted(__func__); return 0; }
int32_t sub_3E410_3E750(int16_t /*movie*/) { NotPorted(__func__); return 0; }
int32_t sub_3E4B0_3E7F0(int16_t /*movie*/) { NotPorted(__func__); return 0; }
int32_t sub_3E580_3E8C0(int16_t /*movie*/) { NotPorted(__func__); return 0; }
void sub_42C70_42FB0(int16_t /*player*/) { NotPorted(__func__); }
void sub_42DE0_43120(uint32_t /*handle*/) { NotPorted(__func__); }
void sub_42D40_43080(uint32_t /*handle*/, char* /*name*/) { NotPorted(__func__); }
void sub_43150_43490() { NotPorted(__func__); }
void sub_438D0_43C10(uint16_t /*player*/, uint8_t* /*buffer*/, uint32_t /*size*/) { NotPorted(__func__); }
void sub_43870_43BB0(uint16_t /*player*/, uint8_t* /*buffer*/, uint32_t /*size*/) { NotPorted(__func__); }
// 20FB0: the icons and the spells
//!!!!! not ported yet
void sub_5A560_5AA70() { NotPorted(__func__); }
//!!!!! not ported yet
// 16B00 Alt+S / Alt+L: the game saved to save/gam00199.dat (the state) and save/map00199.dat (the maps), needs the
// state without pointers
int32_t sub_3E690_3E9D0(int16_t /*number*/) { NotPorted(__func__); return 0; }
int32_t sub_3E750_3EA90(int16_t /*number*/) { NotPorted(__func__); return 0; }
int32_t sub_3E7F0_3EB30(int16_t /*number*/) { NotPorted(__func__); return 0; }
int32_t sub_3E8C0_3EC00(int16_t /*number*/) { NotPorted(__func__); return 0; }
