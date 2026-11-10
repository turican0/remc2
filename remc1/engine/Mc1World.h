#pragma once

#include <cstdint>

#include "Mc1GameState.h"

// MC1 game loop: the frames and the tick of the world

void GameLoop_34610_349D0();
void GameFrame_34530_348F0();
void WorldTick_41780_41AC0();
void UpdateCreatureAlerts_54F00_55430();
int32_t UpdateCreatureAlert_54F80_554B0(Type_Mc1Entity* entity);
void UpdateAttackRecords_16540_16540();
