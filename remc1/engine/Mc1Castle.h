#pragma once

#include <cstdint>

#include "Mc1GameState.h"

// the castle of a wizard (class 3 model 2)

uint32_t Distance3DSquared_42390_426D0(const Type_Mc1Position* from, const Type_Mc1Position* to);
Type_Mc1Entity* NearestOwnMana_46CA0_46FE0(Type_Mc1Entity* entity, Type_Mc1Entity* except, Type_Mc1Entity* except2);
void DropMana_27690_27690(Type_Mc1Entity* entity);
int32_t CastleDamage_47EC0_48200(Type_Mc1Entity* castle);
void CastleOverflow_47130_47470(Type_Mc1Entity* castle);
void CastleBalloonsAndGuards_47400_47740(Type_Mc1Entity* castle);
void CastleAction_46DB0_470F0(Type_Mc1Entity* castle);
void ClearBuildingsAround_12C50_12C50(Type_Mc1Entity* castle);
uint8_t CastleCanGrow_12D10_12D10(Type_Mc1Entity* castle);
void CastleEffect2A_47020_47360(Type_Mc1Entity* castle);
void CastleEffect29_47080_473C0(Type_Mc1Entity* castle);
void CastleGrown_47960_47CA0(Type_Mc1Entity* castle);
void CastleGrowAction_46F10_47250(Type_Mc1Entity* castle);
void DestroyAtTile_40E20_41160(uint16_t tile, uint16_t id);
void CastleRaise_285C0_285C0(Type_Mc1Entity* effect);
void CastleFlatten_28200_28200(Type_Mc1Entity* effect);
void CastleGrowMail_293D0_293D0(Type_Mc1Entity* effect);
int32_t BalloonDamage_481D0_48510(Type_Mc1Entity* balloon);
void BalloonAction_47F90_482D0(Type_Mc1Entity* balloon);
