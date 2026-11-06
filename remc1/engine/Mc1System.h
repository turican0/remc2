#pragma once

#include <cstdint>

// MC1 memory and machine setup

void* Malloc_42540_42880(uint32_t size);
void* LowMalloc_425C0_42900(uint32_t size);
void FreeMc1Memory();
void DetectCpu_19470_19470();
