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
void sub_31600_31640() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_59230_59740() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_3DD50_3E090() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_48230_48570(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
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

// class functions (generated, gen_classes.py)
//!!!!! not ported yet (class action)
void sub_13170_13170(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_19550_19550(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B060_1B060(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B070_1B070(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B090_1B090(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B0E0_1B0E0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B100_1B100(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B110_1B110(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B160_1B160(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B200_1B200(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B2D0_1B2D0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B320_1B320(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B330_1B330(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B340_1B340(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B350_1B350(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B370_1B370(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B3C0_1B3C0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B4C0_1B4C0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B4E0_1B4E0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B4F0_1B4F0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B500_1B500(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B510_1B510(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B520_1B520(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B570_1B570(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B580_1B580(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B590_1B590(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B5A0_1B5A0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1B5D0_1B5D0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1BB20_1BB20(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1BBE0_1BBE0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1BC10_1BC10(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1BC40_1BC40(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1BD10_1BD10(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1BD20_1BD20(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1C110_1C110(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1C170_1C170(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1C3C0_1C3C0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1C3D0_1C3D0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1C490_1C490(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1C4A0_1C4A0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1C4F0_1C4F0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1C880_1C880(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1C8D0_1C8D0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1C8E0_1C8E0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1C8F0_1C8F0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1C900_1C900(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1C960_1C960(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1CA00_1CA00(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1CA20_1CA20(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1CA30_1CA30(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1CA40_1CA40(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1CA50_1CA50(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1CE30_1CE30(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1CF50_1CF50(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1CF60_1CF60(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1CFE0_1CFE0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1CFF0_1CFF0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1D060_1D060(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1DA60_1DA60(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1DC80_1DC80(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1DCB0_1DCB0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1DCC0_1DCC0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1DDD0_1DDD0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1DDE0_1DDE0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1DDF0_1DDF0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1DE10_1DE10(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1DE20_1DE20(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1DE30_1DE30(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1DE40_1DE40(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1DFE0_1DFE0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1E380_1E380(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1E6F0_1E6F0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1E700_1E700(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1E710_1E710(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1EA40_1EA40(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1EED0_1EED0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1F120_1F120(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1F390_1F390(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1F5A0_1F5A0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1F5B0_1F5B0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1F640_1F640(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1FA00_1FA00(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1FAA0_1FAA0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1FAC0_1FAC0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1FE90_1FE90(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1FEC0_1FEC0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1FF50_1FF50(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_1FF60_1FF60(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_201D0_201D0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_203E0_203E0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_203F0_203F0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_20400_20400(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_20700_20700(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_20710_20710(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_207E0_207E0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_20B60_20B60(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_20B70_20B70(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_20B80_20B80(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_24F60_24F60(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_25130_25130(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_252B0_252B0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_252D0_252D0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_253F0_253F0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_25410_25410(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_25760_25760(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_257B0_257B0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_258A0_258A0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_25990_25990(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_25A60_25A60(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_25CE0_25CE0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_25EC0_25EC0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_26140_26140(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_262D0_262D0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_26360_26360(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_263C0_263C0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_26A60_26A60(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_26C00_26C00(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_26CE0_26CE0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_26D20_26D20(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_26E90_26E90(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_27030_27030(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_275C0_275C0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_28200_28200(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_285C0_285C0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_28DC0_28DC0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_28FE0_28FE0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_293D0_293D0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_29540_29540(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_296A0_296A0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_29700_29700(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_29780_29780(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_29920_29960(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_299D0_29A10(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_37A80_37E40(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_37AA0_37E60(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_37AC0_37E80(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_37AE0_37EA0(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_37B00_37EC0(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_37B20_37EE0(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_37B40_37F00(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_37B60_37F20(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_37B80_37F40(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_37BA0_37F60(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39800_39B80(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39820_39BA0(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39840_39BC0(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39860_39BE0(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39880_39C00(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_398A0_39C20(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_398C0_39C40(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39940_39CC0(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39960_39CE0(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39980_39D00(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_399A0_39D20(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_399C0_39D40(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_399E0_39D60(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39A10_39D90(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39A90_39E10(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39B40_39EC0(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39BC0_39F40(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39C40_39FC0(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39CC0_3A040(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39D40_3A0C0(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39DC0_3A140(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39E40_3A1C0(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39EC0_3A240(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39F40_3A2C0(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_39FC0_3A340(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_3A040_3A3C0(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_3A0C0_3A440(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_3A1A0_3A520(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_3A210_3A590(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_3A270_3A5F0(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_3A2F0_3A670(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_3A390_3A710(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_3A410_3A790(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_3C4B0_3C7F0(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_3C4E0_3C820(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_3C510_3C850(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class model)
Type_Mc1Entity* sub_3C540_3C880(Type_Mc1Position* /*position*/) { NotPorted(__func__); return nullptr; }
//!!!!! not ported yet (class action)
void sub_45C90_45FD0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_45FC0_46300(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_46480_467C0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_46DB0_470F0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_46F10_47250(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_470E0_47420(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_47F90_482D0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_49890_49BD0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_499C0_49D00(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_49A50_49D90(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_49AA0_49DE0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_49AD0_49E10(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_49B50_49E90(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_52B30_52E70(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_52ED0_53210(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_53060_533A0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_53070_533B0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_530B0_533F0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_530C0_53400(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_535E0_53920(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_53980_53CC0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_53DC0_54100(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_54180_544D0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_54290_545E0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_542B0_54640(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_54480_54810(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_54600(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_56090_565C0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_56250_56780(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_56260_56790(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_56270_567A0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_56380_568B0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_56510_56A40(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_566C0_56BF0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_56730_56C60(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_567A0_56CD0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_56950_56E80(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_56AF0_57020(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_56CA0_571D0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_56E50_57380(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_57040_57570(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_571B0_576E0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_57250_57780(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_573F0_57920(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_57470_579A0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_57610_57B40(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_57800_57D30(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_579D0_57F00(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_57B80_580B0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_57D40_58270(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_57F00_58410(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_580A0_585B0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_58240_58750(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59A80_59F90(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59AB0_59FC0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59AE0_59FF0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59B30_5A040(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59B80_5A090(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59C40_5A150(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59C70_5A180(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59CA0_5A1B0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59CF0_5A200(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59D40_5A250(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59D70_5A280(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59DA0_5A2B0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59DF0_5A300(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59F60_5A470(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59F70_5A480(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59F80_5A490(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59F90_5A4A0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59FA0_5A4B0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59FB0_5A4C0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59FC0_5A4D0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59FD0_5A4E0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59FE0_5A4F0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_59FF0_5A500(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_5A000_5A510(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_5A010_5A520(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_5A020_5A530(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_5A030_5A540(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_5A040_5A550(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_5A050_5A560(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_5A060_5A570(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet (class action)
void sub_5A070_5A580(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }
//!!!!! not ported yet
void sub_274D0_274D0(Type_Mc1Entity* /*entity*/) { NotPorted(__func__); }

//!!!!! not ported yet
void sub_55370_558A0(int16_t /*entityIndex*/, int32_t /*sound*/, int32_t /*volume*/)
{
	// the sounds off (939E5 / 939E4, the load of the level): nothing
	if (!byte_939E5_939E5 || !byte_939E4_939E4)
		return;
	NotPorted(__func__);
}
