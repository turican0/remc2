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
void sub_4D3B0_4D6F0() { MenuScreenNotPorted(__func__); }
//!!!!! not ported yet
void sub_4BD60_4C0A0() { MenuScreenNotPorted(__func__); }
//!!!!! not ported yet
void sub_4D680_4D9C0() { MenuScreenNotPorted(__func__); }
//!!!!! not ported yet
void sub_4E5B0_4E8F0() { MenuScreenNotPorted(__func__); }
//!!!!! not ported yet
void sub_4F1E0_4F520() { MenuScreenNotPorted(__func__); }
//!!!!! not ported yet
void sub_4EE70_4F1B0() { MenuScreenNotPorted(__func__); }
//!!!!! not ported yet
void sub_4D560_4D8A0() { MenuScreenNotPorted(__func__); }
//!!!!! not ported yet
void sub_65E00_66310() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_34460_34820() { NotPorted(__func__); }
//!!!!! not ported yet
// the sound file groups are not ported yet; the real function is FreeFileGroup_61610_61B20
void FreeSoundFileGroup_61610_61B20(const char* /*group*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_5D070_5D580(int32_t /*value*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_5CEF0_5D400(int32_t /*value*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_31600_31640() { NotPorted(__func__); }
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
void sub_5D230_5D740(int32_t /*a1*/, int32_t /*a2*/, int16_t /*music*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_5D460_5D970(int32_t /*a1*/, int16_t /*sample*/, int32_t /*a3*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_5D610_5DB20(int32_t /*a1*/, int16_t /*sample*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_5D750_5DC60(int32_t /*a1*/, int16_t /*sample*/) { NotPorted(__func__); }
//!!!!! not ported yet, 0 = the music still plays
int32_t sub_5D7F0_5DD00(int32_t /*music*/) { NotPorted(__func__); return 0; }
//!!!!! not ported yet (subtitles)
void sub_24AB0_24AB0(int16_t /*value*/) { NotPorted(__func__); }
//!!!!! not ported yet (subtitles)
void sub_24B80_24B80() { NotPorted(__func__); }
//!!!!! not ported yet (subtitles)
void sub_24BB0_24BB0(int16_t /*value*/) { NotPorted(__func__); }
//!!!!! not ported yet (subtitles)
void sub_24B50_24B50(const char* /*text*/) { NotPorted(__func__); }
//!!!!! not ported yet (subtitle color, MC2 2EC90)
void sub_24BF0_24BF0(uint8_t /*color*/) { NotPorted(__func__); }
//!!!!! not ported yet, remc2 InitMusic_8D970 (MSS) with the driver of the remc2 config
void InitMusic_3C570_3C8B0() { NotPorted(__func__); }
//!!!!! not ported yet, remc2 InitSound_8D290 (MSS) with the driver of the remc2 config
void InitSound_3C800_3CB40() { NotPorted(__func__); }
//!!!!! not ported yet (MC2 PrintTextMessage_70910)
void PrintMessage_5AD10_5B220(const char* /*text*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_448E0_44C20() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_408D0_40C10() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_358A0_35C60() { NotPorted(__func__); }
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
