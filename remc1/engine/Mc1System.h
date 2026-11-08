#pragma once

#include <cstdint>

// MC1 memory and machine setup

void* Malloc_42540_42880(uint32_t size);
void* LowMalloc_425C0_42900(uint32_t size);
void Free_426E0_42A20(void* block);

extern uint8_t enoughMemory_93950_93950;
extern uint8_t allocationTag_93954_93954;

void ShrinkHeapForMenu_41490_417D0();
void GrowHeapAfterMenu_41540_41880();
void SetAllocationTag_42530_42870(uint8_t tag);
void FreeMc1Memory();
void DetectCpu_19470_19470();
