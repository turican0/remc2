#pragma once

#include <cstdint>
#include <vector>

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
// port: the blocks allocated (the heap list 1302A0 of the original for the debug texts)
struct Mc1Allocation
{
	void* block;
	uint32_t size;
	uint8_t tag; // allocationTag_93954 (+10h in the blocks of the original)
};
const std::vector<Mc1Allocation>& Mc1Allocations();
void DetectCpu_19470_19470();
