#pragma once

#include "Mc1GameState.h"

// MC1 actions of the scenery (class 10): the terrain changed by the features of the level, the same in HW

void FreeSceneryAction_253E0_253E0(Type_Mc1Entity* entity);
void RaiseTerrainAction_25470_25470(Type_Mc1Entity* entity);
void LowerTerrainOnceAction_25570_25570(Type_Mc1Entity* entity);
void SinkTerrainAction_25670_25670(Type_Mc1Entity* entity);
void RaiseWallYPlusAction_26560_26560(Type_Mc1Entity* entity);
void RaiseWallYMinusAction_26670_26670(Type_Mc1Entity* entity);
void RaiseWallXAction_26760_26760(Type_Mc1Entity* entity);
void ApplyPathAction_26890_26890(Type_Mc1Entity* entity);
void LayTrailAction_26920_26920(Type_Mc1Entity* entity);
void RaiseRidgeAction_269A0_269A0(Type_Mc1Entity* entity);
void BuildBuildingAction_27D30_27D30(Type_Mc1Entity* entity);
int32_t BuildingDamage_29640_29640(Type_Mc1Entity* entity);
Type_Mc1Entity* SpawnCreature_28D10_28D10(Type_Mc1Entity* entity, Type_Mc1Position* position);
void NearestBuilding_44C90_44C90(Type_Mc1Entity* entity);
void BuildingAction_28DC0_28DC0(Type_Mc1Entity* entity);
void TerrainGradient_41F50_41F50(const Type_Mc1Position* position, Type_Mc1Position* gradient);
Type_Mc1Entity* FindOverlapping_11D10_11D10(Type_Mc1Entity* entity);
void MergeMana_277D0_277D0(Type_Mc1Entity* entity, Type_Mc1Entity* other);
void ManaAction_27030_27030(Type_Mc1Entity* entity);
void SmokeAction_25130_25130(Type_Mc1Entity* entity);
void WizardManaAction_275C0_275C0(Type_Mc1Entity* entity);
int32_t NextFrame_42510_42850(Type_Mc1Entity* entity);
void PuffAction_24F60_24F60(Type_Mc1Entity* entity);
void ExplosionAction_25760_25760(Type_Mc1Entity* entity);
void PushManaAction_29920_29960(Type_Mc1Entity* entity);
void NearestFire_44C10_44F50(Type_Mc1Entity* entity);
void FireAction_252D0_252D0(Type_Mc1Entity* entity);
void FireSmokeAction_257B0_257B0(Type_Mc1Entity* entity);
void BuildingFallAction_25410_25410(Type_Mc1Entity* entity);
Type_Mc1Entity* FindOverlappingMana_11C00_11C00(Type_Mc1Entity* entity);
int32_t EntityNear_11E50_11E50(Type_Mc1Position* position, uint8_t entityClass, uint8_t model);
void LifeAction_252B0_252B0(Type_Mc1Entity* entity);
void LifeAction_253F0_253F0(Type_Mc1Entity* entity);
void RisingSmokeAction_258A0_258A0(Type_Mc1Entity* entity);
void WaterTrailAction_25990_25990(Type_Mc1Entity* entity);
void BouncingRockAction_25A60_25A60(Type_Mc1Entity* entity);
void FireRingAction_25CE0_25CE0(Type_Mc1Entity* entity);
void VolcanoAction_25EC0_25EC0(Type_Mc1Entity* entity);
void FireStormAction_26140_26140(Type_Mc1Entity* entity);
void BlastAction_262D0_262D0(Type_Mc1Entity* entity);
void MailOnceAction_26360_26360(Type_Mc1Entity* entity);
void MailAction_263C0_263C0(Type_Mc1Entity* entity);
void TeleportAction_26A60_26A60(Type_Mc1Entity* entity);
void CircleOwnerAction_26C00_26C00(Type_Mc1Entity* entity);
void FlagLifeAction_26CE0_26CE0(Type_Mc1Entity* entity);
void RisingSplitAction_26D20_26D20(Type_Mc1Entity* entity);
void CreaturesFromManaAction_26E90_26E90(Type_Mc1Entity* entity);
void SlideAction_29540_29540(Type_Mc1Entity* entity);
void EggAction_296A0_296A0(Type_Mc1Entity* entity);
void HatchAction_29700_29700(Type_Mc1Entity* entity);
void FireWallAction_29780(Type_Mc1Entity* entity);
void FireWallActionHw_29780(Type_Mc1Entity* entity);
void ShockwaveAction_299D0_29A10(Type_Mc1Entity* entity);
