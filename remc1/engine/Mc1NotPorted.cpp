#include "Mc1NotPorted.h"

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
//!!!!! not ported yet, -1 stops the top procedure before the game state is used
int16_t ProcessCommandLine_34DD0_35190(uint16_t /*argc*/, char** /*argv*/) { NotPorted(__func__); return -1; }
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
//!!!!! not ported yet
void sub_4AB20() { NotPorted(__func__); }
//!!!!! not ported yet
void sub_4AE60() { NotPorted(__func__); }
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
void* Malloc_42540_42880(uint32_t /*size*/) { NotPorted(__func__); return nullptr; }
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
