#pragma once
#ifndef MAIN_DATTAB_INDEXES
#define MAIN_DATTAB_INDEXES
#include "Basic.h"

void sub_99A77_create_index_dattab_div(uint8_t* a1, uint8_t* a2, uint8_t* a3, bitmap_pos_struct_t* a4);
void sub_99AEB_create_index_dattab_minus(uint8_t* a1, uint8_t* a2, uint8_t* a3, bitmap_pos_struct_t* a4);
void sub_98709_create_index_dattab_power(bitmap_pos_struct2_t* a1, bitmap_pos_struct2_t* a2, uint8_t* a3, bitmap_pos_struct_t* a4);
void sub_98709_create_index_dattab_power_add(uint8_t* a1, uint8_t* a2, uint8_t* a3, bitmap_pos_struct_t* a4, int add);
void sub_9874D_create_index_dattab(bitmap_pos_struct2_t* a1, bitmap_pos_struct2_t* a2, uint8_t* a3, bitmap_pos_struct_t* a4);
void sub_9874D_create_index_dattab_add(uint8_t* a1, uint8_t* a2, uint8_t* a3, bitmap_pos_struct_t* a4, int add);

void CreateIndexes_6EB90(filearray_struct* filearray);

#endif//DATTAB_INDEXES
