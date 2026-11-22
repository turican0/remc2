#include "Mc1Network.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <thread>

#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1Graphics.h"
#include "Mc1MainMenu.h"
#include "Mc1Menu.h"
#include "Mc1MenuText.h"
#include "Mc1PlayerEvents.h"
#include "Mc1Shared.h"
#include "Mc1Sound.h"
#include "Mc1System.h"
#include "../../remc2/engine/Network.h"

int dos_getvect(int vector); // Network.cpp: the vector of NetBIOS (5Ch) with port_net running

// 9398C, 93960, 93964, 93968, 93990: the NCBs and the buffers in DOS memory (LowMalloc)
static myNCB* mainNcb_9398C_9398C = nullptr;
static uint8_t* sendBuffer_93960_93960 = nullptr;
static uint8_t* receiveBuffer_93964_93964 = nullptr;
static uint8_t* receiveBuffers_93968_93968[8] = {}; // never used by MC1 (43010 has no caller)
myNCB* networkNcbs_93990_93990[8] = {};
static int16_t networkCancelled_93988_93988 = 0;
char networkName_939B8_939B8[0x14] = "TESTER"; // the session name, the prefix of the names

// a name of NetBIOS: the text and the spaces up to 15 characters
static void PadName(char* field, const char* name)
{
	strcpy(field, name);
	while (strlen(field) < 0xF)
		strcat(field, " ");
}

static void WaitNcb(myNCB* ncb)
{
	while (ncb->ncb_cmd_cplt_49 == 0xFF)
		std::this_thread::yield();
}

// 431A0 / 434E0: int 5Ch through the real mode interrupt of DPMI, port: SetNetbios_75044 (port_net)
static int32_t NetbiosCall_431A0_434E0(myNCB* ncb)
{
	ncb->ncb_cmd_cplt_49 = 0;
	return SetNetbios_75044(ncb) ? -1 : 0;
}

// the slots of the players on the network screen (9689C): chosen, (never set), the state of the connection
typedef struct {
	uint8_t used_0;
	uint8_t flag_1;
	uint8_t state_2; // 0 failed, 1 waiting (flashing), 2 joined, 3 idle
} Type_Mc1NetworkSlot;

static Type_Mc1NetworkSlot networkSlots_9689C_9689C[8] = {
	{ 1, 0, 3 }, { 1, 0, 3 }, { 0, 0, 3 }, { 0, 0, 3 }, { 0, 0, 3 }, { 0, 0, 3 }, { 0, 0, 3 }, { 0, 0, 3 },
};

static void SetPlayerWaiting_4E3D0_4E710(uint8_t player);
static void SetPlayerJoined_4E470_4E7B0(uint8_t player);
static void SetPlayerFailed_4E510_4E850(uint8_t player);

// 428B0 / 42BF0: Esc or the cancel button (238h-25Eh, 60h-86h) stop the joining
static void CheckNetworkCancel_428B0_42BF0()
{
	if (*lastKey_12EF70_12EF60 == 1
		|| (*mouseLeftPressed_12EFDE_12EFCE && *mouseClickX_9AD8C_9AD8C >= 0x238 && *mouseClickX_9AD8C_9AD8C <= 0x25E
			&& *mouseClickY_9AD8E_9AD8E >= 0x60 && *mouseClickY_9AD8E_9AD8E <= 0x86))
	{
		networkCancelled_93988_93988 = 1;
		*lastKey_12EF70_12EF60 = 0;
		*mouseLeftPressed_12EFDE_12EFCE = 0;
		FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
		return;
	}
	*mouseLeftPressed_12EFDE_12EFCE = 0;
}

// 42930 / 42C70: ADD NAME, the slot of the player flashing until it ends
static int32_t AddName_42930_42C70(myNCB* ncb, const char* name, uint8_t player)
{
	PadName(ncb->ncb_name_26, name);
	ncb->ncb_command_0 = 0xB0;
	if (NetbiosCall_431A0_434E0(ncb) == -1)
		return -99;
	while (ncb->ncb_cmd_cplt_49 == 0xFF)
	{
		CheckNetworkCancel_428B0_42BF0();
		SetPlayerWaiting_4E3D0_4E710(player);
	}
	if (!ncb->ncb_cmd_cplt_49)
		SetPlayerJoined_4E470_4E7B0(player);
	else
		SetPlayerFailed_4E510_4E850(player);
	return -ncb->ncb_cmd_cplt_49;
}

// 42A00 / 42D40: CALL of the player from the own NCB against the LISTEN of his slot, the first done wins
static int32_t CallPlayer_42A00_42D40(int16_t player)
{
	myNCB* own = networkNcbs_93990_93990[networkIndex_9395A_9395A];
	myNCB* slot = networkNcbs_93990_93990[player];
	own->ncb_command_0 = 0x90;
	char name[0x14];
	sprintf(name, "%s%d", networkName_939B8_939B8, player);
	PadName(own->ncb_callName_10, name);
	own->ncb_rto_42 = 0;
	own->ncb_sto_43 = 0;
	if (NetbiosCall_431A0_434E0(own) == -1)
		return -99;
	while (own->ncb_cmd_cplt_49 == 0xFF && own->ncb_cmd_cplt_49 == slot->ncb_cmd_cplt_49)
	{
		SetPlayerWaiting_4E3D0_4E710((uint8_t)player);
		CheckNetworkCancel_428B0_42BF0();
		if (networkCancelled_93988_93988 == 1)
		{
			CancelNcb_42C70_42FB0(networkIndex_9395A_9395A);
			return -1;
		}
	}
	int16_t joined = 0;
	if (!slot->ncb_cmd_cplt_49)
	{
		CancelNcb_42C70_42FB0(networkIndex_9395A_9395A);
		joined = 1;
	}
	if (!own->ncb_cmd_cplt_49)
	{
		CancelNcb_42C70_42FB0(player);
		joined = 1;
	}
	if (joined == 1)
		SetPlayerJoined_4E470_4E7B0((uint8_t)player);
	if (!own->ncb_cmd_cplt_49)
	{
		// the session of the call moves to the slot of the player
		CancelNcb_42C70_42FB0(player);
		strcpy(slot->ncb_callName_10, own->ncb_callName_10);
		slot->ncb_lsn_2 = own->ncb_lsn_2;
		slot->ncb_cmd_cplt_49 = own->ncb_cmd_cplt_49;
	}
	return -slot->ncb_cmd_cplt_49;
}

// 42C70 / 42FB0: CANCEL of the pending command of the slot
int32_t CancelNcb_42C70_42FB0(int16_t player)
{
	myNCB* ncb = networkNcbs_93990_93990[player];
	if (ncb->ncb_cmd_cplt_49 == 0xFF)
	{
		mainNcb_9398C_9398C->ncb_command_0 = 0x35;
		mainNcb_9398C_9398C->ncb_buffer_4.p = (uint8_t*)ncb;
		if (NetbiosCall_431A0_434E0(mainNcb_9398C_9398C) == -1)
			return -99;
		while (mainNcb_9398C_9398C->ncb_cmd_cplt_49 == 0xFF || ncb->ncb_cmd_cplt_49 == 0xFF)
			std::this_thread::yield();
	}
	return -mainNcb_9398C_9398C->ncb_cmd_cplt_49;
}

// 42CF0 / 43030: NetBIOS present, the invalid command 7Fh answers 3
static int32_t NetbiosPresent_42CF0_43030()
{
	if (!dos_getvect(0x5C))
		return -1;
	mainNcb_9398C_9398C->ncb_command_0 = 0x7F;
	if (NetbiosCall_431A0_434E0(mainNcb_9398C_9398C) == -1)
		return -1;
	return mainNcb_9398C_9398C->ncb_retcode_1 == 3 ? 0 : -1;
}

// 42D40 / 43080: DELETE NAME
int32_t DeleteName_42D40_43080(myNCB* ncb, const char* name)
{
	PadName(ncb->ncb_name_26, name);
	ncb->ncb_command_0 = 0xB1;
	if (NetbiosCall_431A0_434E0(ncb) == -1)
		return -99;
	WaitNcb(ncb);
	return -ncb->ncb_cmd_cplt_49;
}

// 42DE0 / 43120: HANG UP
int32_t HangUp_42DE0_43120(myNCB* ncb)
{
	ncb->ncb_command_0 = 0x92;
	if (NetbiosCall_431A0_434E0(ncb) == -1)
		return -99;
	WaitNcb(ncb);
	const uint8_t result = ncb->ncb_cmd_cplt_49;
	ncb->ncb_lsn_2 = 0;
	return -result;
}

// 42E10 / 43150: LISTEN for the player in his slot
static int32_t Listen_42E10_43150(int16_t player)
{
	myNCB* ncb = networkNcbs_93990_93990[player];
	if (ncb->ncb_cmd_cplt_49 != 0xFF)
	{
		char name[0x14];
		sprintf(name, "%s%d", networkName_939B8_939B8, player);
		ncb->ncb_command_0 = 0x91;
		PadName(ncb->ncb_callName_10, name);
		ncb->ncb_rto_42 = 0;
		ncb->ncb_sto_43 = 0;
		if (NetbiosCall_431A0_434E0(ncb) == -1)
			return -99;
		ncb->ncb_buffer_4.p = nullptr; // the offset of the buffer cleared
	}
	return -ncb->ncb_cmd_cplt_49;
}

// 42F30 / 43270: RECEIVE of up to 800h bytes
static int32_t ReceivePacket_42F30_43270(myNCB* ncb, uint8_t* buffer)
{
	ncb->ncb_command_0 = 0x95;
	ncb->ncb_bufferLength_8 = 0x800;
	ncb->ncb_buffer_4.p = receiveBuffer_93964_93964;
	if (NetbiosCall_431A0_434E0(ncb) == -1)
		return -99;
	WaitNcb(ncb);
	if (ncb->ncb_cmd_cplt_49)
		return -ncb->ncb_cmd_cplt_49;
	memcpy(buffer, receiveBuffer_93964_93964, ncb->ncb_bufferLength_8);
	return ncb->ncb_bufferLength_8;
}

// 42FB0 / 432F0: a message in packets of 800h bytes
static int32_t ReceiveMessage_42FB0_432F0(myNCB* ncb, uint8_t* buffer, uint32_t size)
{
	int32_t result;
	for (uint32_t packet = 0; packet < (size >> 11); packet++)
	{
		result = ReceivePacket_42F30_43270(ncb, buffer);
		if (result != 0x800)
			return result;
		buffer += 0x800;
	}
	result = ReceivePacket_42F30_43270(ncb, buffer);
	if (result != (int32_t)(size & 0x7FF))
		return result;
	return (int32_t)size;
}

// 43080 / 433C0: SEND
static int32_t SendPacket_43080_433C0(myNCB* ncb, const uint8_t* buffer, uint32_t size)
{
	memcpy(sendBuffer_93960_93960, buffer, size);
	ncb->ncb_command_0 = 0x94;
	ncb->ncb_bufferLength_8 = (uint16_t)size;
	ncb->ncb_buffer_4.p = sendBuffer_93960_93960;
	if (NetbiosCall_431A0_434E0(ncb) == -1)
		return -99;
	WaitNcb(ncb);
	return -ncb->ncb_cmd_cplt_49;
}

// 430F0 / 43430: a message in packets of 800h bytes
static int32_t SendMessage_430F0_43430(myNCB* ncb, const uint8_t* buffer, uint32_t size)
{
	for (uint32_t packet = 0; packet < (size >> 11); packet++)
	{
		const int16_t result = (int16_t)SendPacket_43080_433C0(ncb, buffer, 0x800);
		if (result)
			return result;
		buffer += 0x800;
	}
	return SendPacket_43080_433C0(ncb, buffer, size & 0x7FF);
}

// 43170 / 434B0: 2 this computer, 1 a session, 0 none
static uint8_t NetworkState_43170_434B0(int16_t player)
{
	if (player == networkIndex_9395A_9395A)
		return 2;
	const myNCB* ncb = networkNcbs_93990_93990[player];
	if (ncb->ncb_lsn_2 && !ncb->ncb_cmd_cplt_49)
		return 1;
	return 0;
}

void UpdateNetworkActive_43150_43490()
{
	for (int16_t i = 0; i < 8; i++)
		networkActive_939B0_939B0[i] = NetworkState_43170_434B0(i);
}

int32_t InitNetwork_43270_435B0()
{
	int32_t result = 1;
	if (!networkFound_93958_93958 && !mainNcb_9398C_9398C)
	{
		// system("netbios"): the TSR NETBIOS.EXE, port: port_net (StartNetworkLibrary)
		mainNcb_9398C_9398C = (myNCB*)LowMalloc_425C0_42900(0x42);
		if (!mainNcb_9398C_9398C)
			result = -1;
		else
		{
			if (NetbiosPresent_42CF0_43030() == -1)
				return -1;
			sendBuffer_93960_93960 = (uint8_t*)LowMalloc_425C0_42900(0x800);
			if (!sendBuffer_93960_93960)
				result = -1;
			else
			{
				receiveBuffer_93964_93964 = (uint8_t*)LowMalloc_425C0_42900(0x800);
				if (!receiveBuffer_93964_93964)
					result = -1;
				else
				{
					for (int32_t i = 0; i < 8; i++)
					{
						receiveBuffers_93968_93968[i] = (uint8_t*)LowMalloc_425C0_42900(0x800);
						if (!receiveBuffers_93968_93968[i])
							result = -1;
						networkNcbs_93990_93990[i] = (myNCB*)LowMalloc_425C0_42900(0x42);
						if (!networkNcbs_93990_93990[i])
							result = -1;
					}
					networkFound_93958_93958 = 1;
				}
			}
		}
	}
	if (result == -1)
	{
		Free_426E0_42A20(mainNcb_9398C_9398C);
		Free_426E0_42A20(sendBuffer_93960_93960);
		Free_426E0_42A20(receiveBuffer_93964_93964);
		for (int32_t i = 0; i < 8; i++)
		{
			Free_426E0_42A20(networkNcbs_93990_93990[i]);
			Free_426E0_42A20(receiveBuffers_93968_93968[i]);
		}
	}
	return result;
}

void FreeNetwork_433C0_43700()
{
	if (!networkFound_93958_93958)
		return;
	Free_426E0_42A20(mainNcb_9398C_9398C);
	for (int32_t i = 0; i < 8; i++)
	{
		Free_426E0_42A20(networkNcbs_93990_93990[i]);
		Free_426E0_42A20(receiveBuffers_93968_93968[i]);
	}
	Free_426E0_42A20(sendBuffer_93960_93960);
	Free_426E0_42A20(receiveBuffer_93964_93964);
	networkFound_93958_93958 = 0;
}

// 43430 / 43770: the first free name <session><index> is this computer, then LISTEN and CALL with all the others
// until all are joined or cancelled; the index of this computer, -1 when not joined
int16_t JoinNetworkSession_43430_43770(char* sessionName, uint16_t playerCount)
{
	if (!networkFound_93958_93958)
		return -1;
	if (networkJoined_93959_93959)
		return networkIndex_9395A_9395A;
	networkCancelled_93988_93988 = 0;
	networkCount_9395C_9395C = (int16_t)playerCount;
	strcpy(networkName_939B8_939B8, sessionName);
	memset(networkActive_939B0_939B0, 0, 8);
	char name[0x14];
	int32_t i = 0;
	networkIndex_9395A_9395A = -1;
	while (networkIndex_9395A_9395A == -1 && !networkCancelled_93988_93988)
	{
		sprintf(name, "%s%d", networkName_939B8_939B8, i);
		const int32_t result = AddName_42930_42C70(networkNcbs_93990_93990[i], name, (uint8_t)i);
		if (!result)
			networkIndex_9395A_9395A = (int16_t)i;
		else if (result == -13)
		{
			// 0Dh: the name in the own table, deleted and tried again
			DeleteName_42D40_43080(networkNcbs_93990_93990[i], name);
			i--;
		}
		if (++i >= 8)
			break;
	}
	if (networkIndex_9395A_9395A == -1)
		return -1;
	if (networkCancelled_93988_93988)
	{
		sprintf(name, "%s%d", networkName_939B8_939B8, networkIndex_9395A_9395A);
		//!!!!! the original deletes on the NCB after the own one (the 9th behind the table for the index 7)
		DeleteName_42D40_43080(networkNcbs_93990_93990[i < 8 ? i : networkIndex_9395A_9395A], name);
		return -1;
	}
	const myNCB* own = networkNcbs_93990_93990[networkIndex_9395A_9395A];
	for (int32_t j = 0; j < 8; j++)
	{
		if (j == networkIndex_9395A_9395A)
			continue;
		strcpy(networkNcbs_93990_93990[j]->ncb_name_26, own->ncb_name_26);
		networkNcbs_93990_93990[j]->ncb_num_3 = own->ncb_num_3;
	}
	for (int16_t j = 0; j < networkCount_9395C_9395C; j++)
		if (j != networkIndex_9395A_9395A)
			Listen_42E10_43150(j);
	for (int16_t j = 0; j < networkCount_9395C_9395C; j++)
	{
		if (j == networkIndex_9395A_9395A || networkNcbs_93990_93990[j]->ncb_cmd_cplt_49 != 0xFF)
			continue;
		CallPlayer_42A00_42D40(j);
		if (networkCancelled_93988_93988)
			break;
	}
	int32_t joined;
	do
	{
		CheckNetworkCancel_428B0_42BF0();
		joined = 0;
		for (int16_t j = 0; j < networkCount_9395C_9395C; j++)
		{
			if (j == networkIndex_9395A_9395A || !networkNcbs_93990_93990[j]->ncb_cmd_cplt_49)
			{
				SetPlayerJoined_4E470_4E7B0((uint8_t)j);
				if (networkCancelled_93988_93988)
				{
					if (j != networkIndex_9395A_9395A)
						HangUp_42DE0_43120(networkNcbs_93990_93990[j]);
					else
					{
						sprintf(name, "%s%d", networkName_939B8_939B8, networkIndex_9395A_9395A);
						DeleteName_42D40_43080(networkNcbs_93990_93990[j], name);
					}
				}
				joined++;
			}
			else if (networkCancelled_93988_93988)
				CancelNcb_42C70_42FB0(j);
		}
	} while (joined != networkCount_9395C_9395C && !networkCancelled_93988_93988);
	if (networkCancelled_93988_93988)
		return -1;
	UpdateNetworkActive_43150_43490();
	networkMaster_9398A_9398A = 0;
	networkJoined_93959_93959 = 1;
	return networkIndex_9395A_9395A;
}

// 438D0 / 43C10: a message from the player, his slot listening again when the session broke
void ReceiveFrom_438D0_43C10(uint16_t player, uint8_t* buffer, uint32_t size)
{
	if (!networkFound_93958_93958 || networkActive_939B0_939B0[player] != 1)
		return;
	ReceiveMessage_42FB0_432F0(networkNcbs_93990_93990[player], buffer, size);
	if (networkNcbs_93990_93990[player]->ncb_cmd_cplt_49)
		Listen_42E10_43150((int16_t)player);
}

// 43870 / 43BB0: a message to the player
void SendTo_43870_43BB0(uint16_t player, uint8_t* buffer, uint32_t size)
{
	if (!networkFound_93958_93958 || networkActive_939B0_939B0[player] != 1)
		return;
	SendMessage_430F0_43430(networkNcbs_93990_93990[player], buffer, size);
	if (networkNcbs_93990_93990[player]->ncb_cmd_cplt_49)
		Listen_42E10_43150((int16_t)player);
}

// ---- the network game screen (the menu item 4) ---------------------------------------------------------------

static uint8_t* networkSpritesTab_12CB10_12CB00 = nullptr;
static uint8_t* networkSpritesTabEnd_12CB14_12CB04 = nullptr;
static uint8_t* networkSpritesData_12CB18_12CB08 = nullptr;
static bitmap_pos_struct_t* networkSpritesIndex = nullptr;
static bitmap_pos_struct_t* networkFontIndex = nullptr;
static int16_t previousLevel_12CBC4_12CBB4 = 0;

// 49EF0 / 4A230: the sprites of the screen and sfont1 again (the font of the main menu, freed with it)
static Type_Mc1FileEntry networkFiles_49EF0_4A230[5] = {
	{ { "data/screens/pmultspr.dat", "data/screens/pmultspr.dat" }, &networkSpritesData_12CB18_12CB08, nullptr, 0, 0 },
	{ { "data/screens/pmultspr.tab", "data/screens/pmultspr.tab" }, &networkSpritesTab_12CB10_12CB00, &networkSpritesTabEnd_12CB14_12CB04, 0, 0 },
	{ { "data/screens/sfont1.dat", "data/screens/sfont1.dat" }, &mainMenuFontData_12CB3C_12CB2C, nullptr, 0, 0 },
	{ { "data/screens/sfont1.tab", "data/screens/sfont1.tab" }, &mainMenuFontTab_12CB34_12CB24, &mainMenuFontTabEnd_12CB38_12CB28, 0, 0 },
	{ { "", "" }, nullptr, nullptr, 0, 0 },
};

// 4A7CC / 4AB0C: the places of the slots of the players (x, y at 320x200)
static const int16_t networkSlotPlaces_4A7CC_4AB0C[8][2] = {
	{ 0x33, 0x2F }, { 0x4B, 0x2D }, { 0x64, 0x2E }, { 0x7E, 0x2C }, { 0x32, 0x69 }, { 0x4D, 0x65 }, { 0x66, 0x64 }, { 0x81, 0x63 },
};

// 99A84: the network levels 51-70 by the game
static const char* const networkLevelsMc1_99A84[20] = {
	"Bussorah", "Bisnagar", "Tartary", "Akkania", "Ryahn", "Zhullor", "Dombren", "Xushtan", "Wazzad", "Phiria",
	"Moussul", "Aleppo", "Schiraz", "Ispahan", "Coam", "Rei", "Buddoor", "Jehoun", "Nurrir", "Comari",
};

static const char* const networkLevelsHw_99A84[20] = {
	"Goyaan", "Cyrecius", "Esyphium", "Barabban", "Tervilar", "Imnara", "Kouppi", "Fayaoud", "Rama'Q", "Halaj",
	"Moussul", "Aleppo", "Schiraz", "Ispahan", "Coam", "Rei", "Buddoor", "Jehoun", "Nurrir", "Comari",
};

static void DrawNetworkSprite(int32_t x, int32_t y, const bitmap_pos_struct_t& sprite)
{
	if (*videoMode_12F02E_12F01E & 1)
		DrawBitmap320_60CE0_611F0((int16_t)x, (int16_t)y, sprite, 1);
	else
		DrawBitmap640_60D18_61228((int16_t)x, (int16_t)y, sprite, 1);
}

// 4DCC0 / 4E000: the background, the slots of the players and 5 of the levels from the scroll position
static void DrawNetworkScreen_4DCC0_4E000()
{
	if ((dword_12CBB0_12CBA0 & 7) == 7)
	{
		byte_12CBD2_12CBC2++;
		if (byte_12CBD2_12CBC2 == 0x15)
			byte_12CBD2_12CBC2 = 6;
	}
	// the waiting cursor while joining
	if (menuFlags_12CBD5_12CBC5 & 0x10)
	{
		if (!inputDevices_90B23_90B23)
			SetCursor_5C05C_5C56C(menuSpritesIndex[byte_12CBD2_12CBC2]);
	}
	else if (!inputDevices_90B23_90B23)
		SetCursor_5C05C_5C56C(menuSpritesIndex[mainMenuItem_12CBCE_12CBBE]);
	CopyBackScreen();
	SaveTextWindow_513E0_51720();
	SetTextWindow_51360_516A0(0, 0, 0x140, 0xC8);
	for (uint8_t i = 0; i < 8; i++)
	{
		const Type_Mc1NetworkSlot& slot = networkSlots_9689C_9689C[i];
		const int32_t x = networkSlotPlaces_4A7CC_4AB0C[i][0];
		const int32_t y = networkSlotPlaces_4A7CC_4AB0C[i][1];
		if (!slot.used_0 && !slot.flag_1)
			DrawNetworkSprite(x * 2, y * 2, networkSpritesIndex[i + 1]);
		else if (slot.flag_1)
			DrawNetworkSprite((x + 10) * 2, (y - 10) * 2, networkSpritesIndex[0x11]);
		else
		{
			switch (slot.state_2)
			{
			case 0:
			case 3:
				DrawNetworkSprite(x * 2, y * 2, networkSpritesIndex[i + 9]);
				break;
			case 1:
				if ((dword_12CBB0_12CBA0 >> 4) & 1)
					DrawNetworkSprite(x * 2, y * 2, networkSpritesIndex[i + 0x11]);
				break;
			case 2:
				DrawNetworkSprite(x * 2, y * 2, networkSpritesIndex[i + 0x11]);
				break;
			}
		}
	}
	SetTextWindow_51360_516A0(0xAD, 0x48, 0x5A, 0x3D);
	const uint8_t selectedColor = FindNearestColor_5CC70_5D180(menuPalette_12CB9C_12CB8C, 0x3F, 0x3F, 0x3F);
	const uint8_t normalColor = FindNearestColor_5CC70_5D180(menuPalette_12CB9C_12CB8C, 0x1A, 0x12, 0x0D);
	const char* const* names = mc1Variant == MC1_VARIANT_HW ? networkLevelsHw_99A84 : networkLevelsMc1_99A84;
	int16_t y = 0;
	for (int32_t level = (uint16_t)word_12CBBE_12CBAE; level < (uint16_t)word_12CBBE_12CBAE + 5; level++)
	{
		mainMenuFont_9688C_9688C.color1_0x06_6 = level == (uint16_t)word_12CBC2_12CBB2 ? selectedColor : normalColor;
		const int32_t width = TextWidth_51420_51760(&mainMenuFont_9688C_9688C, names[level]);
		DrawText_51560_518A0(&mainMenuFont_9688C_9688C, (int16_t)((0x5A - width) / 2), y, names[level]);
		y += mainMenuFont_9688C_9688C.glyphs_0x00_0[0].height_5 - 2;
	}
	RestoreTextWindow_51400_51740();
	mainMenuFont_9688C_9688C.color1_0x06_6 = normalColor;
}

// the screen with the session name in its box (the common tail of 4D680, 4E320, 4E3D0, ...)
static void DrawNetworkScreenAndSession()
{
	DrawNetworkScreen_4DCC0_4E000();
	SaveTextWindow_513E0_51720();
	SetTextWindow_51360_516A0(0xAD, 0x2A, 0x5A, 0x0F);
	const char* session = mc1Settings_AE408_AE3F8->sessionName_0x75_117;
	const int32_t width = TextWidth_51420_51760(&mainMenuFont_9688C_9688C, session);
	DrawText_51560_518A0(&mainMenuFont_9688C_9688C, (int16_t)((0x5A - width) / 2), 1, session);
	RestoreTextWindow_51400_51740();
}

// 4E0E0 / 4E420: the files and the palette of the screen
static void OpenNetworkScreen_4E0E0_4E420()
{
	FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
	StopMusic_20E60_20E60();
	LoadFileGroup_40440_40780(networkFiles_49EF0_4A230);
	BuildSpritesIndex(&networkSpritesIndex, networkSpritesTab_12CB10_12CB00, networkSpritesTabEnd_12CB14_12CB04,
		networkSpritesData_12CB18_12CB08, true);
	BuildSpritesIndex(&networkFontIndex, mainMenuFontTab_12CB34_12CB24, mainMenuFontTabEnd_12CB38_12CB28,
		mainMenuFontData_12CB3C_12CB2C, false);
	InitFont_51480_517C0(&mainMenuFont_9688C_9688C, networkFontIndex, menuPalette_12CB9C_12CB8C);
	mainMenuFont_9688C_9688C.flags_0x04_4 &= 0xFD;
	ReadPackedFile_3EEA0_3F1E0("data/screens/pmulti.pal", menuPalette_12CB9C_12CB8C);
	ReadPackedFile_3EEA0_3F1E0("data/screens/pmulti.dat", backScreen_AE3FC_AE3EC);
	CopyBackScreen();
	BlitScreen();
	if (!inputDevices_90B23_90B23)
		SetCursor_5C05C_5C56C(menuSpritesIndex[mainMenuItem_12CBCE_12CBBE]);
	FadeInOut_61CC0_621D0((TColor*)menuPalette_12CB9C_12CB8C, 0x20, false, 10);
	menuFlags_12CBD5_12CBC5 |= 1;
	mainMenuFont_9688C_9688C.color1_0x06_6 = FindNearestColor_5CC70_5D180(menuPalette_12CB9C_12CB8C, 0x1A, 0x12, 0x0D);
}

// 4E270 / 4E5B0
static void CloseNetworkScreen_4E270_4E5B0()
{
	FadeInOut_61CC0_621D0(nullptr, 0x10, false, 10);
	if (!inputDevices_90B23_90B23)
		SetCursor_5C05C_5C56C(bitmap_pos_struct_t{});
	if (*videoMode_12F02E_12F01E & 1)
		ClearScreen320_411D7_41517(*workScreen_12EFF4_12EFE4, 320, 200, 0);
	else
		ClearScreen640_411FD_4153D(*workScreen_12EFF4_12EFE4, 640, 480, 0);
	BlitScreen();
	FreeFileGroup_61610_61B20(networkFiles_49EF0_4A230);
	FreeSpritesIndex(&networkSpritesIndex);
	FreeSpritesIndex(&networkFontIndex);
	menuFlags_12CBD5_12CBC5 &= 0xFE;
	if (mainMenuItem_12CBCE_12CBBE == 2)
		PlayMusic_5D290_5D7A0(4);
}

// 4E320 / 4E660: the chosen slots back to failed
static void ResetNetworkSlots_4E320_4E660()
{
	dword_12CBB0_12CBA0++;
	for (uint8_t i = 0; i < 8; i++)
		if (networkSlots_9689C_9689C[i].used_0)
			networkSlots_9689C_9689C[i].state_2 = 0;
	DrawNetworkScreenAndSession();
	BlitScreen();
}

static void SetPlayerState(uint8_t player, uint8_t state)
{
	networkSlots_9689C_9689C[player].state_2 = state;
	dword_12CBB0_12CBA0++;
	DrawNetworkScreenAndSession();
	BlitScreen();
}

// 4E3D0 / 4E710, 4E470 / 4E7B0, 4E510 / 4E850
static void SetPlayerWaiting_4E3D0_4E710(uint8_t player)
{
	SetPlayerState(player, 1);
}

static void SetPlayerJoined_4E470_4E7B0(uint8_t player)
{
	SetPlayerState(player, 2);
}

static void SetPlayerFailed_4E510_4E850(uint8_t player)
{
	SetPlayerState(player, 0);
}

static bool InBox(int32_t x, int32_t y, int32_t left, int32_t right, int32_t top, int32_t bottom)
{
	return x >= left && x <= right && y >= top && y <= bottom;
}

void NetworkGameScreen_4D680_4D9C0()
{
	Type_Mc1State_AE400_AE3F0* state = mc1State_AE400_AE3F0;
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	bool leave = false;
	bool joined = false;
	if (!(menuFlags_12CBD5_12CBC5 & 1))
	{
		OpenNetworkScreen_4E0E0_4E420();
		ResetNetworkSlots_4E320_4E660();
	}
	if ((*pressedKeys_12EEF0_12EEE0)[1])
	{
		(*pressedKeys_12EEF0_12EEE0)[1] = 0;
		leave = true;
	}
	// the list of the levels under the mouse
	int32_t mouseX = mouseX_12CADC_12CACC >> 1;
	int32_t mouseY = mouseY_12CADE_12CACE >> 1;
	if (InBox(mouseX, mouseY, 0xAD, 0x107, 0x48, 0x85))
	{
		word_12CBC0_12CBB0 = (int16_t)((mouseY - 0x48) / (mainMenuFont_9688C_9688C.glyphs_0x00_0[0].height_5 - 2));
		if (inputFlags_12CAE4_12CAD4[0] & 1)
			word_12CBC2_12CBB2 = (int16_t)(word_12CBBE_12CBAE + word_12CBC0_12CBB0);
	}
	// the arrows of the list
	if (inputFlags_12CAE4_12CAD4[0] & 1)
	{
		if (InBox(mouseX, mouseY, 0xAD, 0x106, 0x39, 0x47))
		{
			if (word_12CBBE_12CBAE)
				word_12CBBE_12CBAE--;
		}
		else if (InBox(mouseX, mouseY, 0xAD, 0x106, 0x86, 0x94))
		{
			if ((uint16_t)word_12CBBE_12CBAE + 5 < 0x14)
				word_12CBBE_12CBAE++;
		}
	}
	if ((inputFlags_12CAE4_12CAD4[0] & 1) || (inputFlags_12CAE4_12CAD4[0] & 0x40))
	{
		const int32_t clickX = clickX_12CAE0_12CAD0 >> 1;
		const int32_t clickY = clickY_12CAE2_12CAD2 >> 1;
		if (InBox(clickX, clickY, 0x11C, 0x12F, 0x30, 0x43))
		{
			// the upper button does nothing
		}
		else if (InBox(clickX, clickY, 0x11C, 0x12F, 0x7E, 0x91))
		{
			// start: the chosen level, the session joined with the chosen number of the players
			previousLevel_12CBC4_12CBB4 = (int16_t)settings->levelIndex_0x11_17;
			settings->levelIndex_0x11_17 = (uint16_t)(word_12CBC2_12CBB2 + 0x32);
			menuFlags_12CBD5_12CBC5 |= 0x10;
			byte_12CBD2_12CBC2 = 6;
			DrawNetworkScreenAndSession();
			BlitScreen();
			state->playerCount_0x0A_10 = byte_12CBD1_12CBC1;
			menuFlags_12CBD5_12CBC5 &= 0xEF;
			if (!inputDevices_90B23_90B23)
				SetCursor_5C05C_5C56C(menuSpritesIndex[mainMenuItem_12CBCE_12CBBE]);
			state->currentPlayer_0x08_8 = JoinNetworkSession_43430_43770(settings->sessionName_0x75_117, byte_12CBD1_12CBC1);
			if (state->currentPlayer_0x08_8 != -1)
			{
				// the level of the master for all
				settings->flags_0x00_0 |= 0x10;
				state->playerInputs_0x7413_29715[state->currentPlayer_0x08_8].param_0x01_1 = (int8_t)settings->levelIndex_0x11_17;
				ExchangeAll_43930_43C70((uint8_t*)state->playerInputs_0x7413_29715, sizeof(Type_Mc1PlayerInput));
				menuDone_96880_96880 = 1;
				joined = true;
				leave = true;
				settings->levelIndex_0x11_17 = (uint16_t)(int16_t)state->playerInputs_0x7413_29715[0].param_0x01_1;
			}
			else
			{
				ResetNetworkSlots_4E320_4E660();
				FadeInOut_61CC0_621D0((TColor*)menuPalette_12CB9C_12CB8C, 0x20, false, 10);
			}
		}
		else if (InBox(clickX, clickY, 0xEC, 0x100, 0x9C, 0xB0))
			leave = true; // the exit button
		else if (InBox(clickX, clickY, 0xB0, 0xFD, 0x2E, 0x3A))
		{
			// the session name: CARPET0-9
			inputFlags_12CAE4_12CAD4[0] &= 0xFE;
			byte_12CBD0_12CBC0++;
			if (byte_12CBD0_12CBC0 == 10)
				byte_12CBD0_12CBC0 = 0;
			sprintf(settings->sessionName_0x75_117, "CARPET%d", byte_12CBD0_12CBC0);
		}
		else
		{
			// the number of the players: the slot clicked and the ones before it, at least 2
			for (uint8_t slot = 0; slot < 8; slot++)
			{
				const int32_t left = networkSlotPlaces_4A7CC_4AB0C[slot][0];
				const int32_t top = networkSlotPlaces_4A7CC_4AB0C[slot][1];
				const bitmap_pos_struct_t& sprite = networkSpritesIndex[slot + 1];
				if (!InBox(clickX, clickY, left, left + (sprite.width_4 >> 1), top, top + (sprite.height_5 >> 1))
					|| !(inputFlags_12CAE4_12CAD4[0] & 1))
					continue;
				byte_12CBD1_12CBC1 = 0;
				for (uint8_t j = 0; j < 8; j++)
				{
					if (j <= slot || j < 2)
					{
						byte_12CBD1_12CBC1++;
						networkSlots_9689C_9689C[j].used_0 = 1;
					}
					else
						networkSlots_9689C_9689C[j].used_0 = 0;
					networkSlots_9689C_9689C[j].flag_1 = 0;
				}
			}
		}
	}
	inputFlags_12CAE4_12CAD4[0] &= 0xBE;
	DrawNetworkScreenAndSession();
	if (inputDevices_90B23_90B23 & 0x23)
		DrawNetworkSprite(mousePos_9AD90_9AD90->x, mousePos_9AD90_9AD90->y, menuSpritesIndex[mainMenuItem_12CBCE_12CBBE]);
	BlitScreen();
	if (!leave)
		return;
	CloseNetworkScreen_4E270_4E5B0();
	if (joined)
		return;
	// back to the main menu with the player of the single game
	state->players_0x340B_13323[0] = state->networkPlayer_0x2409_9225;
	mainMenuItem_12CBCE_12CBBE = 2;
	settings->flags_0x00_0 &= ~0x10u;
	settings->levelIndex_0x11_17 = settings->networkLevel_0x13_19;
	state->currentPlayer_0x08_8 = 0;
}

// ---- port: port_net started from the command line as MC2 ----------------------------------------------------

static bool networkLibraryStarted = false;

bool ParseNetworkOption(const char* option, int argc, char** argv, int& index)
{
	if (!_stricmp("server", option) && index + 1 < argc)
	{
		Iam_server = true;
		Iam_client = true;
		ServerPort = atoi(argv[++index]);
		NetworkPort = ServerPort;
		strcpy(serverIP, "127.0.0.1");
		return true;
	}
	if (!_stricmp("client", option) && index + 3 < argc)
	{
		Iam_client = true;
		strcpy(serverIP, argv[++index]);
		ServerPort = atoi(argv[++index]);
		NetworkPort = atoi(argv[++index]);
		return true;
	}
	return false;
}

void StartNetworkLibrary()
{
	if (networkLibraryStarted || !CommandLineParams.ModeNetwork() || !(Iam_server || Iam_client))
		return;
	InitNetworkInfo();
	networkLibraryStarted = true;
}

void StopNetworkLibrary()
{
	if (!networkLibraryStarted)
		return;
	EndMyNetLib();
	networkLibraryStarted = false;
}
