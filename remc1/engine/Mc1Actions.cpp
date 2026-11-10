#include "Mc1Actions.h"

#include "Mc1Entity.h"
#include "Mc1Game.h"
#include "Mc1Shared.h"
#include "Mc1Terrain.h"

// MC1 actions of the classes 2 and up (not the creatures), the same in HW

static Type_Mc1Entity* Entities()
{
	return mc1State_AE400_AE3F0->entities_0x7463_29795;
}

static uint32_t NextRand(Type_Mc1Entity* entity)
{
	entity->rand_0x04_4 = entity->rand_0x04_4 * 0x24A1 + 0x24DF;
	return entity->rand_0x04_4;
}

static void SetGroundZ(Type_Mc1Entity* entity)
{
	entity->position_0x48_72.z = (int16_t)GetTerrainAlt_11F50_11F50((axis_3d*)&entity->position_0x48_72);
}

// class 2

void UpdateClass2Model0Action_49890_49BD0(Type_Mc1Entity* entity)
{
	// the damage of mail 0; destroyed: the model 6 of class 10 for the sender and the action 1; in the water the model 5
	entity->flags_0x10_16 |= 0x20000;
	if (entity->mails_0x5A_90[0].sender_4)
	{
		entity->life_0x0C_12 -= entity->mails_0x5A_90[0].value_0;
		if (entity->life_0x0C_12 < 0)
		{
			Type_Mc1Entity* created = CreateEntity_373F0_377B0(&entity->position_0x48_72, 10, 6);
			if (created)
			{
				created->id_0x18_24 = Entities()[entity->mails_0x5A_90[0].sender_4].id_0x18_24;
				created->word_0x2E_46 = (int16_t)(entity->box_0x4E_78[3] * 3 / 4);
				int32_t life = (int32_t)(NextRand(entity) % 60 + 0x82);
				created->life_0x0C_12 = life;
				entity->life_0x0C_12 = life;
				entity->flags_0x10_16 &= 0xFFFDFFF7;
				entity->actionIndex_0x46_70 = 1;
				entity->flags_0x10_16 |= 0x20000;
				MoveOnMap_41CC0_42000(entity, &entity->position_0x48_72);
			}
		}
		entity->mails_0x5A_90[0].sender_4 = 0;
	}
	SetGroundZ(entity);
	if (GetTileTypeBit_11810_11810(&entity->position_0x48_72) == 1)
	{
		Type_Mc1Entity* created = CreateEntity_373F0_377B0(&entity->position_0x48_72, 10, 5);
		if (created)
			created->id_0x18_24 = entity->id_0x18_24;
		MarkToFree_41E80_421C0(entity);
	}
}

void UpdateClass2Model1Action_49AA0_49DE0(Type_Mc1Entity* entity)
{
	entity->flags_0x10_16 |= 0x20000;
	SetGroundZ(entity);
}

void UpdateClass2Model2Action_49AD0_49E10(Type_Mc1Entity* entity)
{
	// the living players touching it get the flag 1000h
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	for (uint16_t i = 0; i < state->playerCount_0x0A_10; i++)
	{
		Type_Mc1Entity* player = &Entities()[state->players_0x340B_13323[i].entityIndex_0x0A_10];
		if (player->life_0x0C_12 >= 0 && EntitiesOverlap_11950_11950(player, entity))
			player->flags_0x10_16 |= 0x1000;
	}
	SetGroundZ(entity);
}
