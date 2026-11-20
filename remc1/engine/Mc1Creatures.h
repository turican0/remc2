#pragma once

#include <cstdint>

#include "Mc1GameState.h"

// the creatures (class 5)

uint32_t SetAction_424F0_42830(Type_Mc1Entity* entity, uint8_t action);
int32_t ClampHeight_42000_42340(Type_Mc1Position* position, int32_t ground, int32_t bottom, int32_t top, int32_t climb);
int32_t GetTileSlope_19650_19650(const Type_Mc1Position* position);
int32_t CheckPosition_11640_11640(Type_Mc1Entity* entity, Type_Mc1Position* position, uint8_t checks);
int32_t MoveCreature_196E0_196E0(Type_Mc1Entity* entity);
void CreatureFollow_19550_19550(Type_Mc1Entity* entity);
void CreatureWander_19D70_19D70(Type_Mc1Entity* entity, uint8_t action);
void CreatureWanderMana_1B200_1B200(Type_Mc1Entity* entity);
void CreatureWander12_1B510_1B510(Type_Mc1Entity* entity);
void CreatureWander48_1EED0_1EED0(Type_Mc1Entity* entity);
void CreatureToBuilding_1F640_1F640(Type_Mc1Entity* entity);
void CreatureToFarBuilding_1FAC0_1FAC0(Type_Mc1Entity* entity);
void SendMail_12B50_12B50(Type_Mc1Entity* from, Type_Mc1Entity* to, uint8_t kind, uint16_t value);
void CreatureFollowLeader_1A390_1A390(Type_Mc1Entity* entity, uint8_t action);
int32_t CreatureAttack_1A120_1A120(Type_Mc1Entity* entity, uint8_t action, int32_t (*attack)(Type_Mc1Entity* entity, Type_Mc1Entity* target));
int32_t CreatureBite_1AB10_1AB10(Type_Mc1Entity* entity, Type_Mc1Entity* target);
void CreatureFollowLeader6_1B320_1B320(Type_Mc1Entity* entity);
void CreatureFollowLeader12_1B570_1B570(Type_Mc1Entity* entity);
void CreatureAttack6_1B2D0_1B2D0(Type_Mc1Entity* entity);
void CreatureFindBuilding_1F390_1F390(Type_Mc1Entity* entity);
void CreatureGoToBuilding_1F120_1F120(Type_Mc1Entity* entity);
int32_t CornerHeightRange_35EA0_36260(uint8_t x, uint8_t y, uint8_t height, uint8_t width);
uint8_t BuildingPlaceFlat_1E920_1E920(const Type_Mc1Position* position, uint16_t width, uint16_t height, uint16_t limit);
void BuildingSize_1E9B0_1E9B0(uint16_t sprite, uint16_t* width, uint16_t* height);
void CreatureBuild_1EA40_1EA40(Type_Mc1Entity* entity);
void CreatureLookForLeader_19B10_19B10(Type_Mc1Entity* entity, uint8_t action);
void CreatureToTarget6_1B160_1B160(Type_Mc1Entity* entity);
void CreatureKilled_1A6C0_1A6C0(Type_Mc1Entity* entity, uint8_t action);
void CreatureInBuilding_1FA00_1FA00(Type_Mc1Entity* entity);
int32_t CreatureFireball_1A8E0_1A8E0(Type_Mc1Entity* entity, Type_Mc1Entity* target);
void CreatureAttack12_1B520_1B520(Type_Mc1Entity* entity);
void CreatureKilled12_1B580_1B580(Type_Mc1Entity* entity);
