#pragma once

#include <cstdint>

#include "Mc1GameState.h"

// the classes of the entities: per class the actions (by actionIndex_0x46) and the models (create functions)

#pragma pack(push, 1)
typedef struct { // 14 bytes in DOS
	const char* name_0;
	uint16_t index_4;
	void (*action_6)(Type_Mc1Entity* entity);
	uint32_t flag_10; // 0 not used
} Type_Mc1ClassAction;

typedef struct { // 14 bytes in DOS
	const char* name_0;
	uint16_t index_4; // the model
	Type_Mc1Entity* (*create_6)(Type_Mc1Position* position);
	uint32_t flag_10; // 0 not used
} Type_Mc1ClassModel;

typedef struct { // 18 bytes in DOS
	Type_Mc1ClassAction* actions_0;
	Type_Mc1ClassModel* models_4;
	const char* name_8;
	uint16_t index_12;
	uint32_t unknown_14;
} Type_Mc1Class;
#pragma pack(pop)

extern Type_Mc1Class classes_96902_96902[14];

void EntityNoAction(Type_Mc1Entity* entity);
Type_Mc1Entity* EntityNoModel(Type_Mc1Position* position);
void PatchClassTablesHw(); // port: HIDDEN.EXE has other functions in some entries
