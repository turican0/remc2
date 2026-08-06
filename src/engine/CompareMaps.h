#pragma once
#ifndef MAIN_COMPARE_MAPS
#define MAIN_COMPARE_MAPS
#include "globals.h"
//#include "Basic.h"
#include "BasicMini.h"
#include "BasicTerrain.h"
#include "Unk_D4A30.h"
#include "Unk_D47E0.h"

#include "Level.h"
#include "engine_support.h"
#include "Terrain.h"

void add_compare(uint32_t adress, bool debugafterload, int stopstep = -1, bool skip = false, int exitindex = 1000000, int skip2 = 0);
void add_compare2(uint32_t adress, uint8_t* memadress, uint32_t dosmemadress, uint32_t size, bool debugafterload, int stopstep = -1, bool skip = false, int exitindex = 1000000);

#endif //MAIN_COMPARE_MAPS