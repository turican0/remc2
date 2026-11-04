#pragma once
#ifndef MAIN_READANDDECOMPRESS
#define MAIN_READANDDECOMPRESS
#include "MapType.h"
#include "Basic.h"

//#include "Entityxxx.h"

extern uint8_t BigTextureBuffer[128 * 128 * 160];//main
//signed int GetFileUnpackSize_AB9E1(const char* a1);//dataIO
//signed int UnpackAndLoadMemoryFromPath(Pathstruct path);//dataIO
//uint8_t* ClearMemoryForPath(Pathstruct path);//dataIO
//bool sub_84250_load_file_array(int psindex);//level
void LoadPsxBlock_54630(uint16_t TextSize);//main
void UpdateTileTextures(uint32_t tileSize, uint8_t* tileBuffer, MapType_t mapType);
void UpdateSkyTexture(uint32_t size, uint8_t* skyBuffer);
void ReadAndDecompressSkyAndBlocks_54660(MapType_t GraphicsType, uint8_t GraphicsSize);//main
void ReadAndDecompressTables_54800(MapType_t a1);//main

#endif //MAIN_READANDDECOMPRESS