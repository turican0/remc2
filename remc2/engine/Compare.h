#pragma once
#include "../engine/Basic.h"

#ifndef F_OK
#define F_OK 00
#endif /* F_OK */

int getcompindex(uint32_t adress);
void SaveCompare(char* name, int value, int len = -1, uint8_t* sequence = nullptr);
void CompareWith(char* name, int value, int len = -1, uint8_t* sequence = nullptr);

