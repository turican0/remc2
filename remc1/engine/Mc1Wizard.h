#pragma once

#include <cstdint>

#include "Mc1GameState.h"

// MC1 wizard of a player (class 3 action 0, 45C90): the flight by the commands, the spells of the buttons, the damage,
// the mana and the life

void UpdatePlayerWizard_45C90_45FD0(Type_Mc1Entity* wizard);
void UpdateSpellEntities_45C10_45F50(Type_Mc1Entity* wizard);
void ApplyWizardControls_46840_46B80(Type_Mc1Entity* wizard);
void UpdateSpellButton_46B00_46E40(Type_Mc1Entity* wizard, Type_Mc1Entity* spell, uint32_t castFlag, uint32_t button);
int32_t TakeWizardMails_46540_46880(Type_Mc1Entity* wizard);
void MoveWizard_455D0_45910(Type_Mc1Entity* wizard);
int32_t SlideAlongWall_45410_45750(Type_Mc1Entity* wizard);
void SetFlashOfPlayer_44BE0_44F20(Type_Mc1Entity* wizard, uint8_t flash);
void ResetWizardTimer_46520_46860(Type_Mc1Entity* wizard);
uint16_t GetPitchAngle_42180_424C0(Type_Mc1Position* from, Type_Mc1Position* to);
int32_t AngleDistance_42210_42550(int32_t angle, int32_t angle2);
int32_t AngleDirection_42240_42580(int32_t angle, int32_t angle2);
int32_t TurnStep_422A0_425E0(int32_t angle, int32_t angle2, int32_t unused, uint16_t maxStep);
uint32_t Distance3D_42340_42680(Type_Mc1Position* from, Type_Mc1Position* to);
void WizardDying_45FC0_46300(Type_Mc1Entity* wizard);
void LookAtKiller_463B0_466F0(Type_Mc1Entity* wizard);
void WizardDead_46480_467C0(Type_Mc1Entity* wizard);
