#pragma once

#include <cstdint>

#include "Mc1GameState.h"

// MC1 the commands of the players in a frame (MC2 PlayerEvents_51BB0), the movie input, the wizard of a player

extern uint8_t networkJoined_93959_93959;
extern int16_t networkIndex_9395A_9395A; // the index of this computer
extern int16_t networkCount_9395C_9395C;
extern uint16_t networkMaster_9398A_9398A; // the computer which sends the commands of all
extern uint32_t networkHandles_93990_93990[8];
extern uint8_t networkActive_939B0_939B0[8];

void PlayerEvents_3C9D0_3CD10();
void SetPlayerMode_3DC90_3DFD0(Type_Mc1Player_340B* player, uint8_t mode);
void CheckCd_3DD10_3E050();
void RecordOrPlayMovieInput_3E9D0_3ED10(Type_Mc1PlayerInput* input);
void CloseMovie_3EC50_3EF90();
void KillAllCreatures_194F0_194F0();
void RemoveNetworkPlayer_43760_43AA0(int16_t player);
void ExchangeAll_43930_43C70(uint8_t* buffer, uint32_t size);
void CreatePlayerWizard_44D30_45070(Type_Mc1Player_340B* player, Type_Mc1Entity* wizard);
void UpdateSpellSlots_45C10_45F50(Type_Mc1Entity* wizard);
void SetCastleLifeAndMana_47BD0_47F10(Type_Mc1Entity* castle, Type_Mc1Entity* manaEntity, int32_t life, int32_t mana);
void GrowCastle_47C60_47FA0(Type_Mc1Entity* castle);
void SetCastleMana_47DD0_48110(Type_Mc1Entity* castle);
void StampCastle_279D0_279D0(Type_Mc1Entity* entity);
void SetCenterMouse_59720_59C30();
void SetMousePosition_59750_59C60(int16_t x, int16_t y);
