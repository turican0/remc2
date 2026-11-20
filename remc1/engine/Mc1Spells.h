#pragma once

#include <cstdint>

#include "Mc1GameState.h"

// the spells of class 12 lying in the world

int32_t FallToGround_42090_423D0(Type_Mc1Position* position, int32_t ground, int32_t bottom, int32_t top, int32_t fall); // -1 sunk
uint32_t Distance2DSquared_42410_42750(const Type_Mc1Position* from, const Type_Mc1Position* to);
int32_t SpellPickUp_55A40_55F70(Type_Mc1Entity* entity, int8_t spell, int8_t action);
void SpellPickUpCopy_55D30_56260(Type_Mc1Entity* entity);
void SpellPickUp_55DB0_562E0(Type_Mc1Entity* entity);
int32_t SpellCanCast_55DD0_56300(Type_Mc1Entity* spell, Type_Mc1Entity* wizard);
int32_t SpellUseMana_55E80_563B0(Type_Mc1Entity* spell, Type_Mc1Entity* wizard);
void SpellFromHand_55EF0_56420(Type_Mc1Entity* wizard, Type_Mc1Entity* projectile);
void SpellFireball_56090_565C0(Type_Mc1Entity* spell);
void SpellModel3_56510_56A40(Type_Mc1Entity* spell);
void SpellModel16_57610_57B40(Type_Mc1Entity* spell);
void RaiseByBox_524C0_52800(Type_Mc1Entity* entity);
void CancelCast_46D20_47060(Type_Mc1Entity* projectile, int16_t keep);
int32_t CastlePlaceFree_12F70_12F70(const Type_Mc1Position* position);
void TurnToEntity_52610_52950(Type_Mc1Entity* projectile, Type_Mc1Entity* target);
void ProjectileToPlace_53B50_53E90(Type_Mc1Entity* projectile);
void ProjectileToCastle_53980_53CC0(Type_Mc1Entity* projectile);
void LowerByBox_524E0_52820(Type_Mc1Entity* entity);
void AimAt_52500_52840(Type_Mc1Entity* projectile, Type_Mc1Entity* target);
void HomeTo_52550_52890(Type_Mc1Entity* projectile, Type_Mc1Entity* target);
void CountHit_526C0(Type_Mc1Entity* projectile, Type_Mc1Entity* hit, Type_Mc1Entity* target);
uint32_t AimValue_54A90_54FC0(Type_Mc1Entity* projectile, Type_Mc1Entity* target, uint16_t maxYaw, uint16_t maxPitch);
uint32_t AimValueCastle_54BD0_55100(Type_Mc1Entity* projectile, Type_Mc1Entity* target, uint16_t maxYaw, uint16_t maxPitch);
int32_t FindTarget_54520(Type_Mc1Entity* projectile);
Type_Mc1Entity* FindHit_11980_11980(Type_Mc1Entity* projectile);
Type_Mc1Entity* FindHitMana_11AC0_11AC0(Type_Mc1Entity* projectile);
void Fireball_52B30_52E70(Type_Mc1Entity* projectile);
void Possession_52ED0_53210(Type_Mc1Entity* projectile);
void GuardShot_54180_544D0(Type_Mc1Entity* projectile);
