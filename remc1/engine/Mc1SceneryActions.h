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
