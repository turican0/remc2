#include "Mc1Wizard.h"

#include <cstring>

#include "Mc1Entity.h"
#include "Mc1Game.h"
#include "Mc1NotPorted.h"
#include "Mc1Sound.h"
#include "Mc1PlayerEvents.h"
#include "Mc1Shared.h"
#include "Mc1Terrain.h"

// 93A88: the controls of the flight, the speed (step, min, max, the slowing of a hit) and the turn
static const int32_t speedStep_93A88_93A88 = 0x10;
static const int32_t speedMin_93A8C_93A8C = -0x50;
static const int32_t speedMax_93A90_93A90 = 0x50;
static const int32_t hitSlowing_93A94_93A94 = -4;
static const int16_t turnStep_93AA8_93AA8 = 0x10;
static const int32_t turnMin_93AAC_93AAC = -0x50;
static const int32_t turnMax_93AB0_93AB0 = 0x50;
static const int16_t turnSlowing_93AB4_93AB4 = -4;

static Type_Mc1Entity* Entities()
{
	return mc1Entities;
}

static int16_t EntityIndex(const Type_Mc1Entity* entity)
{
	return (int16_t)(entity - Entities());
}

static int16_t ReadS16(const uint8_t* data)
{
	int16_t value;
	memcpy(&value, data, sizeof(value));
	return value;
}

static int32_t ReadS32(const uint8_t* data)
{
	int32_t value;
	memcpy(&value, data, sizeof(value));
	return value;
}

static void WriteS16(uint8_t* data, int32_t value)
{
	const int16_t value16 = (int16_t)value;
	memcpy(data, &value16, sizeof(value16));
}

static void WriteS32(uint8_t* data, int32_t value)
{
	memcpy(data, &value, sizeof(value));
}

static int32_t Sign(int32_t value)
{
	return value ? value / (value < 0 ? -value : value) : 0;
}

static axis_3d* Axis(Type_Mc1Position* position)
{
	return (axis_3d*)position;
}

uint16_t GetPitchAngle_42180_424C0(Type_Mc1Position* from, Type_Mc1Position* to)
{
	const int16_t distance = (int16_t)-(int32_t)Distance2D_423D0_42710(from, to);
	return Atan2_40F87_412C7((int16_t)(from->z - to->z), distance);
}

int32_t AngleDistance_42210_42550(int32_t angle, int32_t angle2)
{
	int32_t distance = (angle & 0x7FF) - (angle2 & 0x7FF);
	if (distance < 0)
		distance = -distance;
	if ((uint16_t)distance > 0x400)
		distance = 0x800 - distance;
	return distance;
}

int32_t AngleDirection_42240_42580(int32_t angle, int32_t angle2)
{
	int32_t difference = (angle2 & 0x7FF) - (angle & 0x7FF);
	if (!difference)
		return angle & 0xFFFF0000;
	if ((difference < 0 ? -difference : difference) > 0x400)
		difference += difference < 0 ? 0x800 : -0x800;
	return Sign(difference);
}

int32_t TurnStep_422A0_425E0(int32_t angle, int32_t angle2, int32_t /*unused*/, uint16_t maxStep)
{
	if ((int16_t)angle == (int16_t)angle2)
		return 0;
	int32_t step = AngleDistance_42210_42550((uint16_t)angle, (uint16_t)angle2);
	const int32_t direction = AngleDirection_42240_42580((uint16_t)angle, (uint16_t)angle2);
	if ((int16_t)step > maxStep)
		step = maxStep;
	return step * direction;
}

uint32_t Distance3D_42340_42680(Type_Mc1Position* from, Type_Mc1Position* to)
{
	const int32_t dy = (int16_t)(to->y - from->y);
	const int32_t dx = (int16_t)(to->x - from->x);
	const int32_t dz = (int16_t)(to->z - from->z);
	return IntegerSqrt_410CE_4140E(dy * dy + dx * dx + dz * dz);
}

void SetFlashOfPlayer_44BE0_44F20(Type_Mc1Entity* wizard, uint8_t flash)
{
	if (mc1State_AE400_AE3F0->currentPlayer_0x08_8 == ReadS16(wizard->playerData_0xA0_160 + 0x30))
		mc1Settings_AE408_AE3F8->paletteSubMod_0x98_152 = flash;
}

void ResetWizardTimer_46520_46860(Type_Mc1Entity* wizard)
{
	if (wizard->playerData_0xA0_160)
		WriteS16(wizard->playerData_0xA0_160 + 0x2E, 100);
}

void UpdateSpellEntities_45C10_45F50(Type_Mc1Entity* wizard)
{
	// playerData + 2A4h: the entity of each spell by its model, from the slots (+ 214h)
	uint8_t* playerData = wizard->playerData_0xA0_160;
	memset(playerData + 0x2A4, 0, 0x30);
	for (int32_t slot = 0; slot < 24; slot++)
	{
		const int32_t index = ReadS32(playerData + 0x214 + slot * 4);
		if (index)
			WriteS16(playerData + 0x2A4 + (int8_t)Entities()[index].model_0x41_65 * 2, index);
	}
}

void UpdateSpellButton_46B00_46E40(Type_Mc1Entity* wizard, Type_Mc1Entity* spell, uint32_t castFlag, uint32_t button)
{
	if (spell <= Entities())
		return;
	uint8_t* playerData = wizard->playerData_0xA0_160;
	bool cast = false;
	if (ReadS32(playerData) & button)
	{
		bool charge = true;
		switch (spell->model_0x41_65)
		{
		case 0x10:
			if (spell->word_0x30_48)
			{
				QueueSound_55370_558A0(0, ReadS16(playerData + 0x30), 0x1D);
				return;
			}
			if (wizard->mana_0x8C_140 < spell->dword_0x88_136)
				return;
			cast = true;
			charge = false;
			break;
		case 2:
		case 0x15:
		{
			// the spell of the model 21h / 2 stops the other one
			Type_Mc1Entity& other = Entities()[ReadS16(playerData + (spell->model_0x41_65 == 2 ? 0x2CE : 0x2A8))];
			if (other.word_0x30_48)
				other.word_0x30_48 = 0;
			if (wizard->mana_0x8C_140 < spell->dword_0x88_136)
				return;
			break;
		}
		}
		if (charge)
		{
			if (spell->byte_0x3E_62)
			{
				// a charged spell: one more charge up to the mana and byte_0x3E
				spell->byte_0x3D_61++;
				int16_t charges = (int16_t)(wizard->mana_0x8C_140 / spell->dword_0x88_136);
				if (charges > (int8_t)spell->byte_0x3E_62)
					charges = (int8_t)spell->byte_0x3E_62;
				if ((int8_t)spell->byte_0x3D_61 < 0)
					spell->byte_0x3D_61 = 0;
				if ((int8_t)spell->byte_0x3D_61 > charges)
					spell->byte_0x3D_61 = (uint8_t)charges;
				wizard->flags_0x10_16 &= ~0x300u;
				wizard->flags_0x10_16 |= castFlag;
				return;
			}
			if (wizard->mana_0x8C_140 < spell->dword_0x88_136)
				return;
			cast = true;
		}
	}
	else if (spell->byte_0x3D_61 && (wizard->flags_0x10_16 & castFlag))
		cast = true;
	if (!cast)
		return;
	spell->word_0x30_48 = spell->word_0x32_50;
	wizard->flags_0x10_16 &= ~0x300u;
	wizard->flags_0x10_16 |= castFlag;
	wizard->flags_0x10_16 &= ~0x20u;
}

void ApplyWizardControls_46840_46B80(Type_Mc1Entity* wizard)
{
	// playerData + 0: the buttons of the command 6 (1 forwards, 2 backwards, 4 left, 8 right, 10h / 20h the spells),
	// + 0Ch the speed, + 0Eh accelerating, + 10h the sideways speed
	uint8_t* playerData = wizard->playerData_0xA0_160;
	const int32_t buttons = ReadS32(playerData);
	if (buttons == 0x30)
	{
		// both spells: the castle is destroyed
		if (const uint16_t castle = (uint16_t)ReadS16(playerData + 0x32))
			Entities()[castle].life_0x0C_12 = -1;
		return;
	}
	int16_t accelerate = 0;
	WriteS16(playerData + 0x0E, 0);
	if ((buttons & 1) && ReadS16(playerData + 0x0C) < speedMax_93A90_93A90)
		accelerate = 1;
	if ((ReadS32(playerData) & 2) && ReadS16(playerData + 0x0C) > speedMin_93A8C_93A8C)
		accelerate = -1;
	if (accelerate)
	{
		WriteS16(playerData + 0x0C, ReadS16(playerData + 0x0C) + (int16_t)(accelerate * (int16_t)speedStep_93A88_93A88));
		if (ReadS16(playerData + 0x0C) < speedMin_93A8C_93A8C)
			WriteS16(playerData + 0x0C, speedMin_93A8C_93A8C);
		if (ReadS16(playerData + 0x0C) > speedMax_93A90_93A90)
			WriteS16(playerData + 0x0C, speedMax_93A90_93A90);
		WriteS16(playerData + 0x0E, 1);
	}
	int16_t turn = 0;
	if (ReadS32(playerData) & 4)
		turn = -1;
	if (ReadS32(playerData) & 8)
		turn = 1;
	if (!turn)
	{
		// the sideways speed slows down to 0
		const int32_t sign = Sign(ReadS16(playerData + 0x10));
		WriteS16(playerData + 0x10, ReadS16(playerData + 0x10) + (int16_t)(turnSlowing_93AB4_93AB4 * sign));
		if ((int16_t)sign != Sign(ReadS16(playerData + 0x10)))
			WriteS16(playerData + 0x10, 0);
	}
	else
		WriteS16(playerData + 0x10, ReadS16(playerData + 0x10) + (int16_t)(turn * turnStep_93AA8_93AA8));
	if (ReadS16(playerData + 0x10) < turnMin_93AAC_93AAC)
		WriteS16(playerData + 0x10, turnMin_93AAC_93AAC);
	if (ReadS16(playerData + 0x10) > turnMax_93AB0_93AB0)
		WriteS16(playerData + 0x10, turnMax_93AB0_93AB0);
	UpdateSpellButton_46B00_46E40(wizard, &Entities()[ReadS32(playerData + 0x214 + ReadS16(playerData + 0x3AC) * 4)], 0x100, 0x10);
	UpdateSpellButton_46B00_46E40(wizard, &Entities()[ReadS32(playerData + 0x214 + ReadS16(playerData + 0x3B0) * 4)], 0x200, 0x20);
}

int32_t TakeWizardMails_46540_46880(Type_Mc1Entity* wizard)
{
	uint8_t* playerData = wizard->playerData_0xA0_160;
	if (wizard->life_0x0C_12 < 0)
		return 2;
	int32_t result = 0;
	if (const uint16_t sender = wizard->mails_0x5A_90[4].sender_4)
	{
		// the mail 4: the sender wizard is pulled to this one (its playerData + 13Ah)
		Type_Mc1Entity& puller = Entities()[sender];
		WriteS16(puller.playerData_0xA0_160 + 0x13A, EntityIndex(wizard));
		WriteS16(puller.playerData_0xA0_160 + 0x13C, 0xC8);
		WriteS32(puller.playerData_0xA0_160 + 0x13E, Distance3D_42340_42680(&wizard->position_0x48_72, &puller.position_0x48_72));
		if (ReadS32(puller.playerData_0xA0_160 + 0x13E) < 0x400)
			WriteS32(puller.playerData_0xA0_160 + 0x13E, 0x400);
		if (ReadS32(puller.playerData_0xA0_160 + 0x13E) > 0xC00)
			WriteS32(puller.playerData_0xA0_160 + 0x13E, 0xC00);
		wizard->mails_0x5A_90[4].sender_4 = 0;
		playerData[0x188] = 4;
		WriteS32(playerData + 0x17F, 0x10);
		ResetWizardTimer_46520_46860(wizard);
	}
	if (const uint16_t sender = wizard->mails_0x5A_90[3].sender_4)
	{
		// the mail 3: the mana taken, to the sender wizard
		Type_Mc1Entity& taker = Entities()[sender];
		if (taker.class_0x40_64 == 3)
			taker.mana_0x8C_140 += wizard->mails_0x5A_90[3].value_0;
		wizard->mana_0x8C_140 -= wizard->mails_0x5A_90[3].value_0;
		playerData[0x188] = 4;
		WriteS32(playerData + 0x17F, 0x10);
		ResetWizardTimer_46520_46860(wizard);
		wizard->mails_0x5A_90[3].sender_4 = 0;
	}
	if (wizard->mails_0x5A_90[0].sender_4)
	{
		// the mail 0: the damage, a quarter from the mana with the flag 4000h, and the knock from the sender
		if (wizard->flags_0x10_16 & 0x4000)
		{
			const int32_t mana = wizard->mails_0x5A_90[0].value_0 / 4;
			wizard->mana_0x8C_140 -= mana;
			wizard->mails_0x5A_90[0].value_0 = mana;
			wizard->flags_0x10_16 &= ~0x4000u;
		}
		wizard->life_0x0C_12 -= wizard->mails_0x5A_90[0].value_0;
		Type_Mc1Entity* sender = &Entities()[wizard->mails_0x5A_90[0].sender_4];
		if (sender > Entities())
		{
			WriteS16(playerData + 0x18, GetYawAngle_42150_42490(Axis(&sender->position_0x48_72), Axis(&wizard->position_0x48_72)));
			WriteS16(playerData + 0x1A, GetPitchAngle_42180_424C0(&sender->position_0x48_72, &wizard->position_0x48_72));
			WriteS16(playerData + 0x16, wizard->mails_0x5A_90[0].value_0 / 10);
			if (ReadS16(playerData + 0x16) < 0)
				WriteS16(playerData + 0x16, 0);
			if (ReadS16(playerData + 0x16) > 0x50)
				WriteS16(playerData + 0x16, 0x50);
		}
		SetFlashOfPlayer_44BE0_44F20(wizard, 2);
		playerData[0x188] = 4;
		WriteS32(playerData + 0x17F, 0x10);
		QueueSound_55370_558A0(EntityIndex(wizard), -1, 0x11);
		if (wizard->life_0x0C_12 < 0)
		{
			wizard->word_0x26_38 = wizard->mails_0x5A_90[0].sender_4;
			return 2;
		}
		result = 1;
		ResetWizardTimer_46520_46860(wizard);
		wizard->mails_0x5A_90[0].sender_4 = 0;
	}
	return result;
}

int32_t SlideAlongWall_45410_45750(Type_Mc1Entity* wizard)
{
	// into a wall (tile bit 100h): along the wall by the nearer of the two right angles, else no move
	int32_t move = 1;
	Type_Mc1Position* position = &wizard->position_0x48_72;
	if (GetTileTypeBit_11810_11810(&featurePosition_AE454_AE444) == 0x100)
	{
		const uint16_t yaw = (uint16_t)GetYawAngle_42150_42490(Axis(position), Axis(&featurePosition_AE454_AE444));
		const uint16_t pitch = GetPitchAngle_42180_424C0(position, &featurePosition_AE454_AE444);
		const uint32_t distance = (uint16_t)Distance3D_42340_42680(position, &featurePosition_AE454_AE444);
		const int32_t sector = yaw / 512;
		featurePosition_AE454_AE444 = *position;
		const uint16_t angle = (uint16_t)(sector << 9);
		const uint16_t angle2 = (uint16_t)(((sector + 1) << 9) & 0x7FF);
		MoveEntity_41EC0_42200(Axis(&featurePosition_AE454_AE444), angle, pitch,
			(int16_t)((0x200 - (uint16_t)AngleDistance_42210_42550(yaw, angle)) * (int32_t)distance / 512));
		if (GetTileTypeBit_11810_11810(&featurePosition_AE454_AE444) == 0x100)
		{
			featurePosition_AE454_AE444 = *position;
			MoveEntity_41EC0_42200(Axis(&featurePosition_AE454_AE444), angle2, pitch,
				(int16_t)((0x200 - (uint16_t)AngleDistance_42210_42550(yaw, angle2)) * (int32_t)distance / 512));
			if (GetTileTypeBit_11810_11810(&featurePosition_AE454_AE444) == 0x100)
				move = 0;
		}
	}
	// not under the ground (typeParams + 0Ch the height)
	const int16_t height = ReadS16(wizard->typeParams_0x9C_156 + 0x0C);
	const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50(Axis(&featurePosition_AE454_AE444));
	if (featurePosition_AE454_AE444.z < ground + height)
		featurePosition_AE454_AE444.z = (int16_t)(ground + height);
	return move;
}

void MoveWizard_455D0_45910(Type_Mc1Entity* wizard)
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	uint8_t* playerData = wizard->playerData_0xA0_160;
	featurePosition_AE454_AE444 = wizard->position_0x48_72;
	// the turn: playerData + 4 / 6 the change of the yaw / pitch from the mouse, summed in + 147h / + 149h
	WriteS16(playerData + 0x147, ReadS16(playerData + 0x147) + ReadS16(playerData + 4));
	WriteS16(playerData + 0x149, ReadS16(playerData + 0x149) + ReadS16(playerData + 6));
	wizard->word_0x1E_30 = (int16_t)((ReadS16(playerData + 0x147) / 8 + (uint16_t)wizard->word_0x1E_30) & 0x7FF);
	// the speed (word_0x7E) towards playerData + 0Ch
	wizard->word_0x7E_126 = (int16_t)(wizard->word_0x7E_126 + speedStep_93A88_93A88 * Sign(ReadS16(playerData + 0x0C) - wizard->word_0x7E_126));
	const int16_t ground = (int16_t)GetTerrainAlt_11F50_11F50(Axis(&featurePosition_AE454_AE444));
	const int32_t flyHeight = ReadS16(wizard->typeParams_0x9C_156 + 0x0A);
	int32_t climb = ((featurePosition_AE454_AE444.z - ground - flyHeight) << 10) / flyHeight;
	if (climb < -0x100)
		climb = -0x100;
	if (climb > 0x100)
		climb = 0x100;
	// the pitch of the flight: up / down by the mouse, levelled by the height over the ground
	int16_t pitch = (int16_t)(ReadS16(playerData + 0x149) & 0x7FF);
	wizard->word_0x20_32 = pitch;
	if (pitch > 0x400)
		pitch = (int16_t)-(0x800 - pitch);
	if (wizard->word_0x7E_126 < 0 && pitch > 0)
		WriteS16(playerData + 0x1C, -climb * pitch / 256);
	else if (wizard->word_0x7E_126 < 0 && pitch < 0)
		WriteS16(playerData + 0x1C, ReadS16(playerData + 0x149));
	else if (wizard->word_0x7E_126 > 0 && pitch < 0)
		WriteS16(playerData + 0x1C, -climb * pitch / 256);
	else if (wizard->word_0x7E_126 > 0 && pitch > 0)
		WriteS16(playerData + 0x1C, ReadS16(playerData + 0x149));
	else if (!wizard->word_0x7E_126 && featurePosition_AE454_AE444.z > ground + flyHeight)
		featurePosition_AE454_AE444.z -= 8;
	WriteS16(playerData + 0x1C, ReadS16(playerData + 0x1C) & 0x7FF);
	MoveEntity_41EC0_42200(Axis(&featurePosition_AE454_AE444), (uint16_t)wizard->word_0x1E_30, ReadS16(playerData + 0x1C), wizard->word_0x7E_126);
	// sideways
	if (const int16_t sideways = ReadS16(playerData + 0x10))
		MoveEntity_41EC0_42200(Axis(&featurePosition_AE454_AE444), (uint16_t)(wizard->word_0x1E_30 + 0x200), 0, sideways);
	// the knock of a hit (+ 16h the speed, + 18h the yaw), slowing down
	if (ReadS16(playerData + 0x16))
	{
		if (ReadS16(playerData + 0x16) > 0x80)
			WriteS16(playerData + 0x16, 0x80);
		MoveEntity_41EC0_42200(Axis(&featurePosition_AE454_AE444), (uint16_t)ReadS16(playerData + 0x18), 0, ReadS16(playerData + 0x16));
		WriteS16(playerData + 0x16, ReadS16(playerData + 0x16) + hitSlowing_93A94_93A94 * Sign(ReadS16(playerData + 0x16)));
		const int16_t knock = ReadS16(playerData + 0x16);
		if ((knock < 0 ? -knock : knock) < 4)
			WriteS16(playerData + 0x16, 0);
	}
	// + 198h: a move from outside (once)
	featurePosition_AE454_AE444.x += ReadS16(playerData + 0x198);
	featurePosition_AE454_AE444.y += ReadS16(playerData + 0x19A);
	featurePosition_AE454_AE444.z += ReadS16(playerData + 0x19C);
	WriteS16(playerData + 0x198, 0);
	WriteS16(playerData + 0x19A, 0);
	WriteS16(playerData + 0x19C, 0);
	// pulled to a wizard (+ 13Ah, the mail 4): to the distance + 13Eh, up to 1000 frames
	Type_Mc1Entity* puller = &Entities()[(uint16_t)ReadS16(playerData + 0x13A)];
	if (puller > Entities())
	{
		const int32_t distance = Distance3D_42340_42680(&wizard->position_0x48_72, &puller->position_0x48_72);
		if (distance >= 0x1400 || puller->life_0x0C_12 < 0 || ReadS16(playerData + 0x13C) == 1000)
		{
			WriteS16(playerData + 0x13A, 0);
			WriteS16(playerData + 0x13C, 0);
		}
		const int32_t limit = wizard->word_0x80_128 * 3 / 2;
		int32_t speed = (distance - ReadS32(playerData + 0x13E)) / (0x400 / (int16_t)limit);
		if ((int16_t)speed < -(int16_t)limit)
			speed = -limit;
		if ((int16_t)speed > (int16_t)limit)
			speed = limit;
		const uint16_t yaw = (uint16_t)GetYawAngle_42150_42490(Axis(&wizard->position_0x48_72), Axis(&puller->position_0x48_72));
		wizard->word_0x1E_30 = (int16_t)((wizard->word_0x1E_30 + TurnStep_422A0_425E0((uint16_t)wizard->word_0x1E_30, yaw, 5, 0x82)) & 0x7FF);
		MoveEntity_41EC0_42200(Axis(&featurePosition_AE454_AE444), yaw, wizard->word_0x20_32, (int16_t)speed);
		WriteS16(playerData + 0x13C, ReadS16(playerData + 0x13C) + 1);
	}
	if (SlideAlongWall_45410_45750(wizard))
		CopyEntityPosition_41C70_41FB0(wizard, &featurePosition_AE454_AE444);
	// the sounds of the flight: over the water or the land, the wind (+ 18Ch, + 190h)
	const int16_t player = state->currentPlayer_0x08_8;
	if (GetTileTypeBit_11810_11810(&wizard->position_0x48_72) == 1)
	{
		QueueSound_55370_558A0(0, player, 1);
		FadeOutSound_55890_55DC0(0, player, 2);
	}
	else
	{
		QueueSound_55370_558A0(0, player, 2);
		FadeOutSound_55890_55DC0(0, player, 1);
	}
	if (ReadS32(playerData + 0x18C) < 0x600)
	{
		QueueSound_55370_558A0(0, player, 5);
		WriteS32(playerData + 0x18C, 0x800);
	}
	else
		FadeOutSound_55890_55DC0(0, player, 5);
	if (ReadS32(playerData + 0x190) < 0x600)
	{
		QueueSound_55370_558A0(0, player, 0x1F);
		WriteS32(playerData + 0x190, 0x800);
	}
	else
		FadeOutSound_55890_55DC0(0, player, 0x1F);
	if (ReadS16(playerData + 0x30) == state->currentPlayer_0x08_8)
	{
		const int16_t timer = ReadS16(playerData + 0x2E);
		if (timer > 0)
		{
			WriteS16(playerData + 0x2E, timer - 1);
			SetDangerMusic_20D00_20D00(2);
		}
		else
			SetDangerMusic_20D00_20D00(1);
	}
	if (!(wizard->index_0x3F_63 & 0x3F))
	{
		wizard->rand_0x04_4 = wizard->rand_0x04_4 * 0x24A1 + 0x24DF;
		if (!(wizard->rand_0x04_4 % 11))
			QueueSound_55370_558A0(EntityIndex(wizard), -1, 0x2E);
	}
}

void UpdatePlayerWizard_45C90_45FD0(Type_Mc1Entity* wizard)
{
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	uint8_t* playerData = wizard->playerData_0xA0_160;
	bool inCastle = false;
	if (!(settings->flags_0x00_0 & 0x10000))
		UpdateSpellEntities_45C10_45F50(wizard);
	wizard->word_0x80_128 = (int16_t)speedMax_93A90_93A90;
	if (!(settings->flags_0x00_0 & 0x10000))
	{
		if (const uint16_t castle = (uint16_t)ReadS16(playerData + 0x32))
			if (EntitiesOverlap_11950_11950(wizard, &Entities()[castle]))
				inCastle = true;
	}
	ApplyWizardControls_46840_46B80(wizard);
	if (!(settings->flags_0x00_0 & 0x10000) && inCastle)
	{
		// in the castle: the damage goes to the castle
		if (wizard->mails_0x5A_90[0].sender_4)
		{
			Type_Mc1Entity& castle = Entities()[(uint16_t)ReadS16(playerData + 0x32)];
			if (castle.mails_0x5A_90[0].sender_4)
				castle.mails_0x5A_90[0].value_0 += wizard->mails_0x5A_90[0].value_0;
			else
				castle.mails_0x5A_90[0].value_0 = wizard->mails_0x5A_90[0].value_0;
			castle.mails_0x5A_90[0].sender_4 = wizard->mails_0x5A_90[0].sender_4;
		}
		WriteS16(playerData + 0x14B, 2);
	}
	if (!(settings->flags_0x00_0 & 0x10000))
	{
		if (!ReadS16(playerData + 0x14B))
			TakeWizardMails_46540_46880(wizard);
		else
		{
			memset(wizard->mails_0x5A_90, 0, sizeof(wizard->mails_0x5A_90));
			WriteS16(playerData + 0x14B, ReadS16(playerData + 0x14B) - 1);
		}
		if (playerData[0x146] < 0xC8)
			playerData[0x146]++;
	}
	MoveWizard_455D0_45910(wizard);
	if (wizard->life_0x0C_12 < 0)
	{
		// dead: the action 2
		wizard->actionIndex_0x46_70 = 2;
		wizard->word_0x2E_46 = 0;
		QueueSound_55370_558A0(EntityIndex(wizard), -1, 0x10);
		return;
	}
	if (settings->flags_0x00_0 & 0x10000)
		return;
	// the mana and the life grow (+ 155h the life, not while + 17Fh counts down after a hit)
	wizard->mana_0x8C_140 += wizard->dword_0x84_132;
	if (const int32_t wait = ReadS32(playerData + 0x17F))
		WriteS32(playerData + 0x17F, wait - 1);
	else
		wizard->life_0x0C_12 += ReadS16(playerData + 0x155);
	if (wizard->mana_0x8C_140 < 0)
		wizard->mana_0x8C_140 = 0;
	if (wizard->mana_0x8C_140 > wizard->dword_0x88_136)
		wizard->mana_0x8C_140 = wizard->dword_0x88_136;
	if (wizard->life_0x0C_12 < -1)
		wizard->life_0x0C_12 = -1;
	if (wizard->life_0x0C_12 > wizard->maxLife_0x08_8)
		wizard->life_0x0C_12 = wizard->maxLife_0x08_8;
	if (const int32_t timer = ReadS32(playerData + 0x15F))
		WriteS32(playerData + 0x15F, timer - 1);
	if (const int16_t timer = ReadS16(playerData + 0x210))
		WriteS16(playerData + 0x210, timer - 1);
	if (inCastle || (wizard->flags_0x10_16 & 0x1000))
	{
		// in the castle (or the flag 1000h once) fast
		wizard->dword_0x84_132 = wizard->dword_0x88_136 / 200;
		WriteS16(playerData + 0x155, wizard->maxLife_0x08_8 / 250);
		if (wizard->dword_0x84_132 < 1000)
			wizard->dword_0x84_132 = 1000;
		wizard->flags_0x10_16 &= ~0x1000u;
		return;
	}
	wizard->dword_0x84_132 = wizard->dword_0x88_136 / 2000;
	WriteS16(playerData + 0x155, wizard->maxLife_0x08_8 / 2000);
	if (wizard->dword_0x84_132 < 100)
		wizard->dword_0x84_132 = 100;
}

void WizardDying_45FC0_46300(Type_Mc1Entity* wizard)
{
	// the wizard falls to the ground, there the spells fall out, a mana ball takes its mana and the action 3 waits
	MoveWizard_455D0_45910(wizard);
	if (mc1Settings_AE408_AE3F8->colorIndex_0x5D_93[6])
		SetFlashOfPlayer_44BE0_44F20(wizard, 7);
	wizard->position_0x48_72.z = (int16_t)(wizard->position_0x48_72.z + wizard->word_0x2E_46);
	wizard->word_0x2E_46 = (int16_t)(wizard->word_0x2E_46 - 2);
	if (wizard->word_0x2E_46 < -0x100)
		wizard->word_0x2E_46 = -0x100;
	if (wizard->word_0x2E_46 > 0)
		wizard->word_0x2E_46 = 0;
	const int16_t minHeight = ReadS16(wizard->typeParams_0x9C_156 + 0x0C);
	const int32_t ground = GetTerrainAlt_11F50_11F50((axis_3d*)&wizard->position_0x48_72);
	if (wizard->position_0x48_72.z < minHeight + (int16_t)ground)
		wizard->position_0x48_72.z = (int16_t)(ground + minHeight);
	if (Type_Mc1Entity* smoke = CreateEntity_373F0_377B0(&featurePosition_AE454_AE444, 0x0A, 1))
	{
		smoke->flags_0x10_16 |= 0x80;
		smoke->id_0x18_24 = wizard->id_0x18_24;
	}
	if (wizard->position_0x48_72.z != (int16_t)ground + minHeight)
		return;
	RebuildFreeLists_37220_375E0();
	uint8_t* playerData = wizard->playerData_0xA0_160;
	const int16_t colour = ReadS16(playerData + 0x30);
	// the kill of a wizard to the killer
	if (const uint16_t killerIndex = wizard->word_0x26_38)
	{
		Type_Mc1Entity* killer = &Entities()[killerIndex];
		if (killer->class_0x40_64 == 3 && (killer->model_0x41_65 == 0 || killer->model_0x41_65 == 1))
		{
			uint8_t* kills = killer->playerData_0xA0_160 + 0x1E + colour * 2;
			WriteS16(kills, ReadS16(kills) + 1);
		}
	}
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Message& message = state->players_0x340B_13323[state->currentPlayer_0x08_8].messages_0x1C_28[colour];
	strcpy(message.text_0x00_0, (const char*)textPointers_AE238_AE228[54]);
	message.time_0x40_64 = 100;
	memset(wizard->mails_0x5A_90, 0, 0x24);
	// the spells of the wizard fall around it, their slots keep the spell number
	for (int i = 0; i < 24; i++)
	{
		uint8_t* slot = playerData + 0x214 + i * 4;
		const int32_t index = ReadS32(slot);
		if (index <= 0)
		{
			WriteS32(slot, -1);
			continue;
		}
		Type_Mc1Entity* spell = &Entities()[index];
		WriteS32(slot, (int8_t)spell->model_0x41_65);
		playerData[0x394 + (int8_t)spell->model_0x41_65] = (spell->flags_0x10_16 & 0x40000) ? 1 : 0;
		spell->flags_0x10_16 &= ~1u;
		spell->actionIndex_0x46_70++;
		featurePosition_AE454_AE444 = wizard->position_0x48_72;
		wizard->rand_0x04_4 = wizard->rand_0x04_4 * 0x24A1 + 0x24DF;
		featurePosition_AE454_AE444.x = (int16_t)(featurePosition_AE454_AE444.x + (int32_t)(wizard->rand_0x04_4 & 0x1FF) - 0x100);
		wizard->rand_0x04_4 = wizard->rand_0x04_4 * 0x24A1 + 0x24DF;
		featurePosition_AE454_AE444.y = (int16_t)(featurePosition_AE454_AE444.y + (int32_t)(wizard->rand_0x04_4 & 0x1FF) - 0x100);
		CopyEntityPosition_41C70_41FB0(spell, &featurePosition_AE454_AE444);
		wizard->rand_0x04_4 = wizard->rand_0x04_4 * 0x24A1 + 0x24DF;
		spell->life_0x0C_12 = (int32_t)(wizard->rand_0x04_4 % 0x5A + 0xC8);
	}
	if (Type_Mc1Entity* mana = CreateEntity_373F0_377B0(&wizard->position_0x48_72, 0x0A, 0x28))
	{
		wizard->actionIndex_0x46_70 = 3;
		wizard->word_0x1A_26 = (int16_t)((((0xFF - ReadS16(playerData + 0x20E)) / 8) << 5) + 0x20);
		// the mana balls of the wizard to the new one
		for (Type_Mc1Entity* ball = mc1Settings_AE408_AE3F8->entityLists_0x8E1A_36378[22]; ball > Entities(); ball = ball->next_0x00_0)
		{
			if (ball->model_0x41_65 == 0x27 && ball->owner_0x90_144 == (uint16_t)EntityIndex(wizard))
				ball->owner_0x90_144 = (uint16_t)EntityIndex(mana);
		}
	}
	wizard->flags_0x10_16 |= 0x20;
	state->reuseTop_0x11F1_4593--;
}

void LookAtKiller_463B0_466F0(Type_Mc1Entity* wizard)
{
	// the dead wizard turns to its killer (the pitch back to 0)
	Type_Mc1Position* killer = &Entities()[wizard->word_0x26_38].position_0x48_72;
	wizard->word_0x22_34 = (int16_t)GetYawAngle_42150_42490(Axis(&wizard->position_0x48_72), Axis(killer));
	wizard->word_0x24_36 = (int16_t)GetPitchAngle_42180_424C0(&wizard->position_0x48_72, killer);
	wizard->word_0x1E_30 = (int16_t)((wizard->word_0x1E_30 + TurnStep_422A0_425E0((uint16_t)wizard->word_0x1E_30, (uint16_t)wizard->word_0x22_34, 5, 0x16)) & 0x7FF);
	wizard->word_0x20_32 = (int16_t)((wizard->word_0x20_32 + TurnStep_422A0_425E0((uint16_t)wizard->word_0x20_32, (uint16_t)wizard->word_0x24_36, 5, 0x16)) & 0x7FF);
	wizard->word_0x20_32 = 0;
	WriteS16(wizard->playerData_0xA0_160 + 0x149, wizard->word_0x20_32);
	WriteS16(wizard->playerData_0xA0_160 + 0x147, 0);
}

void WizardDead_46480_467C0(Type_Mc1Entity* wizard)
{
	// a computer player comes back at its castle after word_0x1A frames (or leaves without one), a human looks at the killer
	uint8_t* playerData = wizard->playerData_0xA0_160;
	WriteS16(playerData + 0x16, 0);
	Type_Mc1Player_340B* player = &mc1State_AE400_AE3F0->players_0x340B_13323[ReadS16(playerData + 0x30)];
	if (player->computer_0x09_9 == 1)
	{
		if (mc1Settings_AE408_AE3F8->flags_0x00_0 & 0x200)
			return;
		if (!ReadS16(playerData + 0x32))
		{
			player->active_0x06_6 = 0;
			return;
		}
		if (wizard->word_0x1A_26)
		{
			wizard->word_0x1A_26--;
			return;
		}
		CreatePlayerWizard_44D30_45070(player, wizard);
		return;
	}
	SetFlashOfPlayer_44BE0_44F20(wizard, 7);
	LookAtKiller_463B0_466F0(wizard);
}
