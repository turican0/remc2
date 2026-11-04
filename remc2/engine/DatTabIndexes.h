#pragma once
#ifndef MAIN_DATTAB_INDEXES
#define MAIN_DATTAB_INDEXES
#include "Basic.h"

void CreateIndexDatTabHalved_99A77(uint8_t* a1, uint8_t* a2, uint8_t* a3, bitmap_pos_struct_t* a4);
void CreateIndexDatTabRelative_99AEB(uint8_t* a1, uint8_t* a2, uint8_t* a3, bitmap_pos_struct_t* a4);
void CreateIndexDatTabDoubled_98709(bitmap_pos_struct2_t* a1, bitmap_pos_struct2_t* a2, uint8_t* a3, bitmap_pos_struct_t* a4);
void CreateIndexDatTabDoubledWithOffset_98709(uint8_t* a1, uint8_t* a2, uint8_t* a3, bitmap_pos_struct_t* a4, int add);
void CreateIndexDatTab_9874D(bitmap_pos_struct2_t* a1, bitmap_pos_struct2_t* a2, uint8_t* a3, bitmap_pos_struct_t* a4);
void CreateIndexDatTabWithOffset_9874D(uint8_t* a1, uint8_t* a2, uint8_t* a3, bitmap_pos_struct_t* a4, int add);

void CreateIndexes_6EB90(filearray_struct* filearray);

#endif//DATTAB_INDEXES
