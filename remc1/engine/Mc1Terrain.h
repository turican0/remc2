#pragma once

#include <cstdint>

#include "Mc1GameState.h"

// MC1 changes of the terrain in the game: the type / angle / shading of tiles and of rectangles

extern int32_t lowRange_90758_90758; // set by GetTileHeightType_33640: the heights of the tile differ by 8 or less
extern const uint8_t terrainTypes_909AC_909AC[144][2]; // type and angle, the tables 909AC, 909BC, 909EC, 909FC, 90A2C

// callerCl / callerDl: the registers of the caller the original reads when the tile does not set them
uint8_t GetTileHeightType_33640_33A30(uint8_t callerCl, uint16_t tile, uint8_t callerDl);
void SetTileTerrainType_33800_33BF0(uint8_t callerCl, uint16_t tile, uint8_t type, uint8_t callerDl);
void SetTerrainTypeAround_33AE0_33ED0(uint16_t tile, uint8_t type);
void UpdateTerrainTiles_33B90_33F80(uint16_t from, uint16_t to);
void RebuildTerrainTiles_33E10_34200(uint16_t from, uint16_t to);
bool CheckTileType8Neighbours_264D0_264D0(uint16_t tile); // false: type 8 left and no neighbour higher by more than 1Eh
uint8_t ChangeTileHeight_40A10_40D50(int32_t x, int32_t y, int32_t add, uint8_t fixed);
uint8_t ChangeHeightsAround_40D30_41070(Type_Mc1Entity* entity, int16_t first, int32_t last, int16_t add, uint8_t fixed);
void LowerHeightsAround_255D0_255D0(Type_Mc1Entity* entity, int16_t first, int32_t last);
void SmoothBuildingBorder_35F30_362F0(uint8_t x, uint8_t y, int32_t halfHeight, int32_t halfWidth, uint8_t border);
void SmoothTileHeight_360C0_36480(uint16_t tile);
uint32_t GetTileTypeBit_11810_11810(const Type_Mc1Position* position);
