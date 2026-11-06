#include "Mc1System.h"

#include <algorithm>
#include <cstdlib>
#include <vector>

#include "Mc1Game.h"

// every block the port allocates, the original left them to DOS at exit
static std::vector<void*> mc1Allocations;

static void* AllocateZeroed(uint32_t size)
{
	void* block = calloc(size ? size : 1, 1);
	if (block)
		mc1Allocations.push_back(block);
	return block;
}

void* Malloc_42540_42880(uint32_t size)
{
	// the original rounds to 4 bytes and tries the low memory first, both are zeroed
	size = (size + 3) & 0xFFFFFFFC;
	return AllocateZeroed(size);
}

void* LowMalloc_425C0_42900(uint32_t size)
{
	// the original rounds to 16 bytes and takes a DOS low memory block
	size = (size + 0x0F) & 0xFFFFFFF0;
	return AllocateZeroed(size);
}

void Free_426E0_42A20(void* block)
{
	if (!block)
		return;
	auto found = std::find(mc1Allocations.begin(), mc1Allocations.end(), block);
	if (found != mc1Allocations.end())
		mc1Allocations.erase(found);
	free(block);
}

void FreeMc1Memory()
{
	for (void* block : mc1Allocations)
		free(block);
	mc1Allocations.clear();
}

void DetectCpu_19470_19470()
{
	// the original reads the CPU and FPU type (192F3, 193EE) and sets the flag for a Pentium with FPU,
	// every machine running the port is at least that
	mc1Settings_AE408_AE3F8->fastCpu_0x08_8 = 1;
}
