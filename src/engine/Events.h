#pragma once

#include "global_types.h"
#include "Level.h"
#include "CompareMaps.h"

#pragma pack(push, 1)
typedef struct {//lenght 6
	int8_t* bitmapData_0;
	int16_t bitmapIndex_4;
}
sub2x_BYTE_E7EE0x;
typedef struct {//lenght 24
	int32_t dword_0;
	int32_t dword_4;
	int32_t dword_8;
	int32_t dword_12;
	sub2x_BYTE_E7EE0x* bitmapsStr_16;
	int8_t* bitmapsData_20xx;
}
subx_BYTE_E7EE0x;
#pragma pack(pop)


extern subx_BYTE_E7EE0x str_E7EE0x[700];
extern sub2x_BYTE_E7EE0x bitmaps_E9980x[69];


unsigned int AddE7EE0x_10080(int32_t a1, int32_t a2);
void ResetEvent08_10100(int32_t a1);
void GenerateEvents_49290(Type_Level_2FECE* terrain, char a2, uint16_t width, uint16_t height);
void CopyMaxLifeToLife_49A20(type_entity_0x6E8E* event);
type_entity_0x6E8E* NewEvent_4A050();
void pre_sub_4A190_0x6E8E(uint32_t adress, type_entity_0x6E8E* a1);//pre 22b190
type_entity_0x6E8E* pre_sub_4A190_axis_3d(uint32_t adress, axis_3d* a1);//pre 22b190
type_entity_0x6E8E* IfSubtypeCallCreatingManaSphere_4A190(axis_3d* a1, int a2, int a3);
void SetMapEntity_57E50(type_entity_0x6E8E* entity);
void sub_57F20(type_entity_0x6E8E* entity);
signed int sub_69250(type_entity_0x6E8E* a1);
