#pragma once

#include <cstdint>

#include "Mc1GameState.h"

// the spells of class 12 lying in the world

int32_t FallToGround_42090_423D0(Type_Mc1Position* position, int32_t ground, int32_t bottom, int32_t top, int32_t fall); // -1 sunk
uint32_t Distance2DSquared_42410_42750(const Type_Mc1Position* from, const Type_Mc1Position* to);
int32_t SpellPickUp_55A40_55F70(Type_Mc1Entity* entity, int8_t spell, int8_t action);
void SpellPickUpCopy_55D30_56260(Type_Mc1Entity* entity);
void SpellPickUp_55DB0_562E0(Type_Mc1Entity* entity);
