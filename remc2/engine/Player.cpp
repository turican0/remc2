#include "Player.h"

#include "../utilities/Maths.h"
#include "EventsFunctions.h"
#include "Castle.h"
#include "Creatures.h"
#include "Effects.h"
#include "CommandLineParser.h"
#include "GameUI.h"
#include "Graphics.h"
#include "MenusAndIntros.h"
#include "Network.h"
#include "PlayerInput.h"
#include "Events.h"
#include "Level.h"
#include "../utilities/StateMonitor.h"
#include "../utilities/RendererTests.h"
#include "ConvertMapInfo.h"
#include "LevelInit.h"
#include "DatTabIndexes.h"
#include "MouseOnOff.h"
#include "PrintMessages.h"
#include "ReadAndDecompress.h"
#include "Type_DB080_CdTrack.h"

int moveBoosStep_D4B90 = 4294967292; // weak

//----- (00057FA0) --------------------------------------------------------
void MoveEntity_57FA0(axis_3d* position, uint16_t yaw, int16_t pitch, int16_t speed)//238fa0 // move player
{
	if (speed)
	{
		pitch &= 0x7ffu;
		yaw &= 0x7ffu;
		if (pitch)
		{
			position->z -= (int)(speed * Maths::sin_DB750[pitch]) >> 16;//change z axis
			speed = (int)(speed * Maths::sin_DB750[0x200 + pitch]) >> 16;
		}
		position->x += (int)(speed * Maths::sin_DB750[yaw]) >> 16;
		position->y -= (int)(speed * Maths::sin_DB750[0x200 + yaw]) >> 16;
	}
}


//----- (00048120) --------------------------------------------------------
void SetPlayerColoursForMapType_48120()//229120
{
	MapType_t result = world_D41A0.terrain_2FECE.MapType;
	// Note: these settings affect the color of player names and of spell power bars as well as some entities on the (mini)map
	if (result != MapType_t::Day)
	{
		if (result == MapType_t::Night)
		{
			playersColors_E88E0x[0][0] = 0xA4;
			playersColors_E88E0x[0][1] = 0xAA;
			playersColors_E88E0x[0][2] = 0x7B;
			playersColors_E88E0x[1][0] = 0x77;
			playersColors_E88E0x[1][1] = 0x7D;
			playersColors_E88E0x[1][2] = 0x7B;
			playersColors_E88E0x[2][0] = 0xC0;
			playersColors_E88E0x[2][1] = 0xC6;
			playersColors_E88E0x[2][2] = 0x7B;
			playersColors_E88E0x[3][0] = 0x58;
			playersColors_E88E0x[3][2] = 0x7B;
			playersColors_E88E0x[4][0] = 0x97;
			playersColors_E88E0x[5][0] = 0xD7;
			playersColors_E88E0x[5][1] = 0xDD;
			
		}
		else // Cave
		{
			playersColors_E88E0x[0][0] = 0xE0;
			playersColors_E88E0x[0][1] = 0x58;
			playersColors_E88E0x[0][2] = 0x7B;
			playersColors_E88E0x[1][0] = 0x77;
			playersColors_E88E0x[1][1] = 0x7D;
			playersColors_E88E0x[1][2] = 0x7B;
			playersColors_E88E0x[2][0] = 0xC0;
			playersColors_E88E0x[2][1] = 0xC6;
			playersColors_E88E0x[2][2] = 0x7B;
			playersColors_E88E0x[3][0] = 0x58;
			playersColors_E88E0x[3][2] = 0x7B;
			playersColors_E88E0x[4][0] = 0x97;
			playersColors_E88E0x[5][0] = 0xD7;
			playersColors_E88E0x[5][1] = 0xDD;
		}
		// shared between night and cave
		playersColors_E88E0x[3][1] = 0x5D;
		playersColors_E88E0x[4][1] = 0x9D;
		playersColors_E88E0x[4][2] = 0x7B;
		playersColors_E88E0x[5][2] = 0x7B;
		playersColors_E88E0x[6][0] = 0x69;
		playersColors_E88E0x[6][1] = 0x62;
		playersColors_E88E0x[6][2] = 0x7B;
		playersColors_E88E0x[7][0] = 0xC9;
		playersColors_E88E0x[7][1] = 0xCF;
		playersColors_E88E0x[7][2] = 0x7B;
		
	}
	else // Day
	{
		playersColors_E88E0x[0][0] = 0x60;
		playersColors_E88E0x[0][1] = 0x64;
		playersColors_E88E0x[0][2] = 0x7B;
		playersColors_E88E0x[1][0] = 0x7B;
		playersColors_E88E0x[1][1] = 0x77;
		playersColors_E88E0x[1][2] = 0x7B;
		playersColors_E88E0x[2][0] = 0x1C;
		playersColors_E88E0x[2][1] = 0x18;
		playersColors_E88E0x[2][2] = 0x7B;
		playersColors_E88E0x[3][0] = 0x5B;
		playersColors_E88E0x[3][1] = 0x57;
		playersColors_E88E0x[3][2] = 0x7B;
		playersColors_E88E0x[4][0] = 0x9A;
		playersColors_E88E0x[4][1] = 0x97;
		playersColors_E88E0x[4][2] = 0x7B;
		playersColors_E88E0x[5][0] = 0xDB;
		playersColors_E88E0x[5][1] = 0xD8;
		playersColors_E88E0x[5][2] = 0x7B;
		playersColors_E88E0x[6][0] = 0x76;
		playersColors_E88E0x[6][1] = 0xA0;
		playersColors_E88E0x[6][2] = 0x7B;
		playersColors_E88E0x[7][0] = 0x3D;
		playersColors_E88E0x[7][1] = 0x3A;
		playersColors_E88E0x[7][2] = 0x7B;
		
	}
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// E88E3: using guessed type char x_BYTE_E88E3;
// E88E4: using guessed type char x_BYTE_E88E4;
// E88E5: using guessed type char x_BYTE_E88E5;
// E88E6: using guessed type char x_BYTE_E88E6;
// E88E7: using guessed type char x_BYTE_E88E7;
// E88E8: using guessed type char x_BYTE_E88E8;
// E88E9: using guessed type char x_BYTE_E88E9;
// E88EA: using guessed type char x_BYTE_E88EA;
// E88EB: using guessed type char x_BYTE_E88EB;
// E88EC: using guessed type char x_BYTE_E88EC;
// E88ED: using guessed type char x_BYTE_E88ED;
// E88EE: using guessed type char x_BYTE_E88EE;
// E88EF: using guessed type char x_BYTE_E88EF;
// E88F0: using guessed type char x_BYTE_E88F0;
// E88F1: using guessed type char x_BYTE_E88F1;
// E88F2: using guessed type char x_BYTE_E88F2;
// E88F3: using guessed type char x_BYTE_E88F3;
// E88F4: using guessed type char x_BYTE_E88F4;
// E88F5: using guessed type char x_BYTE_E88F5;
// E88F6: using guessed type char x_BYTE_E88F6;
// E88F7: using guessed type char x_BYTE_E88F7;

//----- (0004A820) --------------------------------------------------------
type_entity_0x6E8E* SetPlayerStart0_4A820(axis_3d* position)//22b820
{
	world_D41A0.playerStarts_0x2362[0] = *position;
	return 0;
}

//----- (0004A840) --------------------------------------------------------
type_entity_0x6E8E* SetPlayerStart1_4A840(axis_3d* position)//22b840
{
	world_D41A0.playerStarts_0x2362[1] = *position;
	return 0;
}

//----- (0004A860) --------------------------------------------------------
type_entity_0x6E8E* SetPlayerStart2_4A860(axis_3d* position)//22b860
{
	world_D41A0.playerStarts_0x2362[2] = *position;
	return 0;
}

//----- (0004A880) --------------------------------------------------------
type_entity_0x6E8E* SetPlayerStart3_4A880(axis_3d* position)//22b880
{
	world_D41A0.playerStarts_0x2362[3] = *position;
	return 0;
}

//----- (0004A8A0) --------------------------------------------------------
type_entity_0x6E8E* SetPlayerStart4_4A8A0(axis_3d* position)//22b8a0
{
	world_D41A0.playerStarts_0x2362[4] = *position;
	return 0;
}

//----- (0004A8C0) --------------------------------------------------------
type_entity_0x6E8E* SetPlayerStart5_4A8C0(axis_3d* position)//22b8c0
{
	world_D41A0.playerStarts_0x2362[5] = *position;
	return 0;
}

//----- (0004A8E0) --------------------------------------------------------
type_entity_0x6E8E* SetPlayerStart6_4A8E0(axis_3d* position)//22b8e0
{
	world_D41A0.playerStarts_0x2362[6] = *position;
	return 0;
}

//----- (0004A900) --------------------------------------------------------
type_entity_0x6E8E* SetPlayerStart7_4A900(axis_3d* position)//22b900
{
	world_D41A0.playerStarts_0x2362[7] = *position;
	return 0;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;

//----- (0004A920) --------------------------------------------------------
type_entity_0x6E8E* AddPlayer_4A920(axis_3d* position)//22b920 - add player 0 //tex 0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 0;
		event->class_0x3F_63 = 3;
		event->model_0x40_64 = 0;
		event->maxLife_0x4 = 10000;
		event->minSpeed_0x84_132 = 80;
		event->mailMask_0x38_56 = 29;
		event->id_0x1A_26 = event - world_D41A0.entities_0x6E8E;
		if (world_D41A0.terrain_2FECE.MapType == MapType_t::Cave)
			event->typeParams_0xA0_160x = &str_D7BD6[104]; //(type_str_160*)&unk_D7BD6[0xdd0];
		else
			event->typeParams_0xA0_160x = &str_D7BD6[66]; //(type_str_160*)&unk_D7BD6[0x8c4];
		AddEventToMap_57D70(event, position);
		SetEntityIndexAndRot_49CD0(event, 44);
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

//----- (0004A9C0) --------------------------------------------------------
type_entity_0x6E8E* AddWizard_4A9C0(axis_3d* position)//22b9c0 //Other player spawn? //tex 0
{
	type_entity_0x6E8E* event = NewEvent_4A050();
	if (event)
	{
		event->actionIndex_0x45_69 = 1;
		event->class_0x3F_63 = 3;
		event->model_0x40_64 = 1;
		event->maxLife_0x4 = 10000;
		event->minSpeed_0x84_132 = 80;
		event->typeParams_0xA0_160x = &str_D7BD6[67]; //(type_str_160*)&unk_D7BD6[0x8e6];
		event->mailMask_0x38_56 = 29;
		event->id_0x1A_26 = event - world_D41A0.entities_0x6E8E;
		AddEventToMap_57D70(event, position);
		SetEntityIndexAndRot_49CD0(event, 44);
		CopyMaxLifeToLife_49A20(event);
	}
	return event;
}

int debugcounter_233d56 = 0;
//----- (00051BB0) --------------------------------------------------------
// ---------------------------------------------------------------------------
// Multiplayer scoreboard, for the result line shown when the game ends.
//
// Everything here is per match and lives only in memory: the game has no scoring of its own,
// the player structure carries no counter to hold one, and nothing about a knock-out is sent
// over the wire beyond the action that caused it.  That is why this is worked out on each node
// from the same inputs every node already runs - the simulations are in step, so each of them
// arrives at the same table and the players all see the same result.
//
// Order is "who lasted longest": whoever is still standing comes first, then the knocked-out
// in reverse order of when they went, so the last one to fall is ahead of the first.
// ---------------------------------------------------------------------------
static int  matchKills[8];        // opponents this player put down
static int  matchOutSeq[8];       // order of knock-outs, 0 = never went out
static bool matchPlayed[8];       // slots that took part at all
static int  matchOutCounter = 0;

// A player who leaves a network game does not take their castle with them.
//
// Nothing used to happen to it: the owner went back to the menu and the castle stayed
// standing on the map, still theirs, for the rest of the match.  Destroying it is what the
// game already does when a castle is lost (action 0x2A sets life = -1) and the mana it held
// is released by the castle's own death, so this only has to say "it is gone".
//
// Every node runs this for the same player at the same point - leaving is an action that
// travels over the wire, and a node that vanished is noticed by all the others - so the
// castle disappears on every screen without anything extra being sent.
void DestroyPlayerCastle(int player)
{
	if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)) return;
	if (player < 0 || player >= 8) return;
	type_entity_0x6E8E* owner = Entities_EA3E4[world_D41A0.players_0x2BDE[player].playerIndex_0x00a_2BE4_11240];
	if (!owner || !owner->playerData_0xA4_164x) return;
	const int16_t castle = owner->playerData_0xA4_164x->CastleEntityIndex_0x3A_58;
	if (!castle) return;
	if (Entities_EA3E4[castle] > Entities_EA3E4[0])
	{
		// A stage at a time, the way the game takes a castle apart.  ShrinkCastleMana_605E0 removes one
		// level per call and, on the call that brings the count to zero, clears the owner's
		// castle and stops drawing it - that last step is what makes the castle actually go.
		//
		// Setting life = -1 once, which is all action 0x2A does, costs exactly ONE level: a
		// level 3 castle came back as a level 2 one, which is what the tester saw.
		if (CommandLineParams.DoNetworkDebug())
			debug_net_printf("CASTLE: player %d left, taking down its castle (%d level(s))\n",
				player + 1, (int)Entities_EA3E4[castle]->param_0x10_16);
		// Bounded: the loop trusts a counter that lives in game state, and a castle has
		// nothing like eight levels.
		int guard = 16;
		while (Entities_EA3E4[castle] > Entities_EA3E4[0]
			&& Entities_EA3E4[castle]->param_0x10_16 > 0 && guard-- > 0)
		{
			ShrinkCastleMana_605E0(Entities_EA3E4[castle]);
		}
		// A castle already at level 0 still has to be cleared away, and that is the same call.
		if (Entities_EA3E4[castle] > Entities_EA3E4[0])
			ShrinkCastleMana_605E0(Entities_EA3E4[castle]);
	}
}

void MatchScoreReset()
{
	for (int i = 0; i < 8; i++) { matchKills[i] = 0; matchOutSeq[i] = 0; matchPlayed[i] = false; }
	matchOutCounter = 0;
}

// Both of these are called from paths a single-player game runs too, so they check the mode
// themselves: the scoreboard exists for the multiplayer result line and must not so much as
// record anything outside it.
void MatchScoreMarkPlaying(int player)
{
	if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)) return;
	if (player >= 0 && player < 8) matchPlayed[player] = true;
}

// Credits the kill and remembers when the victim went.  The killer is read off the victim:
// applying damage stores the index of the entity that landed the hit in the victim's
// word_0x26_38 (see CreatureFollowParent_1B6B0, where str_0x5E_94.word_0x62_98 is copied into it), and that
// entity's owner is the player behind it - directly for a wizard, through the owner index for
// anything it cast.  A victim that fell to something with no owner (its own doing, terrain)
// still gets its place in the order; nobody is credited.
void MatchScoreRecordKnockOut(int victim, type_entity_0x6E8E* victimEntity)
{
	if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)) return;
	if (victim < 0 || victim >= 8) return;
	if (matchOutSeq[victim]) return;                 // already counted, this can be re-entered
	matchPlayed[victim] = true;
	matchOutSeq[victim] = ++matchOutCounter;

	if (!victimEntity) return;
	const int16_t hitBy = victimEntity->hitById_0x26_38;
	if (hitBy <= 0) return;
	type_entity_0x6E8E* killerEntity = Entities_EA3E4[hitBy];
	if (!killerEntity) return;
	if (!killerEntity->playerData_0xA4_164x)              // a projectile: follow it back to its owner
		killerEntity = Entities_EA3E4[killerEntity->id_0x1A_26];
	if (!killerEntity || !killerEntity->playerData_0xA4_164x) return;

	const int killer = killerEntity->playerData_0xA4_164x->playerColorIndex_0x38_56;
	if (killer < 0 || killer >= 8 || killer == victim) return;   // suicide credits nobody
	matchKills[killer]++;
	matchPlayed[killer] = true;
	if (CommandLineParams.DoNetworkDebug())
		debug_net_printf("SCORE: player %d put down player %d (now %d)\n",
			killer + 1, victim + 1, matchKills[killer]);
}

// "Player 1(killed 2), Player 2(killed 0), Player 3(killed 0)"
std::string MatchScoreResultLine()
{
	int order[8], n = 0;
	for (int i = 0; i < 8; i++) if (matchPlayed[i]) order[n++] = i;

	// Still standing first (their sequence is 0), then the latest knock-out down to the first.
	for (int a = 0; a < n; a++)
		for (int b = a + 1; b < n; b++) {
			const int sa = matchOutSeq[order[a]] ? matchOutSeq[order[a]] : 0x7FFFFFFF;
			const int sb = matchOutSeq[order[b]] ? matchOutSeq[order[b]] : 0x7FFFFFFF;
			if (sb > sa) { const int t = order[a]; order[a] = order[b]; order[b] = t; }
		}

	std::string line;
	for (int i = 0; i < n; i++) {
		char part[64];
		snprintf(part, sizeof(part), "Player %d (killed %d)\n", order[i] + 1, matchKills[order[i]]);
		line += part;
	}
	return line;
}

void PlayerEvents_51BB0()//232bb0
{
	type_entity_0x6E8E* ifsEvent;
	int16_t spellIndex;
	type_entity_0x6E8E* actEvent;
	bool useSound;

	// Playing a recording back in a network game: only the node that holds the token may
	// replay, and it has to do so BEFORE the exchange below.  The recorded inputs then
	// travel over the wire like any other input, so every node runs on exactly the same
	// array and the simulations stay in step.
	//
	// Replaying on each node separately does not work: the lookup below is indexed by
	// that node's own turn counter (array_0x2BDE[own index].Turn), and those counters are
	// not kept in sync between nodes, so the same "turn" is a different moment on each
	// machine.  Feeding the array in after the exchange would also overwrite the only
	// thing keeping the nodes together.
	// Scripted endings for unattended tests: leave the game, or drop the network link to
	// see how the other side copes with a peer that vanishes.
	if ((gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
		&& (CommandLineParams.QuitAfterS() > 0 || CommandLineParams.NetKillAfterS() > 0))
	{
		static long inGameSince = 0;
		if (inGameSince == 0) inGameSince = (long)j___clock();
		long secondsInGame = ((long)j___clock() - inGameSince) / 100;

		if (CommandLineParams.NetKillAfterS() > 0 && secondsInGame >= CommandLineParams.NetKillAfterS()) {
			static bool linkDropped = false;
			if (!linkDropped) {
				linkDropped = true;
				if (CommandLineParams.DoNetworkDebug())
					debug_net_printf("AUTOTEST: dropping the network link now\n");
				EndMyNetLib();
			}
		}
		if (CommandLineParams.QuitAfterS() > 0 && secondsInGame >= CommandLineParams.QuitAfterS()) {
			static bool leftGame = false;
			if (!leftGame) {
				leftGame = true;
				if (CommandLineParams.DoNetworkDebug())
					debug_net_printf("AUTOTEST: leaving the game now\n");
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].syncReady_0x004_2BE0_11234 = 1;
			}
		}
	}

	// End this match and go round again, for the multi-match test.
	//
	// Deliberately NOT byte_0x004_2BE0_11234, which is what --quit_after sets: that one is
	// tested at the top of MainLoop_46830 as well, so it leaves the application
	// altogether.  Bit 3 of dw_w_b_0_2BDE_11230.byte[2] only breaks InGameLoop_47320, and the
	// outer while(1) then runs MenusAndIntros_76930 again - which is exactly the path a
	// player takes back to the menu, and the one that has to rebuild the network state.
	if ((gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
		&& CommandLineParams.AutoTest() && CommandLineParams.AutoTestMatchSeconds() > 0
		&& CommandLineParams.AutoTestMatches() > 1)
	{
		static long matchSince = 0;
		static int  matchInProgress = -1;
		if (matchInProgress != g_autotest_match) { matchInProgress = g_autotest_match; matchSince = 0; }
		if (matchSince == 0) matchSince = (long)j___clock();

		if (((long)j___clock() - matchSince) / 100 >= CommandLineParams.AutoTestMatchSeconds())
		{
			if (g_autotest_match + 1 < CommandLineParams.AutoTestMatches())
			{
				if (CommandLineParams.DoNetworkDebug())
					debug_net_printf("AUTOTEST: match %d over, returning to the menu for match %d\n",
						g_autotest_match, g_autotest_match + 1);
				g_autotest_match++;
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] |= 8;
			}
			else
			{
				if (CommandLineParams.DoNetworkDebug())
					debug_net_printf("AUTOTEST: last match (%d) over, leaving\n", g_autotest_match);
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].syncReady_0x004_2BE0_11234 = 1;
			}
		}
	}

	bool replayingOverNetwork = false;
	if (m_InputRecorder != nullptr && m_InputRecorder->m_IsPlaying
		&& (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE))
	{
		replayingOverNetwork = true;
		if (GetIndexNetwork_74536() == GetIndexNetwork2_74515())
		{
			int32_t turn = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248;
			for (int pi = 0; pi < world_D41A0.NumberOfPlayers_0xe; pi++)
			{
				RecordedEventTurn* rec = m_InputRecorder->GetCurrentPlayerActions(
					gameState_D41A4.levelnumber_43w, pi, turn);
				if (rec && rec->Bytes)
				{
					size_t n = (size_t)rec->SizeBytes;
					if (n > sizeof(Type_PlayerInput_0x6E3E)) n = sizeof(Type_PlayerInput_0x6E3E);
					memcpy(&world_D41A0.playerInputs_0x6E3E[pi], rec->Bytes, n);
				}
			}
		}
	}

	if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
	{
		NetworkUpdateConnections2_74374();
		ReceiveSendAll_7438A((uint8_t*)world_D41A0.playerInputs_0x6E3E, sizeof(Type_PlayerInput_0x6E3E));//multi receive


		bool playerFound = false;
		for (int i = 0; i < world_D41A0.NumberOfPlayers_0xe; i++)
		{
			if (world_D41A0.playerInputs_0x6E3E[i].PlayerAction_byte0 == 1)
			{
				world_D41A0.players_0x2BDE[i].playerActive_0x006_2BE4_11236 = 1;
				if (i == world_D41A0.LevelIndex_0xc && (gameState_D41A4.player_name_57ar[0]))
				{
					strcpy(world_D41A0.players_0x2BDE[i].WizardName_0x39f_2BFA_12157, gameState_D41A4.player_name_57ar); //copy wizard name
				}
				playerFound = true;
			}
		}
		if (playerFound)
			ReceiveSendAll_7438A((uint8_t*)&world_D41A0.players_0x2BDE[0], sizeof(type_str_0x2BDE));
	}
	world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248++;
	gameState_D41A4.FrameTimingIndex_26++;//232c9e
	gameState_D41A4.setting_30++;
	if (gameState_D41A4.leftSpellPlayerIndex_38400)
		gameState_D41A4.leftSpellPlayerIndex_38400--;
	if (gameState_D41A4.rightSpellPlayerIndex_38401)
		gameState_D41A4.rightSpellPlayerIndex_38401--;
	//adress 232cd2
	for (int i = 1; i < 16; i++)
	{
		gameState_D41A4.colorIndex_121[i] = world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248 / i & 1;
	}
	for (int i = 0; i < world_D41A0.NumberOfPlayers_0xe; i++)
	{
		// !replayingOverNetwork: in a network game the recording was already applied
		// above, before the exchange, so the array now holds what every node agreed on.
		if (!replayingOverNetwork && m_InputRecorder != nullptr && m_InputRecorder->m_IsPlaying && m_InputRecorder->GetCurrentPlayerActions(gameState_D41A4.levelnumber_43w, i, world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248) != nullptr)
		{
			if (CommandLineParams.ModeRegressionsTestType()==-1)
			{
				std::string msg = "Playing Turn: " + std::to_string(world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248);
				SetCurrentNotificationMessage_19760(msg.c_str(), 3u, 50);
			}
			memcpy(&world_D41A0.playerInputs_0x6E3E[i], m_InputRecorder->GetCurrentPlayerActions(gameState_D41A4.levelnumber_43w, i, world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248)->Bytes, m_InputRecorder->GetCurrentPlayerActions(gameState_D41A4.levelnumber_43w, i, world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248)->SizeBytes);
			// the recorded rand_0x8 only checks the playback: the first turn of a level it differs in is logged
			static int randDiffersLevel = -1;
			const uint32_t recordedRand = m_InputRecorder->GetCurrentPlayerActions(gameState_D41A4.levelnumber_43w, i, world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248)->Rand;
			if (recordedRand != 0 && recordedRand != world_D41A0.rand_0x8 && randDiffersLevel != gameState_D41A4.levelnumber_43w)
			{
				Logger->warn("Playback differs from the recording: level {} turn {} rand {:X}, recorded {:X}", gameState_D41A4.levelnumber_43w, world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248, world_D41A0.rand_0x8, recordedRand);
				randDiffersLevel = gameState_D41A4.levelnumber_43w;
			}
		}
		else if (m_InputRecorder != nullptr && m_InputRecorder->m_IsRecording)
		{
			std::string msg = "Recording Turn: " + std::to_string(world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248);
			SetCurrentNotificationMessage_19760(msg.c_str(), 3u, 50);
			m_InputRecorder->RecordPlayerActions(gameState_D41A4.levelnumber_43w, i, world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248, world_D41A0.rand_0x8, sizeof(Type_PlayerInput_0x6E3E), (uint8_t*)&world_D41A0.playerInputs_0x6E3E[i]);
			// recording started in a running level (a loaded game): 54A50 never ran, take the spells now
			if (!m_InputRecorder->HasPlayerSpells(gameState_D41A4.levelnumber_43w, i))
				m_InputRecorder->RecordPlayerSpells(gameState_D41A4.levelnumber_43w, i,
					world_D41A0.players_0x2BDE[i].playerData_0x3E6_2BE4_12228.str_611.SpellsEnabled_0x333_819x.SpellEnabled,
					world_D41A0.players_0x2BDE[i].playerData_0x3E6_2BE4_12228.str_611.SpellIndexes_0x39B_923x.SpellIndex,
					world_D41A0.players_0x2BDE[i].playerData_0x3E6_2BE4_12228.str_611.SpellLevels_0x41D_1053z.SpellIndex,
					world_D41A0.players_0x2BDE[i].playerData_0x3E6_2BE4_12228.str_611.SpellExperience_0x263_611x.SpellExperience);
		}

		//adress 232d2f
		actEvent = Entities_EA3E4[world_D41A0.players_0x2BDE[i].playerIndex_0x00a_2BE4_11240];
		// Anybody the loop runs for is in this match, which is how the result table knows which
		// slots to list - the array has eight of them and a game rarely fills it.
		MatchScoreMarkPlaying(i);
		if (gameState_D41A4.setting_byte1_22 & 0x20)
		{
			RecordOrPlayMovieInput_53A40(&world_D41A0.playerInputs_0x6E3E[i]);
		}
		//adress 233d56
		switch (world_D41A0.playerInputs_0x6E3E[i].PlayerAction_byte0)
		{
		case 1:
			world_D41A0.players_0x2BDE[i].playerActive_0x006_2BE4_11236 = 1;
			if (!(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
				&& i == world_D41A0.LevelIndex_0xc
				&& *gameState_D41A4.player_name_57ar)
			{
				strcpy(world_D41A0.players_0x2BDE[i].WizardName_0x39f_2BFA_12157, gameState_D41A4.player_name_57ar);
			}
			UpdatePlayerWizard_5C950(&world_D41A0.players_0x2BDE[i], actEvent);
			SetCenterScreenForFlyAssistant_6EDB0();
			BackupMousePosition_548F0(&world_D41A0.players_0x2BDE[i]);
			SetMenuCursorPosition_52E90(&world_D41A0.players_0x2BDE[i], 0, false);
			break;
		case 2:
			CheckCdPresent_53120();
			if (world_D41A0.LevelIndex_0xc == i)
				world_D41A0.players_0x2BDE[i].syncReady_0x004_2BE0_11234 = 1;
			if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
				DisableEntitesDrawing_5E660(actEvent);
			MatchScoreRecordKnockOut(i, actEvent);
			DestroyPlayerCastle(i);
			NetworkEvent_7373D(i);
			world_D41A0.players_0x2BDE[i].playerActive_0x006_2BE4_11236 = 0;
			break;
		case 3:
			UpdatePlayerWizard_5C950(&world_D41A0.players_0x2BDE[i], actEvent);
			SetCenterScreenForFlyAssistant_6EDB0();
			BackupMousePosition_548F0(&world_D41A0.players_0x2BDE[i]);
			SetMenuCursorPosition_52E90(&world_D41A0.players_0x2BDE[i], 0, false);
			break;
		case 4:
			world_D41A0.players_0x2BDE[i].ShowDebugTextFlag_0x005_2BE0_11235 ^= world_D41A0.players_0x2BDE[i].stateFlags_0_2BDE_11230.byte[1];
			break;
		case 6: //Move player
			if (world_D41A0.playerInputs_0x6E3E[i].entityIndex_0x6E3E_byte5 & 0x40)
				actEvent->playerData_0xA4_164x->str_611.spellIndex_0x458_1112 = world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1;
			if (world_D41A0.playerInputs_0x6E3E[i].entityIndex_0x6E3E_byte5 < 0 && !world_D41A0.players_0x2BDE[i].rollState_0x846_2BDE)
				world_D41A0.players_0x2BDE[i].rollState_0x846_2BDE = 1;
			break;
		case 7:
			if (world_D41A0.players_0x2BDE[i].ActPlayerIndex_0x00e_2BDE_11244 + world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1 >= 0
				&& world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1 + world_D41A0.players_0x2BDE[i].ActPlayerIndex_0x00e_2BDE_11244 < world_D41A0.players_0x2BDE[i].positionCount_0x010_2BDE_11246 - 1)
			{
				world_D41A0.players_0x2BDE[i].ActPlayerIndex_0x00e_2BDE_11244 += world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1;
			}
			break;
		case 8:
			world_D41A0.players_0x2BDE[i].positions_0x1d1_2BDE_11695[0].rotation__2BDE_11701.fov += world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1;
			break;
		case 0xC:
			gameState_D41A4.setting_byte1_22 = (gameState_D41A4.setting_byte1_22 | 0x22) & 0xFB;
			break;
		case 0xD:
			gameState_D41A4.setting_byte1_22 = (gameState_D41A4.setting_byte1_22 | 0x24) & 0xFD;
			break;
		case 0xF:
			if (actEvent->playerData_0xA4_164x->CastleEntityIndex_0x3A_58)
			{
				UpdatePlayerWizard_5C950(&world_D41A0.players_0x2BDE[i], actEvent);
				SetCenterScreenForFlyAssistant_6EDB0();
				BackupMousePosition_548F0(&world_D41A0.players_0x2BDE[i]);
				SetMenuCursorPosition_52E90(&world_D41A0.players_0x2BDE[i], 0, false);
				break;
			}
			if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
			{
				strcpy(world_D41A0.players_0x2BDE[i].CurrentNotificationText_0x01c_2BFA_11258, langStrings_E9C4C[HAS_BEEN_BANISHED]);//has been banished from the realm.
				world_D41A0.players_0x2BDE[i].endState_0x04f_2C2D_11309 = 1;
				world_D41A0.players_0x2BDE[i].CurrentNotificationDuration_0x04d_2C2B_11307 = 100;
				world_D41A0.players_0x2BDE[i].stateFlags_0_2BDE_11230.word[1] = 8;
				MatchScoreRecordKnockOut(i, actEvent);
				NetworkEvent_7373D(i);
				world_D41A0.players_0x2BDE[i].playerActive_0x006_2BE4_11236 = 0;
				if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
					DisableEntitesDrawing_5E660(actEvent);
				gameState_D41A4.setting_byte1_22 &= 0xD9u;
				if (gameState_D41A4.levelnumber_43w > 24)
					world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] |= 0x10u;
				break;
			}
			world_D41A0.players_0x2BDE[i].stateFlags_0_2BDE_11230.byte[2] |= 0xCu;
			UpdatePlayerWizard_5C950(&world_D41A0.players_0x2BDE[i], actEvent);
			SetCenterScreenForFlyAssistant_6EDB0();
			BackupMousePosition_548F0(&world_D41A0.players_0x2BDE[i]);
			SetMenuCursorPosition_52E90(&world_D41A0.players_0x2BDE[i], 0, false);
			break;
		case 0x10:
			if (world_D41A0.flyAssistant_0x36DEA && i == world_D41A0.LevelIndex_0xc)
			{
				world_D41A0.playerInputs_0x6E3E[i].roll = 0;
				world_D41A0.playerInputs_0x6E3E[i].pitch = 0;
				SetCenterScreenForFlyAssistant_6EDB0();
			}
			SetMenuCursorPosition_52E90(&world_D41A0.players_0x2BDE[i], 3, true);
			// Opening the panel starts a fresh message, so the slot it types into starts empty.
			//
			// Sending (0x13) leaves the text in names_81, and only zeroing the counter here
			// meant the box came back up showing the message just sent, with the caret at the
			// end of it - the text looked editable, but the first keypress dropped the whole
			// line (loc_520C4 below: names_81[slot][0] = 0 while the counter is still 0).  That
			// deferred wipe is what players report as "the previous message is still there and
			// it blanks as I start typing".  Clearing here makes the box show what typing will
			// actually produce.
			//
			// The eight slots stay usable: selecting one (0x23) does NOT clear, so picking a
			// slot with Shift+F1..F8 and pressing Enter still sends what is stored in it.  Only
			// the slot the panel opens on is treated as the scratch line.
			world_D41A0.players_0x2BDE[i].names_81[world_D41A0.players_0x2BDE[i].chatLineIndex_0x3E0_2BE4_12222][0] = 0;
			world_D41A0.players_0x2BDE[i].chatLength_0x3E2_2BE4_12224 = 0;
			if (i == world_D41A0.LevelIndex_0xc)
			{
				DrawLoadingBar_54960();
			}
			break;
		case 0x11:
			if (world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1 == 8)
			{
				// Backspace removes the last character: loc_520C4 zeroes names_81[len - 1].
				// Taking one off that (as this used to) ate two characters at a time, and on a
				// one-character message it wrote the byte in front of the string - which is
				// word_0x04f_2C2D_11309, the notification type.
				if (strlen(world_D41A0.players_0x2BDE[i].names_81[world_D41A0.players_0x2BDE[i].chatLineIndex_0x3E0_2BE4_12222]) != 0)
				{
					world_D41A0.players_0x2BDE[i].chatLength_0x3E2_2BE4_12224--;
					world_D41A0.players_0x2BDE[i].names_81[world_D41A0.players_0x2BDE[i].chatLineIndex_0x3E0_2BE4_12222][strlen(world_D41A0.players_0x2BDE[i].names_81[world_D41A0.players_0x2BDE[i].chatLineIndex_0x3E0_2BE4_12222]) - 1] = 0;
				}
			}
			else if (world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1)
			{
				if (strlen(world_D41A0.players_0x2BDE[i].names_81[world_D41A0.players_0x2BDE[i].chatLineIndex_0x3E0_2BE4_12222]) + 1 - 1 < 43)
				{
					if (!world_D41A0.players_0x2BDE[i].chatLength_0x3E2_2BE4_12224)
						world_D41A0.players_0x2BDE[i].names_81[world_D41A0.players_0x2BDE[i].chatLineIndex_0x3E0_2BE4_12222][0] = 0;
					world_D41A0.players_0x2BDE[i].chatLength_0x3E2_2BE4_12224++;
					sprintf(printbuffer, "%c", world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1);
					strcpy(&world_D41A0.players_0x2BDE[i].names_81[world_D41A0.players_0x2BDE[i].chatLineIndex_0x3E0_2BE4_12222][strlen(world_D41A0.players_0x2BDE[i].names_81[world_D41A0.players_0x2BDE[i].chatLineIndex_0x3E0_2BE4_12222])], printbuffer);
				}
			}
			break;
		case 0x12:
			SetMenuCursorPosition_52E90(&world_D41A0.players_0x2BDE[i], 0, true);
			break;
		case 0x14: //Exit menu
			if (world_D41A0.flyAssistant_0x36DEA && i == world_D41A0.LevelIndex_0xc)
			{
				switch (world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1)
				{
				case 3:
				case 5:
				case 8:
				case 9:
				case 0xA:
				case 0xB:
				case 0xC:
				case 0xD:
				case 0xE:
					world_D41A0.playerInputs_0x6E3E[i].roll = 0;
					world_D41A0.playerInputs_0x6E3E[i].pitch = 0;
					SetCenterScreenForFlyAssistant_6EDB0();
					break;
				default:
					break;
				}
			}
			SetMenuCursorPosition_52E90(&world_D41A0.players_0x2BDE[i], world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1, true, gameUiScale);
			break;
		case 0x17:
			if (world_D41A0.playerInputs_0x6E3E[i].param2_0x6E3E_byte2 != -1)
			{
				for (int k = 0; k < 26; k++)
				{
					if (actEvent->playerData_0xA4_164x->str_611.SpellIndexes_0x39B_923x.SpellIndex[k] == world_D41A0.playerInputs_0x6E3E[i].param2_0x6E3E_byte2)
						actEvent->playerData_0xA4_164x->str_611.SpellIndexes_0x39B_923x.SpellIndex[k] = 0xff;
				}
				actEvent->playerData_0xA4_164x->str_611.SpellIndexes_0x39B_923x.SpellIndex[world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1] = world_D41A0.playerInputs_0x6E3E[i].param2_0x6E3E_byte2;
				PrepareEventSound_6E450(0, world_D41A0.players_0x2BDE[i].playerSlot_0x007_2BE4_11237, 14);
			}
			break;
		case 0x19:
		case 0x1A:
			for (int k = 0; k < world_D41A0.NumberOfPlayers_0xe; k++)
			{
				if (world_D41A0.players_0x2BDE[k].playerActive_0x006_2BE4_11236)
				{
					world_D41A0.players_0x2BDE[k].syncReady_0x004_2BE0_11234 = 1;
					NetworkEvent_7373D(k);
				}
			}
			break;
		case 0x1B:
			CheckCdPresent_53120();
			world_D41A0.players_0x2BDE[i].stateFlags_0_2BDE_11230.word[1] = 10;
			NetworkEvent_7373D(i);
			break;
		case 0x1C:
			world_D41A0.players_0x2BDE[i].stateFlags_0_2BDE_11230.word[1] = 12;
			NetworkEvent_7373D(i);
			break;
		case 0x1D: //Banished
			strcpy(world_D41A0.players_0x2BDE[i].CurrentNotificationText_0x01c_2BFA_11258, langStrings_E9C4C[HAS_BEEN_BANISHED]);//has been banished from the realm.
			world_D41A0.players_0x2BDE[i].endState_0x04f_2C2D_11309 = 1;
			world_D41A0.players_0x2BDE[i].CurrentNotificationDuration_0x04d_2C2B_11307 = 100;
			world_D41A0.players_0x2BDE[i].stateFlags_0_2BDE_11230.word[1] = 8;//SKIP FROM GAMELOOP
			NetworkEvent_7373D(i);
			world_D41A0.players_0x2BDE[i].playerActive_0x006_2BE4_11236 = 0;
			if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
				DisableEntitesDrawing_5E660(actEvent);
			gameState_D41A4.setting_byte1_22 &= 0xD9u;
			if (gameState_D41A4.levelnumber_43w > 24)
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] |= 0x10u;
			break;
		case 0x1E: //Cheat
			CheckCdPresent_53120();
			switch (world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1)
			{
			case 1:
				GiveAllSpells_Cheat(actEvent);
				ShowMessage_52D70(i, (char*)".. CHEAT: access all spells");
				break;
			case 2:
				ifsEvent = CreateEntity_4A190(&actEvent->position_0x4C_76, 10, 39);
				if (ifsEvent)
				{
					ifsEvent->mana_0x90_144 = 100000;
					ifsEvent->playerEntityIndex_0x94_148 = actEvent - world_D41A0.entities_0x6E8E;
				}
				actEvent->mana_0x90_144 = actEvent->maxMana_0x8C_140;
				ShowMessage_52D70(i, (char*)".. CHEAT: more mana");
				break;
			case 3:
				for (type_entity_0x6E8E* evLoop = gameState_D41A4.class3List_38519; evLoop > Entities_EA3E4[0]; evLoop = evLoop->next_0)
				{
					if (evLoop->id_0x1A_26 != actEvent->id_0x1A_26)
					{
						if (!evLoop->model_0x40_64 || evLoop->model_0x40_64 == 1)
							evLoop->life_0x8 = -1;
					}
				}
				ShowMessage_52D70(i, (char*)".. CHEAT: destroy all players");
				break;
			case 4:
				for (type_entity_0x6E8E* evLoop = gameState_D41A4.class3List_38519; evLoop > Entities_EA3E4[0]; evLoop = evLoop->next_0)
				{
					if (evLoop->id_0x1A_26 != actEvent->id_0x1A_26 && evLoop->model_0x40_64 == 2)
						evLoop->life_0x8 = -1;
				}
				ShowMessage_52D70(i, (char*)".. CHEAT: destroy all castles");
				break;
			case 5:
				for (type_entity_0x6E8E* evLoop = gameState_D41A4.class3List_38519; evLoop > Entities_EA3E4[0]; evLoop = evLoop->next_0)
				{
					if (evLoop->id_0x1A_26 != actEvent->id_0x1A_26 && evLoop->model_0x40_64 == 3)
						evLoop->life_0x8 = -1;
				}
				ShowMessage_52D70(i, (char*)".. CHEAT: destroy all balloons");
				break;
			case 6:
				actEvent->life_0x8 = actEvent->maxLife_0x4;
				ShowMessage_52D70(i, (char*)".. CHEAT: heal");
				break;
			case 7:
				KillAllCreatures_1B5F0();
				ShowMessage_52D70(i, (char*)".. CHEAT: Kill all creatures");
				break;
			case 8:
				for (int k = 0; k < 26; k++)
				{
					actEvent->playerData_0xA4_164x->str_611.spellsExperience_0x2CB_715x.at(spellIndex_D94FF[k]) += 100;
				}
				GrantSpellToPlayer_6DB50(0, 0);
				ShowMessage_52D70(i, (char*)".. CHEAT: More Spell Experience Points");
				break;
			case 9:
				if (gameState_D41A4.OptionsSettingFlag_24 & 0x20)
				{
					gameState_D41A4.OptionsSettingFlag_24 &= 0xDF;
					ShowMessage_52D70(i, (char*)".. CHEAT: Free Spell Usage OFF");
				}
				else
				{
					gameState_D41A4.OptionsSettingFlag_24 |= 0x20;
					ShowMessage_52D70(i, (char*)".. CHEAT: Free Spell Usage ON");
				}
				break;
			case 0xA:
				if (gameState_D41A4.setting_byte4_25 & 1) {
					gameState_D41A4.setting_byte4_25 &= 0xFE;
					ShowMessage_52D70(i, (char*)".. CHEAT: Invincability ON");
				}
				else {
					gameState_D41A4.setting_byte4_25 |= 1;
					ShowMessage_52D70(i, (char*)".. CHEAT: Invincability OFF");
				}
				break;
			default:
				break;
			}
			break;
		case 0x1F:
		case 0x20: //Change Spell
			if (world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1 != -1)
			{
				PrepareEventSound_6E450(0, world_D41A0.players_0x2BDE[i].playerSlot_0x007_2BE4_11237, 14);
				CloseHelpPopup_87C10();
				spellIndex = spellIndex_D94FF[world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1];
				actEvent->playerData_0xA4_164x->str_611.selectedSubSpells_0x437_1079x.SpellIndex[spellIndex_D94FF[world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1]] = world_D41A0.playerInputs_0x6E3E[i].param2_0x6E3E_byte2;
				if (world_D41A0.playerInputs_0x6E3E[i].PlayerAction_byte0 >= 0x1Fu)
				{
					if (world_D41A0.playerInputs_0x6E3E[i].PlayerAction_byte0 <= 0x1Fu)
					{
						actEvent->playerData_0xA4_164x->str_611.SpellIndexLeft_0x451_1105 = spellIndex;
						actEvent->playerData_0xA4_164x->str_611.SubSpellIndexLeft_1109 = world_D41A0.playerInputs_0x6E3E[i].param2_0x6E3E_byte2;
						gameState_D41A4.leftSpellPlayerIndex_38400 = 8;
					}
					else if (world_D41A0.playerInputs_0x6E3E[i].PlayerAction_byte0 == 32)
					{
						actEvent->playerData_0xA4_164x->str_611.SpellIndexRight_0x453_1107 = spellIndex;
						actEvent->playerData_0xA4_164x->str_611.SubSpellIndexRight_1110 = world_D41A0.playerInputs_0x6E3E[i].param2_0x6E3E_byte2;
						gameState_D41A4.rightSpellPlayerIndex_38401 = 8;
					}
				}
				SetSpell_6D5E0(Entities_EA3E4[actEvent->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex]], world_D41A0.playerInputs_0x6E3E[i].param2_0x6E3E_byte2);
				CopyAxisForSpellWithLife_6D830(Entities_EA3E4[actEvent->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[spellIndex]], world_D41A0.playerInputs_0x6E3E[i].param2_0x6E3E_byte2);
				if (i == world_D41A0.LevelIndex_0xc)
				{
					strcpy(world_D41A0.players_0x2BDE[i].CurrentNotificationText_0x01c_2BFA_11258, langStrings_E9C4C[SPELLS_BEGIN_BUFFER_str[spellIndex].subspell[world_D41A0.playerInputs_0x6E3E[i].param2_0x6E3E_byte2].hintText_0x16x]);
					world_D41A0.players_0x2BDE[i].CurrentNotificationDuration_0x04d_2C2B_11307 = 20;
					world_D41A0.players_0x2BDE[i].endState_0x04f_2C2D_11309 = 3;
				}
				if (gameState_D41A4.setting_byte1_22 & 4)
				{
					if (i == world_D41A0.LevelIndex_0xc)
					{
						MoveCursorToSelectedSpell_6D200(&world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc]);
						FlvInitSet_473B0();
					}
				}
			}
			break;
		case 0x23:
			world_D41A0.players_0x2BDE[i].chatLength_0x3E2_2BE4_12224 = 0;
			world_D41A0.players_0x2BDE[i].chatLineIndex_0x3E0_2BE4_12222 = world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1;
			break;
		case 0x24:
			world_D41A0.players_0x2BDE[i].chatState_0x3E1_2BE4_12223 = world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1;
			break;
		case 0x25:
			if (world_D41A0.players_0x2BDE[i].chatState_0x3E1_2BE4_12223 == 3)
				world_D41A0.players_0x2BDE[i].chatRecipients_0x3E3_2BE4_12225 ^= 1 << world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1;
			break;
		case 0x26:
			actEvent->playerData_0xA4_164x->str_611.spellSlotTypes_0x3B5_949x.SpellIndex[world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1] = world_D41A0.playerInputs_0x6E3E[i].param2_0x6E3E_byte2;
			PrepareEventSound_6E450(0, world_D41A0.players_0x2BDE[i].playerSlot_0x007_2BE4_11237, 14);
			break;
		case 0x27: //Full stop player
			world_D41A0.playerInputs_0x6E3E[i].roll = 0;
			world_D41A0.playerInputs_0x6E3E[i].pitch = 0;
			if (world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1)
			{
				actEvent->actSpeed_0x82_130 = 0;
				actEvent->playerData_0xA4_164x->speed_0xc_12 = 0;
				if (actEvent->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[3])
					Entities_EA3E4[actEvent->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[3]]->stage_0x2E_46 = 0;
			}
			if (world_D41A0.players_0x2BDE[i].playerSlot_0x007_2BE4_11237 == world_D41A0.LevelIndex_0xc)
				SetCenterScreenForFlyAssistant_6EDB0();
			break;
		case 0x28: //Select Spell
			if (gameState_D41A4.setting_byte1_22 & 4)
			{
				actEvent->playerData_0xA4_164x->str_611.spellIndex_0x458_1112 = world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1;
				actEvent->playerData_0xA4_164x->str_611.selectSpellType_0x457_1111 = world_D41A0.playerInputs_0x6E3E[i].param2_0x6E3E_byte2;
				if (i == world_D41A0.LevelIndex_0xc)
				{
					MoveCursorToSelectedSpell_6D200(&world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc]);
					FlvInitSet_473B0();
				}
			}
			break;
		case 0x29:
			if (gameState_D41A4.setting_byte1_22 & 4)
			{
				actEvent->playerData_0xA4_164x->str_611.subSpellIndex_0x459_1113 = world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1;
				actEvent->playerData_0xA4_164x->str_611.selectedSubSpells_0x437_1079x.SpellIndex[spellIndex_D94FF[actEvent->playerData_0xA4_164x->str_611.spellIndex_0x458_1112]] = world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1;
				if (i == world_D41A0.LevelIndex_0xc)
				{
					MoveCursorToSelectedSpell_6D200(&world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc]);
					FlvInitSet_473B0();
				}
			}
			break;
		case 0x2A:
			if (Entities_EA3E4[actEvent->playerData_0xA4_164x->CastleEntityIndex_0x3A_58] > Entities_EA3E4[0])
			{
				if (Entities_EA3E4[actEvent->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]->param_0x10_16 == 1)
					actEvent->playerData_0xA4_164x->castleBuilding_0x1BE_446 = 1;
				Entities_EA3E4[actEvent->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]->life_0x8 = -1;
			}
			break;
		case 0x2B:
			world_D41A0.players_0x2BDE[i].chatLineIndex_0x3E0_2BE4_12222 = world_D41A0.playerInputs_0x6E3E[i].param1_0x6E3E_byte1;
		case 0x13:
			strcpy(printbuffer, world_D41A0.players_0x2BDE[i].names_81[world_D41A0.players_0x2BDE[i].chatLineIndex_0x3E0_2BE4_12222]);
			useSound = false;
			if (printbuffer[0])
			{
				useSound = true;
				if (x_toupper(printbuffer[0]) == 'W'
					&& x_toupper(printbuffer[1]) == 'I'
					&& x_toupper(printbuffer[2]) == 'N'
					&& x_toupper(printbuffer[3]) == 'D'
					&& x_toupper(printbuffer[4]) == 'Y')
				{
					// WINDY is a single-player cheat and is ignored in a network game.
					//
					// Setting the top bit turns setting_byte2_23 (an int8_t) negative, and every
					// "setting_byte2_23 < 0" test in PlayerInput.cpp is that byte being read as
					// "cheats are on" - it hands out the debug keys, and it also lifts the castle
					// experience gate in EventsFunctions.  None of that is exchanged with anyone:
					// the other players neither agree to it nor ever find out, so one node would
					// be playing by rules of its own while the rest keep to the game's.  There is
					// already one key excluded from multiplayer this way (PlayerInput.cpp, the
					// "setting_byte2_23 < 0 && !MULTIPLAYER_MODE" test); this is the same reason
					// applied at the source.
					//
					// The word is still recognised and swallowed rather than passed on, so typing
					// it in a network game does nothing at all - it is not broadcast as an
					// ordinary chat line either.
					if (i == world_D41A0.LevelIndex_0xc
						&& !(gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE))
						gameState_D41A4.setting_byte2_23 |= 0x80u;
				}
				else
				{
					// loc_522C2: your own message is always shown to you; whether someone
					// else's reaches you is decided by the mode they sent it in
					// (byte_0x3E1: 0 = in view, 1 = nearest, 2 = everyone, 3 = allies).
					// This test used to be the wrong way round - the switch ran for the
					// local player and remote messages were dropped without being read,
					// so chat never arrived from anyone else in a network game.
					bool bool1 = false;
					if (i == world_D41A0.LevelIndex_0xc)
					{
						bool1 = true;
					}
					else
					{
						switch (world_D41A0.players_0x2BDE[i].chatState_0x3E1_2BE4_12223)
						{
						case 0:
							if (!(IsBalloonNearCastle_61810(actEvent, Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]) == 0))
								bool1 = true;
							break;
						case 1:
							if (!(FindNearestManaForBalloon_61620(actEvent, Entities_EA3E4[world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerIndex_0x00a_2BE4_11240]) == 0))
								bool1 = true;
							break;
						case 2:
							bool1 = true;
							break;
						case 3:
							if ((1 << world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].playerData_0x3E6_2BE4_12228.playerColorIndex_0x38_56) & world_D41A0.players_0x2BDE[i].chatRecipients_0x3E3_2BE4_12225)
								bool1 = true;
							break;
						default:
							break;
						}
					}
					if (bool1)
					{
						strcpy(world_D41A0.players_0x2BDE[i].CurrentNotificationText_0x01c_2BFA_11258, printbuffer);
						world_D41A0.players_0x2BDE[i].CurrentNotificationDuration_0x04d_2C2B_11307 = 200;
						world_D41A0.players_0x2BDE[i].endState_0x04f_2C2D_11309 = 2;
					}
				}
			}
			SetMenuCursorPosition_52E90(&world_D41A0.players_0x2BDE[i], 0, useSound);
			break;
		case 0x2C:
			world_D41A0.players_0x2BDE[i].chatLength_0x3E2_2BE4_12224 = 0;
			world_D41A0.players_0x2BDE[i].names_81[world_D41A0.players_0x2BDE[i].chatLineIndex_0x3E0_2BE4_12222][0] = 0;
			PrepareEventSound_6E450(0, world_D41A0.players_0x2BDE[i].playerSlot_0x007_2BE4_11237, 14);
			break;
		default:
			break;
		}
		
		//Player movement/actions
		int rollEnv = 2 * world_D41A0.playerInputs_0x6E3E[i].roll - actEvent->playerData_0xA4_164x->roll_0x155_341;
		actEvent->playerData_0xA4_164x->rollDelta_0x4_4 = (rollEnv - (my_sign32(rollEnv) << 2) + my_sign32(rollEnv)) >> 2;
		int pitchEnv = 2 * world_D41A0.playerInputs_0x6E3E[i].pitch - actEvent->playerData_0xA4_164x->pitch_0x157_343;
		actEvent->playerData_0xA4_164x->pitchDelta_0x6_6 = (pitchEnv - (my_sign32(pitchEnv) << 2) + my_sign32(pitchEnv)) >> 2;
		actEvent->playerData_0xA4_164x->entityIndex_0x0 = (uint8_t)world_D41A0.playerInputs_0x6E3E[i].entityIndex_0x6E3E_byte5;//233CD1 xor eax,eax; mov al,[edx+5]
		actEvent->playerData_0xA4_164x->nextEntity_0x18_24 = world_D41A0.playerInputs_0x6E3E[i].nextEntity_0x6E3E_word6;
		actEvent->playerData_0xA4_164x->entityIndex2_0x1A_26 = world_D41A0.playerInputs_0x6E3E[i].entityIndex2_0x6E3E_word8;

		if ((CommandLineParams.DoKillMoveAndRotation()&&(world_D41A0.LevelIndex_0xc==i))&&
			(CommandLineParams.GetPlaybackPath().length() <= 0))
		{
			actEvent->playerData_0xA4_164x->entityIndex_0x0 = 0;
			actEvent->playerData_0xA4_164x->speed_0xc_12 = 0;
			actEvent->playerData_0xA4_164x->pitchDelta_0x6_6 = 0;
			actEvent->playerData_0xA4_164x->rollDelta_0x4_4 = 0;
			actEvent->playerData_0xA4_164x->fov_0x22_34 = 0;
			//actEvent->playerData_0xA4_164x->yaw_0x1E_30 = 0;
			actEvent->playerData_0xA4_164x->roll_0x155_341 = 0;
		}

		CopyPlayerPositionAndRotation_57B20(&world_D41A0.players_0x2BDE[i], Entities_EA3E4[world_D41A0.players_0x2BDE[i].playerIndex_0x00a_2BE4_11240]);
		if (world_D41A0.players_0x2BDE[i].rollState_0x846_2BDE)
			ApplyPlayerOptions_55C60(&world_D41A0.players_0x2BDE[i]);
		memset(&world_D41A0.playerInputs_0x6E3E[i], 0, 10);
	}
}

//----- (00055C60) --------------------------------------------------------
void ApplyPlayerOptions_55C60(type_str_0x2BDE* loc0x2BDE)//236c60
{
	int param1 = 0;
	int param2 = 0;
	int param3 = 0;

	if ((gameState_D41A4.OptionsSettingFlag_24 & 1) == 0)
	{
		int number1 = abs(loc0x2BDE->playerData_0x3E6_2BE4_12228.roll_0x155_341);
		switch (loc0x2BDE->rollState_0x846_2BDE)
		{
		case 1:
		case 2:
			param1 = 1;
			param2 = -2;
			param3 = 1024;
			break;
		case 3:
			param1 = 1;
			param3 = number1 + 2048;
			param2 = 1;
			break;
		case 4:
			param1 = 1;
			param2 = -2;
			param3 = number1 + 2116;
			break;
		case 5:
			param1 = -1;
			param2 = 3;
			param3 = number1 + 2048;
			break;
		case 6:
			param1 = -1;
			param2 = -4;
			param3 = number1 + 2013;
			break;
		case 7:
			param1 = 1;
			param2 = 5;
			param3 = number1 + 2048;
			break;
		default:
			break;
		}
		if (loc0x2BDE->rollState_0x846_2BDE == 1)
		{
			if (loc0x2BDE->playerData_0x3E6_2BE4_12228.roll_0x155_341 >= 0)
				loc0x2BDE->rollDirection_0x847_2BDE = 1;
			else
				loc0x2BDE->rollDirection_0x847_2BDE = -1;
			loc0x2BDE->rollSpeed_0x84A_2BDE = 91;
			loc0x2BDE->rollAngle_0x848_2BDE = abs(loc0x2BDE->playerData_0x3E6_2BE4_12228.roll_0x155_341);
			loc0x2BDE->rollState_0x846_2BDE++;
			gameState_D41A4.lastMouseX_220 = mouse_E3760.x;
			InitPlayerSlots_55EB0(loc0x2BDE->playerIndex_0x00a_2BE4_11240);
		}
		loc0x2BDE->rollAngle_0x848_2BDE += param1 * loc0x2BDE->rollSpeed_0x84A_2BDE;
		if (param2)
		{
			loc0x2BDE->rollSpeed_0x84A_2BDE += param2;
			if (loc0x2BDE->rollSpeed_0x84A_2BDE < 11)
				loc0x2BDE->rollSpeed_0x84A_2BDE = 11;
			if (loc0x2BDE->rollSpeed_0x84A_2BDE > 68)
				loc0x2BDE->rollSpeed_0x84A_2BDE = 68;
		}
		if (param1 > 0 && loc0x2BDE->rollAngle_0x848_2BDE >= param3 || param1 < 0 && loc0x2BDE->rollAngle_0x848_2BDE <= param3)
		{
			loc0x2BDE->rollState_0x846_2BDE++;
			if (loc0x2BDE->rollState_0x846_2BDE > 3u)
				loc0x2BDE->rollSpeed_0x84A_2BDE >>= 1;
		}
		if (loc0x2BDE->rollState_0x846_2BDE >= 4u)
		{
			if (abs(mouse_E3760.x - gameState_D41A4.lastMouseX_220) > 16)
				loc0x2BDE->rollState_0x846_2BDE = 8;
			gameState_D41A4.lastMouseX_220 = mouse_E3760.x;
		}
		loc0x2BDE->playerData_0x3E6_2BE4_12228.rollDelta_0x4_4 = 0;
		if (loc0x2BDE->rollState_0x846_2BDE == 8)
		{
			InitPlayerSlots_55EB0(loc0x2BDE->playerIndex_0x00a_2BE4_11240);
			loc0x2BDE->rollState_0x846_2BDE = 0;
		}
		else
		{
			param3 = loc0x2BDE->rollAngle_0x848_2BDE * loc0x2BDE->rollDirection_0x847_2BDE;
			param3 &= 0x07ffu;
			loc0x2BDE->positions_0x1d1_2BDE_11695[loc0x2BDE->positionCount_0x010_2BDE_11246].rotation__2BDE_11701.roll = param3;
		}
	}
}

int debugcounter_238b2f = 0;
//----- (00057B20) --------------------------------------------------------
void CopyPlayerPositionAndRotation_57B20(type_str_0x2BDE* a1x, type_entity_0x6E8E* a2x)//238b20 //copy position and rotation
{
	//__int16 v2; // t1
	//int v3; // edi
	int v4; // ecx

	if (CommandLineParams.DoRotatePlayer()) {
		a2x->yaw_0x1C_28 = 0x900;
	}

	if (CommandLineParams.DoMovePlayer()) {
		if (debugcounter_238b2f == 10)
		{
			a2x->position_0x4C_76.x = 0x8c07;
			a2x->position_0x4C_76.y = 0xb427;
		}
		/*if (debugcounter_238b2f == 10) {
			a2x->array_0x4C_76.x = 0x7249;
			a2x->array_0x4C_76.y = 0xd491;
		}else
		if (debugcounter_238b2f == 351) {
			//a2x->array_0x4C_76.x = 0xbedc+656;
			//a2x->array_0x4C_76.y = 0xd541+1200;
			//a2x->word_0x82_130 = 0x50;
			//a2x->array_0x52_82.aa = 64;
		}
		else*/
		/*	if (debugcounter_238b2f == 10) {
				//a2x->array_0x4C_76.x = 0xbedc + 356;
				//a2x->array_0x4C_76.y = 0xd541 - 1200;
				a2x->word_0x82_130 = 0x300;
				//a2x->array_0x52_82.aa = 64;
			}
			if (debugcounter_238b2f == 200) {
				//a2x->array_0x4C_76.x = 0xbedc + 356;
				//a2x->array_0x4C_76.y = 0xd541 - 1200;
				a2x->word_0x1C_28 = -0x10;
				a2x->word_0x82_130 = 0x200;
				//a2x->array_0x52_82.aa = 64;
			}
			if(debugcounter_238b2f>250)
				if (debugcounter_238b2f % 50 == 0)
				{
					a2x->word_0x1C_28 -= 0x10;
					a2x->word_0x82_130 = 0x200;
				}
			//a2x->array_0x4C_76.x = 0x2876;
			//a2x->array_0x4C_76.y = 0x2c75;*/
			/*if (debugcounter_238b2f < 10) {
				a2x->array_0x4C_76.x = 0x73a0;
				a2x->array_0x4C_76.y = 0xd5e6;
			}
			else {
				a2x->array_0x4C_76.x = 0xc1aa;
				a2x->array_0x4C_76.y = 0xd542;
			}*/
		debugcounter_238b2f++;
	}

	if (!(gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED))
	{
		a1x->positions_0x1d1_2BDE_11695[a1x->positionCount_0x010_2BDE_11246].axis_2BDE_11695.x = a2x->position_0x4C_76.x;
		a1x->positions_0x1d1_2BDE_11695[a1x->positionCount_0x010_2BDE_11246].axis_2BDE_11695.y = a2x->position_0x4C_76.y;
		GetTerrainAlt_10C40(&a2x->position_0x4C_76);
		//v2 = a2x->array_0x4C_76.z;
		a1x->positions_0x1d1_2BDE_11695[a1x->positionCount_0x010_2BDE_11246].axis_2BDE_11695.z = a2x->position_0x4C_76.z;

		a1x->positions_0x1d1_2BDE_11695[a1x->positionCount_0x010_2BDE_11246].rotation__2BDE_11701.yaw = a2x->yaw_0x1C_28;
		a1x->positions_0x1d1_2BDE_11695[a1x->positionCount_0x010_2BDE_11246].rotation__2BDE_11701.fov = a1x->positions_0x1d1_2BDE_11695[0].rotation__2BDE_11701.fov;
		a1x->positions_0x1d1_2BDE_11695[a1x->positionCount_0x010_2BDE_11246].rotation__2BDE_11701.roll = a2x->playerData_0xA4_164x->roll_0x155_341;
		//if(mouseturnoff)*(x_WORD *)(14 * (*(unsigned __int16 *)(a1 + 16) - 1) + a1 + 489) = 6;//testdebug
		//v3 = a2x->dword_0xA4_164;
		v4 = a2x->playerData_0xA4_164x->moveBoost_0x1E_30;
		/*a1x->struct_0x1d1_2BDE_11695[a1x->word_0x010_2BDE_11246].rotation__2BDE_11701.bb = a2x->playerData_0xA4_164x->word_0x157_343 / 2
			+ gameState_D41A4.byteindex_121[1]
			* (unsigned __int16)((signed int)(v4 - (__CFSHL__((unsigned __int64)a2x->playerData_0xA4_164x->word_0x1E_30 >> 32, 4) + 16 * ((unsigned __int64)a2x->playerData_0xA4_164x->word_0x1E_30 >> 32))) >> 4)
			- ((v4 - (__CFSHL__(v4 >> 31, 3) + 8 * (v4 >> 31))) >> 3);
			*/
		a1x->positions_0x1d1_2BDE_11695[a1x->positionCount_0x010_2BDE_11246].rotation__2BDE_11701.pitch = a2x->playerData_0xA4_164x->pitch_0x157_343 / 2
			+ gameState_D41A4.colorIndex_121[1]
			* ((v4 - (my_sign32(v4) << 4) + my_sign32(v4)) >> 4)
			- ((v4 - (my_sign32(v4) << 3) + my_sign32(v4)) >> 3);

		//my_sign32(a2x->playerData_0xA4_164x->word_0x1E_30)
		a1x->positions_0x1d1_2BDE_11695[a1x->positionCount_0x010_2BDE_11246].rotation__2BDE_11701.yaw += a2x->playerData_0xA4_164x->nextEntity_0x18_24;
		a1x->positions_0x1d1_2BDE_11695[a1x->positionCount_0x010_2BDE_11246].rotation__2BDE_11701.pitch += a2x->playerData_0xA4_164x->entityIndex2_0x1A_26;
	}
	//return 1;
}
// D41A4: using guessed type int x_DWORD_D41A4;

//----- (00058630) --------------------------------------------------------
void UpdateMultiplayerRandom_58630()//239630
{
	char v0; // bl
	unsigned int v1; // edx
	//int v2; // eax
	//unsigned int v3; // eax
	uint8_t v3x;
	type_entity_0x6E8E* v4x; // eax
	uaxis_2d v5x; // cx
	//int v6; // ebx
	//unsigned int v7; // eax
	//unsigned int v8; // edx
	//int v9; // eax
	signed int v10; // esi
	signed int v11; // edi
	char v12; // ah
	uaxis_2d v13x; // bx
	axis_3d v15x; // [esp+0h] [ebp-10h]
	//__int16 v16; // [esp+4h] [ebp-Ch]
	int v17; // [esp+8h] [ebp-8h]
	uaxis_2d v18x; // [esp+Ch] [ebp-4h]

	v0 = 0;
	v1 = 0;
	if (gameState_D41A4.setting_byte1_22 & Setting::MULTIPLAYER_MODE)
	{
		//v2 = (int)x_D41A0_BYTEARRAY_0;
		//*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8) = 9377 * *(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8) + 9439;
		world_D41A0.rand_0x8 = 9377 * world_D41A0.rand_0x8 + 9439;
		v1 = world_D41A0.rand_0x8 % world_D41A0.NumberOfPlayers_0xe;
	}
	else if (world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].Turn_2BE0_11248 & 7)
		//D41A0_BYTESTR_0.array_0x2BDE[D41A0_BYTESTR_0.word_0xc]+18
	{
		goto LABEL_5;
	}
	v0 = 1;
LABEL_5:
	//v3 = 2124 * v1 + (int)x_D41A0_BYTEARRAY_0 + 11230;
	if (!world_D41A0.players_0x2BDE[v1].playerActive_0x006_2BE4_11236)
		v0 = 0;
	if (v0)
	{
		v4x = Entities_EA3E4[world_D41A0.players_0x2BDE[v1].playerIndex_0x00a_2BE4_11240];
		v15x = v4x->position_0x4C_76;
		MoveEntity_57FA0(&v15x, v4x->yaw_0x1C_28, 0, 2560);
		//LOBYTE(v5) = (unsigned __int16)(v15x.x + 128) >> 8;
		//v6 = (int)x_D41A0_BYTEARRAY_0;
		//HIBYTE(v5) = (unsigned __int16)(v15x.y + 128) >> 8;
		v5x._axis_2d.x = (unsigned __int16)(v15x.x + 128) >> 8;
		v5x._axis_2d.y = (unsigned __int16)(v15x.y + 128) >> 8;
		//v7 = 9377 * *(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8) + 9439;
		//*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8) = v7;
		world_D41A0.rand_0x8 = 9377 * world_D41A0.rand_0x8 + 9439;
		//v8 = D41A0_BYTESTR_0.dword_0x8 % 0x14;
		//v9 = 9377 * *(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8);
		v17 = world_D41A0.rand_0x8 % 0x14;
		//v3 = v9 + 9439;
		world_D41A0.rand_0x8 = 9377 * world_D41A0.rand_0x8 + 9439;
		//*(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 8) = v3;
		//LOBYTE(v5) = v5 - 10;
		//HIBYTE(v5) -= 10;
		v5x._axis_2d.x -= 10;
		v5x._axis_2d.y -= 10;
		//LOWORD(v18) = v5;
		v18x.word = v5x.word;
		v10 = world_D41A0.rand_0x8 % 0x14;
		v18x._axis_2d.y = world_D41A0.rand_0x8 % 0x14 + v5x._axis_2d.y;
		v3x = 0;
		while (v10 < 20 && !v3x)
		{
			v11 = v17;
			v13x._axis_2d.y = v18x._axis_2d.y;
			v12 = v17;
			v17 = 0;
			v13x._axis_2d.x = v12 + v18x._axis_2d.x;
			while (v11 < 20 && !v3x)
			{
				if (!mapTerrainType_10B4E0[v13x.word] && !(mapAngle_13B4E0[v13x.word] & 8))
				{
					v15x.x = v13x._axis_2d.x << 8;
					v15x.y = v13x._axis_2d.y << 8;
					CreateEntity_4A190(&v15x, 10, 86);
					v3x = 1;
				}
				v11 += 11;
				v13x._axis_2d.x += 11;
			}
			v10++;
			v18x._axis_2d.y++;
		}
	}
	//return v3;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

int debugcounter_23d950 = 0;
//----- (0005C950) --------------------------------------------------------
void UpdatePlayerWizard_5C950(type_str_0x2BDE* a1x, type_entity_0x6E8E* a2x)//23d950 //tex 0,211,243,203,251,235,227,219
{
	type_entity_0x6E8E* v2x; // ebx
	__int16 v3; // ax
	//int v4; // eax
	//unsigned __int16 v5; // dx
	axis_3d* v6x; // esi
	//int v7; // ecx
	char v8; // ST08_1
	int v9; // edi
	//int v10; // edx
	signed int i; // eax
	//uint8_t* v12; // eax
	//int v13; // edx
	__int16 v14; // cx
	//int v15; // eax
	type_entity_0x6E8E* v16x; // eax
	//uint8_t* v17; // ecx
	axis_3d* v18x; // edi
	//uint8_t* v19; // esi
	type_entity_0x6E8E* v20x; // eax
	__int16 v21; // dx
	int v22; // edi
	int v23; // eax
	type_entity_0x6E8E* v24x; // edx
	type_entity_0x6E8E* v25x; // eax
	type_entity_0x6E8E* v26x; // ST08_4
	type_entity_0x6E8E* v27x; // esi
	int v28; // eax
	int v29; // eax
	//int v30; // edx
	type_entity_0x6E8E* kx; // eax
	char v32; // dl
	signed int l; // eax
	//int result; // eax
	axis_3d v35x; // [esp+0h] [ebp-14h]
	//__int16 v36; // [esp+4h] [ebp-10h]
	int v37; // [esp+8h] [ebp-Ch]
	int j; // [esp+Ch] [ebp-8h]
	type_entity_0x6E8E* v39x; // [esp+10h] [ebp-4h]

	a2x->param_0x10_16 = 0;//fix 23D95C: the death countdown (1200) is not left to the castle code
	if (CommandLineParams.DoDebugSequences2()) {
		//add_compare(0x23d954, CommandLineParams.DoDebugafterload());
	}

	//fix it
	//v10 = 0;
	//fix it

	v2x = a2x;
	v37 = 0;
	RebuildEntityLists_49F90();
	//*(uint32_t*)v35x = *(x_DWORD *)(x_D41A0_BYTEARRAY_0 + 6 * ((a1 - &x_D41A0_BYTEARRAY_0[11230]) / 2124) + 9058);//fix
	//v35x[2] = *(x_WORD *)(x_D41A0_BYTEARRAY_0 + 6 * ((a1 - &x_D41A0_BYTEARRAY_0[11230]) / 2124) + 9062);
	v35x = world_D41A0.playerStarts_0x2362[a1x - &world_D41A0.players_0x2BDE[0]]; // FIXME: pointer arithmetic for array_0x2362
	//v35x[2] = get_x_D41A0_BYTEARRAY_0_0x2366(((a1 - &x_D41A0_BYTEARRAY_0[11230]) / 2124));
	v3 = GetTerrainAlt_10C40(&v35x);
	v3 += 0x100;
	v35x.z = v3;
	if (a2x == Entities_EA3E4[0])
	{
		v2x = CreateEntity_4A190(&v35x, 3, a1x->IsAiPlayer_0x009_2BE4_11239 == 1);
		v37 = 1;
	}
	else
	{
		a2x->actionIndex_0x45_69 = a1x->IsAiPlayer_0x009_2BE4_11239 == 1;
		//v4 = a2x->dword_0xA4_164;
		a2x->flags_0xc_12_15.byte[0] &= 0xDFu;
		//v5 = a2x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58;
		if (a2x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58)
		{
			v6x = &Entities_EA3E4[a2x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]->position_0x4C_76;
			v35x = *v6x;
		}
		CopyEntityPosition_57CF0(a2x, &v35x);
	}
	//v7 = (int)x_D41A0_BYTEARRAY_0;
	a1x->playerIndex_0x00a_2BE4_11240 = v2x - world_D41A0.entities_0x6E8E;
	v2x->playerData_0xA4_164x = &a1x->playerData_0x3E6_2BE4_12228;
	v2x->playerData_0xA4_164x->playerColorIndex_0x38_56 = a1x - &world_D41A0.players_0x2BDE[0]; // FIXME: pointer arithmetic
	v2x->playerData_0xA4_164x->invulnerableTimer_0x159_345 = 100;
	v2x->playerData_0xA4_164x->word_0x24C_588 = 0;
	v2x->playerData_0xA4_164x->startTimer_0x16D_365 = 2000;
	v2x->playerData_0xA4_164x->speed_0xc_12 = 0;
	v2x->playerData_0xA4_164x->yaw_0x1E_30 = 0;
	v2x->playerData_0xA4_164x->fov_0x22_34 = 0;
	v2x->playerData_0xA4_164x->moveBoost_0x1E_30 = 0;
	v2x->playerData_0xA4_164x->strafeSpeed_0x10_16 = 0;
	memset(&v2x->playerData_0xA4_164x->objectiveTargets_0x1AC_428, 0, 18);
	v2x->playerData_0xA4_164x->lifeScale_0x24A_586 = 256;
	v8 = v37;
	v2x->maxMana_0x8C_140 = 1000;
	v9 = v37;
	v2x->maxLife_0x4 = 10000;
	UpdatePlayerSpellCharges_5CF40(v2x, v8);
	if (v9)
	{
		LinkObjectiveToEntity_58DA0(0, v2x);
		for (i = 0; i < 26; i++)
			v2x->playerData_0xA4_164x->str_611.spellsExperience_0x2CB_715x.at(i) = 0;
		v2x->playerData_0xA4_164x->time_393 = j___clock();
		switch (TransformPlayerColorIndex_616D0(v2x->playerData_0xA4_164x->playerColorIndex_0x38_56))
		{
		case 0:
			SetEntityIndexAndRot_49CD0(v2x, 44);
			break;
		case 1:
			SetEntityIndexAndRot_49CD0(v2x, 273);
			break;
		case 2:
			SetEntityIndexAndRot_49CD0(v2x, 274);
			break;
		case 3:
			SetEntityIndexAndRot_49CD0(v2x, 275);
			break;
		case 4:
			SetEntityIndexAndRot_49CD0(v2x, 276);
			break;
		case 5:
			SetEntityIndexAndRot_49CD0(v2x, 277);
			break;
		case 6:
			SetEntityIndexAndRot_49CD0(v2x, 278);
			break;
		case 7:
			SetEntityIndexAndRot_49CD0(v2x, 279);
			break;
		default:
			break;
		}
		if (a1x->IsAiPlayer_0x009_2BE4_11239 == 1)
		{
			//v12 = x_D41A0_BYTEARRAY_0;
			v2x->playerData_0xA4_164x->aiAggression_0x242_578 = world_D41A0.terrain_2FECE.WizardMapSettings_0x360D2[v2x->playerData_0xA4_164x->playerColorIndex_0x38_56].Aggression_0x360D5;
			v2x->playerData_0xA4_164x->aiPerception_0x244_580 = world_D41A0.terrain_2FECE.WizardMapSettings_0x360D2[v2x->playerData_0xA4_164x->playerColorIndex_0x38_56].Perception_0x360DD;
			v2x->playerData_0xA4_164x->aiReflexes_0x246_582 = world_D41A0.terrain_2FECE.WizardMapSettings_0x360D2[v2x->playerData_0xA4_164x->playerColorIndex_0x38_56].Reflexes_0x360D9;
			//v13 = v2x->dword_0xA4_164;
			v14 = world_D41A0.terrain_2FECE.WizardMapSettings_0x360D2[v2x->playerData_0xA4_164x->playerColorIndex_0x38_56].Life_0x3612F;
			if (v14)
			{
				v2x->playerData_0xA4_164x->lifeScale_0x24A_586 = v14;
				v2x->maxLife_0x4 = v2x->maxLife_0x4 * v2x->playerData_0xA4_164x->lifeScale_0x24A_586 >> 8;
			}
			//v15 = v2x->dword_0xA4_164;
			if (v2x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[2])
			{
				if (world_D41A0.terrain_2FECE.player_0x2FED9[v2x->playerData_0xA4_164x->playerColorIndex_0x38_56])
				{
					v16x = CreateEntity_4A190(&v2x->position_0x4C_76, 3, 2);
					v39x = v16x;
					if (v16x)
					{
						//v17 = &x_D41A0_BYTEARRAY_0[28302];
						v16x->id_0x1A_26 = v2x->id_0x1A_26;
						v2x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58 = v16x - world_D41A0.entities_0x6E8E;
						PrepareEventSound_6E450(v2x - world_D41A0.entities_0x6E8E, -1, 30);
						for (j = 0; ; j = v22 + 1)
						{
							v23 = world_D41A0.terrain_2FECE.player_0x2FED9[v2x->playerData_0xA4_164x->playerColorIndex_0x38_56];
							if (v23 <= j)
								break;
							v18x = &Entities_EA3E4[0]->position_0x4C_76;
							//v19 = v39x->axis_0x9A_154x;
							*v18x = v39x->axis_0x9A_154x;
							v20x = Entities_EA3E4[0];
							v21 = v39x->id_0x1A_26;
							Entities_EA3E4[0]->model_0x40_64 = 0;
							v22 = j;
							v20x->param_0x10_16 = 0;
							v20x->id_0x1A_26 = v21;
							v20x->levelPar3_0x46_70 = j;
							UpdateManaSphereMovement_36FC0(v20x);
						}
						v24x = v39x;
						v39x->param_0x10_16 = v23 - 1;
						SetShiftByCastle_49EC0(v24x, v24x->param_0x10_16);
						v25x = v39x;
						v26x = v39x;
						v39x->params_0x52_82.yaw = 0xe000;
						v27x = v25x;
						v25x->params_0x52_82.fov = 0x4000;
						GrowCastle_60810(v26x);
						v28 = v27x->maxMana_0x8C_140;
						v27x->mana_0x90_144 = v28;
						if (v28 < 0)
							v27x->mana_0x90_144 = 0;
						if (v39x->mana_0x90_144 > 320000)
							v39x->mana_0x90_144 = 320000;
					}
				}
			}
		}
		v2x->playerData_0xA4_164x->creaturesKilledPercent_373 = 0;
	}
	if (world_D41A0.LevelIndex_0xc == v2x->playerData_0xA4_164x->playerColorIndex_0x38_56)
		v2x->flags_0xc_12_15.byte[0] |= 1u;
	//23ddf7
	v2x->life_0x8 = v2x->maxLife_0x4;
	v29 = v2x->maxMana_0x8C_140;
	//v30 = v2x->playerData_0xA4_164x;
	v2x->mana_0x90_144 = v29;
	v2x->playerData_0xA4_164x->maxMana_0x150_336 = v29;
	for (kx = gameState_D41A4.class3List_38519; kx > Entities_EA3E4[0]; kx = kx->next_0)
	{
		if (kx->id_0x1A_26 != v2x->id_0x1A_26)
		{
			v32 = kx->model_0x40_64;
			if (!v32 || v32 == 1)
				kx->playerData_0xA4_164x->aiTargets_0x1FC_508[4 + 4 * v2x->playerData_0xA4_164x->playerColorIndex_0x38_56] = -24609;
		}
	}
	v2x->playerData_0xA4_164x->mountId_0x146_326 = 0;
	v2x->playerData_0xA4_164x->word_0x148_328 = 0;
	if (v2x->model_0x40_64 == 1)
	{
		memset(&v2x->playerData_0xA4_164x->aiState_0x1C1_449, 0, 1);
		//for (l = 0; l < 8; *(x_WORD *)(v2x->dword_0xA4_164 + 8 * l + 508) = 24607)
		for (l = 0; l < 8; l++)
			//*(x_WORD*)(v2x->dword_0xA4_164 + 8 * l + 508) = 24607
			v2x->playerData_0xA4_164x->aiTargets_0x1FC_508[4 * l + 4] = 24607;
		v2x->playerData_0xA4_164x->str_611.spellCooldowns_0x367_871x.SpellEnabled[2] = 4 * v2x->playerData_0xA4_164x->playerColorIndex_0x38_56;
	}
	v2x->playerData_0xA4_164x->screenShake_0x19A_410 = 2048;
	v2x->playerData_0xA4_164x->maxDistance_0x19E_414 = 2048;
	v2x->playerData_0xA4_164x->hitTimer_0x36_54 = 0;
	memset(v2x->playerData_0xA4_164x->array_0x15B_347, 16, 8);
	//result = (int)x_D41A0_BYTEARRAY_0;
	world_D41A0.freeEntityCount2_0x11e6 = -1;
	if (v2x->playerData_0xA4_164x->playerColorIndex_0x38_56 == world_D41A0.LevelIndex_0xc)
	{
		//result = (int)x_D41A0_BYTEARRAY_4;
		gameState_D41A4.speedIndex = 0;
	}
	//return result;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// 98786: using guessed type int /*__fastcall*/ j___clock(x_DWORD, x_DWORD, x_DWORD);
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0005CF40) --------------------------------------------------------
void UpdatePlayerSpellCharges_5CF40(type_entity_0x6E8E* a1x, char a2)//23df40
{
	int i; // esi
	//int v3; // edi
	type_entity_0x6E8E* v4x; // eax
	//uint8_t* v5; // ST0C_4
	signed int j; // edx
	signed int v7; // esi
	//int v8; // eax
	//__int16 v9; // cx

	for (i = 0; i < 26; i++)
	{
		//v3 = 2 * i;
		/*
		esi=1
		edi=2*esi=2*1
		eax=[ebx+a4]
		*/
		if (a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[i])
		{
			v4x = CreateEntity_4A190(&a1x->position_0x4C_76, 15, i);
			if (v4x)
			{
				a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[i] = v4x - world_D41A0.entities_0x6E8E;
				v4x->parentId_0x28_40 = a1x - world_D41A0.entities_0x6E8E;
				v4x->flags_0xc_12_15.byte[0] |= 1u;
			}
			else
			{
				a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[i] = 0;
			}
		}
	}
	if (a2)
	{
		for (j = 0; j < 26; j++)
			a1x->playerData_0xA4_164x->str_611.selectedSubSpells_0x437_1079x.SpellIndex[j] = 0;
	}
	GrantSpellToPlayer_6DB50(0, 0);
	for (v7 = 0; v7 < 26; v7++)
	{
		//v8 = a1x->dword_0xA4_164;
		//v9 = *(x_WORD *)(v8 + 2 * v7 + 819);
		if (a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[v7])
			SetSpell_6D5E0(Entities_EA3E4[a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[v7]], a1x->playerData_0xA4_164x->str_611.selectedSubSpells_0x437_1079x.SpellIndex[v7]);
	}
	a1x->playerData_0xA4_164x->str_611.SubSpellIndexLeft_1109 = a1x->playerData_0xA4_164x->str_611.selectedSubSpells_0x437_1079x.SpellIndex[a1x->playerData_0xA4_164x->str_611.SpellIndexLeft_0x451_1105];
	a1x->playerData_0xA4_164x->str_611.SubSpellIndexRight_1110 = a1x->playerData_0xA4_164x->str_611.selectedSubSpells_0x437_1079x.SpellIndex[a1x->playerData_0xA4_164x->str_611.SpellIndexRight_0x453_1107];
	ResetLevelSpells_574A0();
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0005D0A0) --------------------------------------------------------
bool MoveTest_5D0A0(type_entity_0x6E8E* a1x)//23e0a0
{
	//unsigned __int16 v1; // di
	//int v2; // eax
	int v3; // eax
	unsigned __int16 v4; // si
	unsigned __int16 v5; // ax
	unsigned __int16 v6; // ax
	//int v7; // esi
	int v8; // esi
	__int16 v9; // ax
	int v10; // eax
	int v11; // edx
	int v12; // edx
	__int16 v13; // si
	__int16 v14; // ax
	int v15; // edi
	__int16 v16; // ax
	int v17; // esi
	int v18; // eax
	axis_3d* v19x; // esi
	//char v20; // ch
	//int v21; // edx
	//int v22; // eax
	int v23; // eax
	//__int16 v24; // ax
	//__int16 v25; // dx
	axis_3d v27x; // [esp+0h] [ebp-60h]
	axis_3d v29x; // [esp+8h] [ebp-58h]
	//axis_3d v31x; // [esp+10h] [ebp-50h]
	axis_3d v33x; // [esp+18h] [ebp-48h]
	int v35; // [esp+20h] [ebp-40h]
	int v36; // [esp+24h] [ebp-3Ch]
	int v37; // [esp+28h] [ebp-38h]
	int v38; // [esp+2Ch] [ebp-34h]
	int v39; // [esp+30h] [ebp-30h]
	int i; // [esp+34h] [ebp-2Ch]
	int v41; // [esp+38h] [ebp-28h]
	unsigned int v42; // [esp+3Ch] [ebp-24h]
	int v43; // [esp+40h] [ebp-20h]
	int v44; // [esp+44h] [ebp-1Ch]
	int v45; // [esp+48h] [ebp-18h]
	//int result; // [esp+4Ch] [ebp-14h]
	unsigned __int16 v47; // [esp+50h] [ebp-10h]
	int v48; // [esp+54h] [ebp-Ch]
	int v49; // [esp+58h] [ebp-8h]
	char v50; // [esp+5Ch] [ebp-4h]

	bool result = true;
	if (TerrainTileIsWater_104D0(&predictedAxis_EB398ar) == 256)
	{
		a1x->playerData_0xA4_164x->waterCounter_0x262_610++;
		//v1 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &x_WORD_EB398ar);
		//LOWORD(v2) = Maths::GetPitchAngle_58210(&a1x->position_0x4C_76, &x_WORD_EB398ar);
		v45 = Maths::GetPitchAngle_58210(&a1x->position_0x4C_76, &predictedAxis_EB398ar);
		v42 = Distance3D_583F0(&a1x->position_0x4C_76, &predictedAxis_EB398ar);
		v41 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &predictedAxis_EB398ar);
		v3 = (v41 - (__CFSHL__(v41 >> 31, 9) + (v41 >> 31 << 9))) >> 9;
		predictedAxis_EB398ar = a1x->position_0x4C_76;
		v4 = (x_WORD)v3 << 9;
		v49 = (((x_WORD)v3 + 1) << 9) & 0x7FF;
		v5 = AngleDifference_582B0(v41, (x_WORD)v3 << 9);
		v36 = (unsigned __int16)v42;
		v35 = (unsigned __int16)v45;
		MoveEntity_57FA0(
			&predictedAxis_EB398ar,
			v4,
			v45,
			((unsigned __int16)v42 * (512 - v5)
				- (__CFSHL__((unsigned __int16)v42 * (512 - v5) >> 31, 9)
					+ ((unsigned __int16)v42 * (512 - v5) >> 31 << 9))) >> 9);
		if (TerrainTileIsWater_104D0(&predictedAxis_EB398ar) == 256)
		{
			predictedAxis_EB398ar = a1x->position_0x4C_76;
			v6 = AngleDifference_582B0(v41, v49);
			MoveEntity_57FA0(
				&predictedAxis_EB398ar,
				v49,
				v35,
				(v36 * (512 - v6) - (__CFSHL__(v36 * (512 - v6) >> 31, 9) + (v36 * (512 - v6) >> 31 << 9))) >> 9);
			if (TerrainTileIsWater_104D0(&predictedAxis_EB398ar) == 256)
				result = false;
		}
	}
	if (!isCaveLevel_D41B6)
		return result;
	axis_3d tempAxis = predictedAxis_EB398ar;
	//v7 = a1x->typeParams_0xA0_160x->word_160_0xc_12;
	v8 = a1x->typeParams_0xA0_160x->minHeight_160_0xc_12 + GetTerrainAlt_10C40(&tempAxis) + a1x->params_0x52_82.fov;
	v9 = GetCeilingAlt_10C60(&tempAxis);
	LOBYTE(v47) = tempAxis.x >> 8;
	HIBYTE(v47) = tempAxis.y >> 8;
	if (v8 < v9 - 576 && !(mapAngle_13B4E0[v47] & 8))
		;
	else
	{
		v48 = 0;
		LOWORD(v10) = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &tempAxis);
		v11 = v10 - 512;
		v50 = 0;
		BYTE1(v11) &= 7u;
		BYTE1(v10) = (BYTE1(v10) + 2) & 7;
		v43 = v11;
		v44 = v10;
		v12 = 16;
		for (i = 0; ; i++)
		{
			v39 = v12;
			if (i >= 6 || v50)
				break;
			v33x = tempAxis;
			v27x = tempAxis;
			v13 = v39;
			v37 = (unsigned __int16)v48;
			MoveEntity_57FA0(&v33x, v43, v48, v39);
			v38 = (signed __int16)GetCeilingAlt_10C60(&v33x);
			v14 = GetTerrainAlt_10C40(&v33x);
			v15 = v38 - v14;
			MoveEntity_57FA0(&v27x, v44, v37, v13);
			v38 = (signed __int16)GetCeilingAlt_10C60(&v27x);
			v16 = GetTerrainAlt_10C40(&v27x);
			v17 = v38 - v16;
			LOBYTE(v47) = v33x.x >> 8;
			HIBYTE(v47) = v33x.y >> 8;
			v18 = v27x.x >> 8;
			BYTE1(v18) = v27x.y >> 8;
			if (!(mapAngle_13B4E0[v47] & 8) || !(mapAngle_13B4E0[(unsigned __int16)v18] & 8))
			{
				if (v15 > v17 && !(mapAngle_13B4E0[v47] & 8))
				{
					if (!IsNearCeiling_11E20(a1x, &v33x))
					{
						v19x = &v33x;
						v50 = 1;
						v29x = *v19x;
					}
				}
				else if (v17 > v15 && !(mapAngle_13B4E0[(unsigned __int16)v18] & 8) && !IsNearCeiling_11E20(a1x, &v27x))
				{
					v19x = &v27x;
					v50 = 2;
					v29x = *v19x;
				}
			}
			v12 = 16 * (i + 1) + v39;
		}
		if (v50)
		{
			predictedAxis_EB398ar = v29x;
			v23 = (17 * i) / 6;
			if (v50 == 1)
				v23 = -v23;
			a1x->yaw_0x1C_28 = (a1x->yaw_0x1C_28 + v23) & 0x7ffu;
		}
		else if (IsNearCeiling_11E20(a1x, &tempAxis))
		{
			result = false;
		}
		else
		{
			predictedAxis_EB398ar = tempAxis;
		}
	}
	if (result)
	{
		LOBYTE(v47) = HIBYTE(predictedAxis_EB398ar.x);
		HIBYTE(v47) = HIBYTE(predictedAxis_EB398ar.y);
		if (mapAngle_13B4E0[v47] & 8)
			result = false;
	}
	if (!result)
	{
		a1x->playerData_0xA4_164x->waterCounter_0x262_610++;//0x23E4CC inc byte ptr [eax+262h]
		predictedAxis_EB398ar = a1x->position_0x4C_76;
		a1x->playerData_0xA4_164x->speed_0xc_12 = 0;
		if (a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[3])
			Entities_EA3E4[a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[3]]->stage_0x2E_46 = 0;
	}
	return result;
}

//----- (0005D530) --------------------------------------------------------
void MovePlayerWizard_5D530(type_entity_0x6E8E* a1x)//*(x_DWORD *)(a1 + 160)//23e530 (better break on 23e534)
{
	int locIntTemp;
	int locActSpeed;
	int locAlt;

	if (a1x->flags_0xc_12_15.byte[1] & 8)
	{
		a1x->flags_0xc_12_15.byte[1] &= 0xF7;
		return;
	}
	predictedAxis_EB398ar = a1x->position_0x4C_76;
	if (a1x->playerData_0xA4_164x->moveSpeed_0x14C_332)
	{
		locIntTemp = a1x->playerData_0xA4_164x->rollDelta_0x4_4 * (4 - a1x->playerData_0xA4_164x->moveSpeed_0x14C_332);
		a1x->playerData_0xA4_164x->roll_0x155_341 += (locIntTemp - (my_sign32(locIntTemp) * 3)) >> 2;
		locIntTemp = a1x->playerData_0xA4_164x->pitchDelta_0x6_6 * (4 - a1x->playerData_0xA4_164x->moveSpeed_0x14C_332);
		a1x->playerData_0xA4_164x->pitch_0x157_343 += (locIntTemp - (my_sign32(locIntTemp) * 3)) >> 2;
	}
	else
	{
		a1x->playerData_0xA4_164x->roll_0x155_341 += a1x->playerData_0xA4_164x->rollDelta_0x4_4;
		a1x->playerData_0xA4_164x->pitch_0x157_343 += a1x->playerData_0xA4_164x->pitchDelta_0x6_6;
	}
	locIntTemp = a1x->playerData_0xA4_164x->roll_0x155_341;
	a1x->yaw_0x1C_28 = (a1x->yaw_0x1C_28 + ((locIntTemp - (my_sign32(locIntTemp) * 7)) >> 3)) & 0x7FF;
	int speedSign = a1x->playerData_0xA4_164x->speed_0xc_12 - a1x->actSpeed_0x82_130;
	if (a1x->playerData_0xA4_164x->speed_0xc_12 != a1x->actSpeed_0x82_130)
	{
		if (speedSign <= 0)
			speedSign = -1;
		else
			speedSign = 1;
	}
	a1x->actSpeed_0x82_130 += speedSign * speedIcrement_D4B84;
	int altDiff = ((predictedAxis_EB398ar.z - GetTerrainAlt_10C40(&predictedAxis_EB398ar) - a1x->typeParams_0xA0_160x->maxHeight_160_0xa_10) << 10)
		/ a1x->typeParams_0xA0_160x->maxHeight_160_0xa_10;
	if (altDiff < -256)
		altDiff = -256;
	if (altDiff > 256)
		altDiff = 256;
	int16_t tempPitch = a1x->playerData_0xA4_164x->pitch_0x157_343 & 0x7ffu;
	a1x->pitch_0x1E_30 = tempPitch;
	if (tempPitch > 1024)
		tempPitch -= 2048;
	if (a1x->actSpeed_0x82_130 >= 0 || tempPitch <= 0)
	{
		if (a1x->actSpeed_0x82_130 < 0 && tempPitch < 0)
			a1x->playerData_0xA4_164x->pitch_0x24_36 = a1x->playerData_0xA4_164x->pitch_0x157_343;
		else if (a1x->actSpeed_0x82_130 > 0 && tempPitch < 0)
			a1x->playerData_0xA4_164x->pitch_0x24_36 = ((tempPitch * -altDiff - (my_sign32(tempPitch * -altDiff) * 255)) >> 8);
		else if (a1x->actSpeed_0x82_130 > 0 && tempPitch > 0)
			a1x->playerData_0xA4_164x->pitch_0x24_36 = a1x->playerData_0xA4_164x->pitch_0x157_343;
	}
	else
		a1x->playerData_0xA4_164x->pitch_0x24_36 = ((tempPitch * -altDiff - (my_sign32(a1x->pitch_0x1E_30 * -altDiff) * 255)) >> 8);
	a1x->playerData_0xA4_164x->pitch_0x24_36 &= 0x7ffu;
	if (a1x->playerData_0xA4_164x->moveSpeed_0x14C_332)
	{
		locIntTemp = a1x->actSpeed_0x82_130 * (4 - a1x->playerData_0xA4_164x->moveSpeed_0x14C_332);
		locActSpeed = (locIntTemp - (my_sign32(locIntTemp) * 3)) >> 2;//0x23E78C shl edx, 2 / sbb eax, edx
	}
	else if (a1x->playerData_0xA4_164x->mobilizeCounter_0x14E_334)
	{
		locActSpeed = 0;
	}
	else
	{
		locActSpeed = a1x->actSpeed_0x82_130;
	}
	MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, a1x->playerData_0xA4_164x->pitch_0x24_36, locActSpeed);
	if (a1x->playerData_0xA4_164x->strafeSpeed_0x10_16)
	{
		if (a1x->playerData_0xA4_164x->moveSpeed_0x14C_332)
		{
			locIntTemp = (4 - a1x->playerData_0xA4_164x->moveSpeed_0x14C_332) * a1x->playerData_0xA4_164x->strafeSpeed_0x10_16;
			locIntTemp = (locIntTemp - (my_sign32(locIntTemp) * 3)) >> 2;//0x23E803 shl edx, 2 / sbb eax, edx
		}
		else if (a1x->playerData_0xA4_164x->mobilizeCounter_0x14E_334)
			locIntTemp = 0;
		else
			locIntTemp = a1x->playerData_0xA4_164x->strafeSpeed_0x10_16;
		MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28 + 0x200, 0, locIntTemp);
	}
	if (a1x->playerData_0xA4_164x->moveBoost_0x1E_30)
	{
		if (a1x->playerData_0xA4_164x->moveBoost_0x1E_30 > 128)
			a1x->playerData_0xA4_164x->moveBoost_0x1E_30 = 128;
		MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->playerData_0xA4_164x->yaw_0x1E_30, 0, a1x->playerData_0xA4_164x->moveBoost_0x1E_30);
		if (a1x->playerData_0xA4_164x->moveBoost_0x1E_30)
		{
			if (a1x->playerData_0xA4_164x->moveBoost_0x1E_30 <= 0)
				locIntTemp = -1;
			else
				locIntTemp = 1;
		}
		else
			locIntTemp = 0;
		a1x->playerData_0xA4_164x->moveBoost_0x1E_30 += locIntTemp * moveBoosStep_D4B90;
		if (abs(a1x->playerData_0xA4_164x->moveBoost_0x1E_30) < 4)
			a1x->playerData_0xA4_164x->moveBoost_0x1E_30 = 0;
	}
	predictedAxis_EB398ar.x += a1x->playerData_0xA4_164x->xAdd_0x1A6_422;
	predictedAxis_EB398ar.y += a1x->playerData_0xA4_164x->yAdd_0x1A8_424;
	predictedAxis_EB398ar.z += a1x->playerData_0xA4_164x->zAdd_0x1AA_426;
	a1x->playerData_0xA4_164x->xAdd_0x1A6_422 = 0;
	a1x->playerData_0xA4_164x->yAdd_0x1A8_424 = 0;
	a1x->playerData_0xA4_164x->zAdd_0x1AA_426 = 0;
	if (a1x->playerData_0xA4_164x->waterCounter_0x262_610)
		a1x->playerData_0xA4_164x->waterCounter_0x262_610--;
	UpdateWizardMount_5DE30(a1x);
	if (a1x->playerData_0xA4_164x->moveSpeed_0x14C_332)
	{
		a1x->playerData_0xA4_164x->moveSpeedCounter_0x14D_333--;
		if (!a1x->playerData_0xA4_164x->moveSpeedCounter_0x14D_333)
		{
			a1x->playerData_0xA4_164x->moveSpeed_0x14C_332--;
			if (a1x->playerData_0xA4_164x->moveSpeed_0x14C_332)
			{
				a1x->playerData_0xA4_164x->moveSpeedCounter_0x14D_333 = 8;
				gameState_D41A4.moveSpeedFlag_181 = 1;
			}
			else
			{
				SetLocalPaletteSubMode_5C800(a1x, 1);
			}
		}
	}
	if (a1x->playerData_0xA4_164x->mobilizeCounter_0x14E_334)
	{
		a1x->playerData_0xA4_164x->mobilizeCounter2_0x150_336--;
		if (!a1x->playerData_0xA4_164x->mobilizeCounter2_0x150_336)
			a1x->playerData_0xA4_164x->mobilizeCounter_0x14E_334--;
	}
	if (MoveTest_5D0A0(a1x))
	{
		locAlt = GetTerrainAlt_10C40(&predictedAxis_EB398ar);
		if (a1x->playerData_0xA4_164x->mobilizeCounter_0x14E_334)
		{
			predictedAxis_EB398ar.z -= 51;
		}
		else
		{
			if (predictedAxis_EB398ar.z > a1x->typeParams_0xA0_160x->minHeight_160_0xc_12 + locAlt)
				predictedAxis_EB398ar.z += a1x->typeParams_0xA0_160x->climbSpeed_160_0xe_14;
		}
		if (predictedAxis_EB398ar.z >= locAlt + a1x->typeParams_0xA0_160x->minHeight_160_0xc_12)
		{
			if (isCaveLevel_D41B6)
			{
				locIntTemp = GetCeilingAlt_10C60(&predictedAxis_EB398ar);
				if (predictedAxis_EB398ar.z > locIntTemp - 384)
					predictedAxis_EB398ar.z = locIntTemp - 384;
			}
		}
		else
		{
			predictedAxis_EB398ar.z = a1x->typeParams_0xA0_160x->minHeight_160_0xc_12 + locAlt;
		}
		CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
	}
	else
	{
		UpdateWizardWaterEffect_5DD50(a1x);
	}
	if (!(a1x->instanceIndex_0x3E_62 & 0x3F))
	{
		if (Entities_EA3E4[a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[4]] > Entities_EA3E4[0])
		{
			if (Entities_EA3E4[a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[4]]->stage_0x2E_46)
			{
				switch (SPELLS_BEGIN_BUFFER_str[4].subspell[Entities_EA3E4[a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[4]]->levelPar3_0x46_70].life_0x1A)
				{
					case 0x2:
						PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 12);//0x2
						break;
					case 0x10:
						PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 39);//0x10
						break;
					case 0x13:
						PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 43);//0x13
						break;
					case 0x19:
						PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 37);//0x19
						break;
				}
			}
		}
	}
	if (world_D41A0.terrain_2FECE.MapType == MapType_t::Cave)
	{
		world_D41A0.rand_0x8 = 9377 * world_D41A0.rand_0x8 + 9439;
		world_D41A0.rand_0x8 = world_D41A0.rand_0x8 + gameState_D41A4.setting_30;
		if ((int)(world_D41A0.rand_0x8 % 0x83) < 5)
		{
			if (world_D41A0.LevelIndex_0xc == a1x->playerData_0xA4_164x->playerColorIndex_0x38_56)
				PrepareEventSound_6E450(0, world_D41A0.LevelIndex_0xc, world_D41A0.rand_0x8 % 0x83 + 65);
		}
	}
	else if (TerrainTileIsWater_104D0(&a1x->position_0x4C_76) == 1)
	{
		PrepareEventSound_6E450(0, world_D41A0.LevelIndex_0xc, 1);
		EndLoop_6EAB0(0, world_D41A0.LevelIndex_0xc, 2);
	}
	else
	{
		PrepareEventSound_6E450(0, world_D41A0.LevelIndex_0xc, 2);
		EndLoop_6EAB0(0, world_D41A0.LevelIndex_0xc, 1);
	}
	if (a1x->playerData_0xA4_164x->screenShake_0x19A_410 >= 1536)
	{
		EndLoop_6EAB0(0, world_D41A0.LevelIndex_0xc, 5);
	}
	else
	{
		PrepareEventSound_6E450(0, world_D41A0.LevelIndex_0xc, 5);
		a1x->playerData_0xA4_164x->screenShake_0x19A_410 = 2048;
	}
	if (a1x->playerData_0xA4_164x->maxDistance_0x19E_414 >= 1536)
	{
		//End building sound
		EndLoop_6EAB0(0, world_D41A0.LevelIndex_0xc, 31);
	}
	else
	{
		PrepareEventSound_6E450(0, world_D41A0.LevelIndex_0xc, 31);
		a1x->playerData_0xA4_164x->maxDistance_0x19E_414 = 2048;
	}
	if (a1x->playerData_0xA4_164x->playerColorIndex_0x38_56 == world_D41A0.LevelIndex_0xc)
	{
		if (a1x->playerData_0xA4_164x->hitTimer_0x36_54 <= 0)
		{
			UpdateMusic_99970(1, 3u);
		}
		else
		{
			a1x->playerData_0xA4_164x->hitTimer_0x36_54--;
			UpdateMusic_99970(2, 3u);
		}
	}
}

//----- (0005DD50) --------------------------------------------------------
void UpdateWizardWaterEffect_5DD50(type_entity_0x6E8E* a1x)//23dd50
{
	int v1; // eax
	//signed int result; // eax
	char v3; // [esp+0h] [ebp-4h]

	v3 = 0;
	if (TerrainTileIsWater_104D0(&a1x->position_0x4C_76) == 256
		|| isCaveLevel_D41B6
		&& (v1 = a1x->position_0x4C_76.x >> 8,
			BYTE1(v1) = a1x->position_0x4C_76.y >> 8,
			mapAngle_13B4E0[(unsigned __int16)v1] & 8))
	{
		v3 = 1;
	}
	if (!v3 && isCaveLevel_D41B6 && a1x->playerData_0xA4_164x->nearCeiling_0x261_609 && IsNearCeiling_11E20(a1x, &a1x->position_0x4C_76))
		v3 = 1;
	if (v3)
	{
		a1x->playerData_0xA4_164x->nearCeiling_0x261_609 = 1;
		predictedAxis_EB398ar = a1x->position_0x4C_76;
		MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, 128);
		/*result = */CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
	}
	else
	{
		//result = a1x->dword_0xA4_164;
		a1x->playerData_0xA4_164x->nearCeiling_0x261_609 = 0;
	}
	//return result;
}
// D41B6: using guessed type char x_BYTE_D41B6;
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (0005DE30) --------------------------------------------------------
void UpdateWizardMount_5DE30(type_entity_0x6E8E* a1x)//23ee30
{
	//int v1; // edx
	type_entity_0x6E8E* v2x; // ebx
	type_entity_0x6E8E* v3x; // edi
	__int16 v4; // cx
	signed int v5; // ecx
	int v6; // edi
	__int16 v7; // ax
	unsigned __int16 v8; // ST10_2
	__int16 v9; // ax
	char v10; // al
	int v11; // esi
	int v12; // [esp+4h] [ebp-Ch]
	//char *v13; // [esp+8h] [ebp-8h]

	//v1 = a1x->dword_0xA4_164;
	v2x = Entities_EA3E4[a1x->playerData_0xA4_164x->mountId_0x146_326];
	v3x = 0;
	if (v2x > Entities_EA3E4[0])
	{
		//v13 = (char *)& SPELLS_BEGIN_BUFFER_DA818[0x15a +0x308] + 26 * a1x->playerData_0xA4_164x->word_0x14A_330;
		//v13 = SPELLS_BEGIN_BUFFER_str[14].subspell[a1x->playerData_0xA4_164x->word_0x14A_330].dword_2
		v4 = a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[14];
		if (v4)
			v3x = Entities_EA3E4[v4];
		v5 = Distance3D_583F0(&a1x->position_0x4C_76, &v2x->position_0x4C_76);
		if (v3x && v3x->stage_0x2E_46 && v5 < SPELLS_BEGIN_BUFFER_str[14].subspell[a1x->playerData_0xA4_164x->duelLevel_0x14A_330].subSpellIndex_2 && v2x->life_0x8 >= 0)
		{
			v12 = 3 * a1x->minSpeed_0x84_132 / 2;
			LOWORD(v6) = (v5 - a1x->playerData_0xA4_164x->targetDistance_0x142_322) / (1024 / (signed __int16)v12);
			if ((signed __int16)v6 < -(signed __int16)v12)
				v6 = 3 * a1x->minSpeed_0x84_132 / -2;
			if ((signed __int16)v6 > (signed __int16)v12)
				LOWORD(v6) = v12;
			v7 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v2x->position_0x4C_76);
			v8 = v7;
			v9 = a1x->yaw_0x1C_28 + TurnTowardsAngle_58350(a1x->yaw_0x1C_28, v7, 5, 0x82u);
			HIBYTE(v9) &= 7u;
			a1x->yaw_0x1C_28 = v9;
			MoveEntity_57FA0(&predictedAxis_EB398ar, v8, a1x->pitch_0x1E_30, v6);
			v10 = SPELLS_BEGIN_BUFFER_str[14].subspell[a1x->playerData_0xA4_164x->duelLevel_0x14A_330].life_0x1A;
			if ((unsigned __int8)v10 >= 1u)
			{
				if ((unsigned __int8)v10 > 1u)
				{
					if (v10 != 2)
						return;
					v2x->life_0x8 -= v2x->playerData_0xA4_164x->lifeRegen_0x163_355 + 2;
				}
				v11 = v2x->mana_0x90_144 - (v2x->manaRegen_0x88_136 + 8);
				v2x->mana_0x90_144 = v11;
				if (v11 < 0)
					v2x->mana_0x90_144 = 0;
			}
		}
		else
		{
			a1x->playerData_0xA4_164x->mountId_0x146_326 = 0;
		}
	}
}
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;

//----- (0005E010) --------------------------------------------------------
void UpdatePlayer_5E010(type_entity_0x6E8E* a1x)//23f010
{
	bool locIsOk = false;
	a1x->minSpeed_0x84_132 = wizardSpeed2_D4B8C;
	if (!(gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED))
	{
		if (a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58)
		{
			if (EntitiesOverlap_106C0(a1x, Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]))
				locIsOk = true;
		}
	}
	UpdateWizardSpeed_5F380(a1x);
	if (!(gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED) && locIsOk)
	{
		if (a1x->mail_0x5E_94.channel[0].source)
		{
			if (Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]->mail_0x5E_94.channel[0].source)
				Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]->mail_0x5E_94.channel[0].amount.dword += a1x->mail_0x5E_94.channel[0].amount.dword;
			else
				Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]->mail_0x5E_94.channel[0].amount.dword = a1x->mail_0x5E_94.channel[0].amount.dword;
			Entities_EA3E4[a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58]->mail_0x5E_94.channel[0].source = a1x->mail_0x5E_94.channel[0].source;
		}
		a1x->playerData_0xA4_164x->invulnerableTimer_0x159_345 = 2;
	}
	if (!(gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED))
	{
		if (a1x->playerData_0xA4_164x->invulnerableTimer_0x159_345)
		{
			memset((void*)&a1x->mail_0x5E_94, 0, 36);
			a1x->playerData_0xA4_164x->invulnerableTimer_0x159_345--;
		}
		else
		{
			UpdateCastleTarget_5EFA0(a1x);
		}
		if (a1x->playerData_0xA4_164x->chargeTime_0x154_340 < 200)
			a1x->playerData_0xA4_164x->chargeTime_0x154_340++;
	}
	MovePlayerWizard_5D530(a1x);
	if (a1x->life_0x8 >= 0)
	{
		if (!(gameState_D41A4.OptionsSettingFlag_24 & GAME_PAUSED))
		{
			a1x->mana_0x90_144 += a1x->manaRegen_0x88_136;
			if (a1x->playerData_0xA4_164x->dword_0x18D_397)
				a1x->playerData_0xA4_164x->dword_0x18D_397--;
			else
				a1x->life_0x8 += a1x->playerData_0xA4_164x->lifeRegen_0x163_355;//cure
			if (a1x->playerData_0xA4_164x->word_0x24C_588)
				a1x->playerData_0xA4_164x->word_0x24C_588--;
			if (a1x->mana_0x90_144 < 0)
				a1x->mana_0x90_144 = 0;
			if (a1x->mana_0x90_144 > a1x->maxMana_0x8C_140)
				a1x->mana_0x90_144 = a1x->maxMana_0x8C_140;
			if (a1x->life_0x8 < -1)
				a1x->life_0x8 = -1;
			if (a1x->life_0x8 > a1x->maxLife_0x4)
				a1x->life_0x8 = a1x->maxLife_0x4;
			if (a1x->playerData_0xA4_164x->startTimer_0x16D_365)
				a1x->playerData_0xA4_164x->startTimer_0x16D_365--;
			if (a1x->playerData_0xA4_164x->attackedTimer_0x248_584)
				a1x->playerData_0xA4_164x->attackedTimer_0x248_584--;
			if (locIsOk || a1x->flags_0xc_12_15.byte[1] & 0x10)
			{
				a1x->manaRegen_0x88_136 = a1x->maxMana_0x8C_140 / 200;
				a1x->playerData_0xA4_164x->lifeRegen_0x163_355 = a1x->maxLife_0x4 / 250;
				if (a1x->manaRegen_0x88_136 < 1000)
					a1x->manaRegen_0x88_136 = 1000;
				a1x->flags_0xc_12_15.byte[1] &= 0xEFu;
			}
			else
			{
				a1x->manaRegen_0x88_136 = a1x->maxMana_0x8C_140 / 2000;
				a1x->playerData_0xA4_164x->lifeRegen_0x163_355 = a1x->maxLife_0x4 / 2000;
				if (a1x->manaRegen_0x88_136 < 100)
					a1x->manaRegen_0x88_136 = 100;
			}
		}
	}
	else
	{
		a1x->actionIndex_0x45_69 = 2;
		a1x->param2_0x2C_44 = 0;
		PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 16);
		EventDispatcher::I->DispatchEvent(EventType::E_SCENE_CHANGE, Scene::DEAD);
	}
}

//----- (0005E310) --------------------------------------------------------
void MultiplayerTestDie_5E310(type_entity_0x6E8E* a1x)//23f310
{
	//int v1; // edx
	__int16 v2; // cx
	int v3; // esi
	__int16 v4; // ax
	__int16 v5; // di
	type_entity_0x6E8E* v6x; // eax
	//int result; // eax
	unsigned __int16 v8; // di
	type_entity_0x6E8E* v9x; // eax
	char v10; // dl
	//int v11; // edx
	int v12; // eax
	//int v13; // edx
	char* v14; // esi
	char* v15; // edi
	//char v16; // al
	//char v17; // al
	//int v18; // eax
	type_entity_0x6E8E* v19x; // edx
	__int16 v20; // di
	__int16 v21; // ax
	//int v22; // ecx
	type_entity_0x6E8E* jx; // ecx
	type_entity_0x6E8E* v24x; // [esp+0h] [ebp-Ch]
	signed int i; // [esp+4h] [ebp-8h]
	type_entity_0x6E8E* v26x; // [esp+8h] [ebp-4h]

	MovePlayerWizard_5D530(a1x);
	if (gameState_D41A4.colorIndex_121[6])
		SetLocalPaletteSubMode_5C800(a1x, 7);
	//v1 = 2124 * a1x->playerData_0xA4_164x->word_0x38_56;
	if (world_D41A0.players_0x2BDE[a1x->playerData_0xA4_164x->playerColorIndex_0x38_56].MenuState_0x3DF_2BE4_12221)
		SetMenuCursorPosition_52E90(&world_D41A0.players_0x2BDE[a1x->playerData_0xA4_164x->playerColorIndex_0x38_56], 0, false);
	v2 = a1x->param2_0x2C_44 - 2;
	a1x->position_0x4C_76.z += a1x->param2_0x2C_44;
	a1x->param2_0x2C_44 = v2;
	if (v2 < -256)
		a1x->param2_0x2C_44 = -256;
	if (a1x->param2_0x2C_44 > 0)
		a1x->param2_0x2C_44 = 0;
	v3 = a1x->typeParams_0xA0_160x->minHeight_160_0xc_12;
	v4 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	v5 = v4;
	if (a1x->position_0x4C_76.z < v3 + v4)
		a1x->position_0x4C_76.z = a1x->typeParams_0xA0_160x->minHeight_160_0xc_12 + v4;
	v6x = CreateEntity_4A190(&predictedAxis_EB398ar, 10, 1);
	if (v6x)
	{
		v6x->flags_0xc_12_15.byte[0] |= 0x80u;
		v6x->id_0x1A_26 = a1x->id_0x1A_26;
	}
	//result = v5 + *(signed __int16 *)(*(x_DWORD *)(a1 + 160) + 12);
	if (a1x->position_0x4C_76.z == v5 + a1x->typeParams_0xA0_160x->minHeight_160_0xc_12)
	{
		RebuildEntityLists_49F90();
		v8 = a1x->ownerPlayerId_0x24_36;
		if (v8)
		{
			v9x = Entities_EA3E4[v8];
			if (v9x->class_0x3F_63 == 3)
			{
				v10 = v9x->model_0x40_64;
				if (!v10 || v10 == 1)
				{
					//v11 = Entities_EA3E4[a1x->word_0x24_36]->dword_0xA4_164;
					v12 = a1x->playerData_0xA4_164x->playerColorIndex_0x38_56;
					Entities_EA3E4[a1x->ownerPlayerId_0x24_36]->playerData_0xA4_164x->kills_0x26_38[v12]++;
					//++*(x_WORD *)(v11 + 2 * v12 + 38);
				}
			}
		}
		memset((void*)&a1x->mail_0x5E_94, 0, 36);
		//v13 = 2124 * a1x->playerData_0xA4_164x->word_0x38_56 + (int)x_D41A0_BYTEARRAY_0 + 11230;
		v14 = (char*)langStrings_E9C4C[374];//has died.
		//v15 = (char *)(v13 + 28);
		v15 = world_D41A0.players_0x2BDE[a1x->playerData_0xA4_164x->playerColorIndex_0x38_56].CurrentNotificationText_0x01c_2BFA_11258;
		strcpy(v15, v14);
		/*do
		{
			v16 = *v14;
			*v15 = *v14;
			if (!v16)
				break;
			v17 = v14[1];
			v14 += 2;
			v15[1] = v17;
			v15 += 2;
		} while (v17);*/
		world_D41A0.players_0x2BDE[a1x->playerData_0xA4_164x->playerColorIndex_0x38_56].endState_0x04f_2C2D_11309 = 1;
		world_D41A0.players_0x2BDE[a1x->playerData_0xA4_164x->playerColorIndex_0x38_56].CurrentNotificationDuration_0x04d_2C2B_11307 = 100;
		for (i = 0; i < 26; i++)
		{
			//v18 = a1x->dword_0xA4_164 + 2 * i;
			//v19x = Entities_EA3E4[*(signed __int16 *)(v18 + 819)];
			v19x = Entities_EA3E4[a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[i]];
			v26x = v19x;
			if (v19x <= Entities_EA3E4[0])
			{
				a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[i] = 0;
			}
			else
			{
				a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[i] = 1;
				v19x->flags_0xc_12_15.byte[0] &= 0xFEu;
				v19x->actionIndex_0x45_69++;
				predictedAxis_EB398ar = a1x->position_0x4C_76;
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				v20 = a1x->rand_0x14_20;
				predictedAxis_EB398ar.x += (a1x->rand_0x14_20 & 0x1FF) - 256;
				v21 = 9377 * v20 + 9439;
				a1x->rand_0x14_20 = v21;
				predictedAxis_EB398ar.y += (v21 & 0x1FF) - 256;
				CopyEntityPosition_57CF0(v19x, &predictedAxis_EB398ar);
				a1x->rand_0x14_20 = 9377 * a1x->rand_0x14_20 + 9439;
				v26x->life_0x8 = a1x->rand_0x14_20 % 0x5Au + 200;
			}
		}
		v24x = CreateEntity_4A190(&a1x->position_0x4C_76, 10, 40);
		if (v24x)
		{
			a1x->actionIndex_0x45_69 = 3;
			//v22 = (int)x_D41A0_BYTEARRAY_4;
			//v22 = (int)x_D41A0_BYTEARRAY_4;
			a1x->param_0x10_16 = 1200;
			for (jx = gameState_D41A4.manaSphereList_38523; jx > Entities_EA3E4[0]; jx = jx->next_0)
			{
				if (jx->model_0x40_64 == 39 && jx->playerEntityIndex_0x94_148 == a1x - world_D41A0.entities_0x6E8E)
					jx->playerEntityIndex_0x94_148 = v24x - world_D41A0.entities_0x6E8E;
			}
		}
		a1x->flags_0xc_12_15.byte[0] |= 0x20u;
		//result = (int)x_D41A0_BYTEARRAY_0;
		world_D41A0.freeEntityCount2_0x11e6--;
	}
	//return result;
}
// 8C250: using guessed type x_DWORD memset(x_DWORD, x_DWORD, x_DWORD);
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// EA224: using guessed type int x_DWORD_EA224;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// EB39A: using guessed type __int16 x_WORD_EB398ar[1];

//----- (0005E6C0) --------------------------------------------------------
__int16 CheckWizardKiller_5E6C0(type_entity_0x6E8E* a1x)//23f6c0
{
	char v1; // al
	axis_3d* v2x; // edi
	__int16 v3; // ax
	__int16 v4; // ST04_2
	unsigned __int16 v5; // ST00_2
	__int16 v6; // ax
	//unsigned __int16 v7; // ST00_2
	__int16 v8; // ax
	__int16 result; // ax

	v1 = 0;
	if (!a1x->ownerPlayerId_0x24_36)
		v1 = 1;
	if (v1)
	{
		a1x->yaw_0x1C_28 += 5;
		a1x->yaw_0x1C_28 &= 0x7ff;
		v8 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
		HIBYTE(v8)++;
	}
	else
	{
		v2x = &Entities_EA3E4[a1x->ownerPlayerId_0x24_36]->position_0x4C_76;
		a1x->roll_0x20_32 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, v2x);
		v3 = Maths::GetPitchAngle_58210(&a1x->position_0x4C_76, v2x);
		v4 = a1x->roll_0x20_32;
		v5 = a1x->yaw_0x1C_28;
		a1x->fov_0x22_34 = v3;
		v6 = TurnTowardsAngle_58350(v5, v4, 5, 0x16u);
		//LOWORD(v2) = a1x->fov_0x22_34;
		a1x->yaw_0x1C_28 += v6;
		//v7 = a1x->word_0x1E_30;
		a1x->yaw_0x1C_28 &= 0x7ff;
		a1x->pitch_0x1E_30 += TurnTowardsAngle_58350(a1x->pitch_0x1E_30, a1x->fov_0x22_34, 5, 0x16u);
		a1x->pitch_0x1E_30 &= 0x7ff;
		v8 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	}
	a1x->position_0x4C_76.z = v8;
	a1x->pitch_0x1E_30 = 0;
	result = a1x->pitch_0x1E_30;
	a1x->playerData_0xA4_164x->pitch_0x157_343 = result;
	a1x->playerData_0xA4_164x->roll_0x155_341 = 0;
	return result;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (0005E7C0) --------------------------------------------------------
void MultiplayerTestBanished_5E7C0(type_entity_0x6E8E* a1x)//23f7c0
{
	//int v1; // eax
	char v1x;
	//int v2; // ebx
	//uint8_t* v3; // edx
	int v4; // ebx
	char* v5; // esi
	char* v6; // edi

	a1x->playerData_0xA4_164x->moveBoost_0x1E_30 = 0;
	//v1 = a1x->dword_0xA4_164;
	//v2 = 2124 * *(signed __int16 *)(v1 + 56);
	if (world_D41A0.players_0x2BDE[a1x->playerData_0xA4_164x->playerColorIndex_0x38_56].IsAiPlayer_0x009_2BE4_11239 == 1)
	{
		//v3 = &x_D41A0_BYTEARRAY_0[v2 + 11230];
		if (a1x->playerData_0xA4_164x->CastleEntityIndex_0x3A_58)
		{
			v4 = a1x->param_0x10_16;
			if (v4)
				a1x->param_0x10_16 = v4 - 1;
			else
				/*LOBYTE(v1) = */UpdatePlayerWizard_5C950(&world_D41A0.players_0x2BDE[a1x->playerData_0xA4_164x->playerColorIndex_0x38_56], a1x);
		}
		else
		{
			if (world_D41A0.players_0x2BDE[a1x->playerData_0xA4_164x->playerColorIndex_0x38_56].playerActive_0x006_2BE4_11236)
			{
				v5 = langStrings_E9C4C[HAS_BEEN_BANISHED];//has been banished from the realm.
				v6 = world_D41A0.players_0x2BDE[a1x->playerData_0xA4_164x->playerColorIndex_0x38_56].CurrentNotificationText_0x01c_2BFA_11258;//&v3[28];
				strcpy(v6, v5);
				/*do
{
	LOBYTE(v1) = v5[0];
	*v6 = *v5;
	if (!(x_BYTE)v1)
		break;
	LOBYTE(v1) = v5[1];
	v5 += 2;
	v6[1] = v1;
	v6 += 2;
} while ((x_BYTE)v1);*/
				world_D41A0.players_0x2BDE[a1x->playerData_0xA4_164x->playerColorIndex_0x38_56].endState_0x04f_2C2D_11309 = 1;
				world_D41A0.players_0x2BDE[a1x->playerData_0xA4_164x->playerColorIndex_0x38_56].CurrentNotificationDuration_0x04d_2C2B_11307 = 200;
			}
			// An AI opponent leaving the realm counts on the scoreboard exactly like a human
			// one: it holds a player slot, and whoever put it down gets the kill.  Its way out
			// is here rather than through PlayerEvents, which is why it needs its own hook.
			MatchScoreRecordKnockOut(a1x->playerData_0xA4_164x->playerColorIndex_0x38_56, a1x);
			world_D41A0.players_0x2BDE[a1x->playerData_0xA4_164x->playerColorIndex_0x38_56].playerActive_0x006_2BE4_11236 = 0;
		}
	}
	else
	{
		SetLocalPaletteSubMode_5C800(a1x, 7);
		v1x = CheckWizardKiller_5E6C0(a1x);
	}
	//return v1x;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA0B8: using guessed type int x_DWORD_EA0B8;

//----- (0005E8C0) --------------------------------------------------------
signed int EndGameSequence_5E8C0(type_entity_0x6E8E* a1x)//23f8c0 //end game sequences
{
	__int16 v1; // si
	//int v2; // eax
	unsigned __int8 v3; // dl
	__int16 v4; // di
	__int16 v5; // dx
	type_entity_0x6E8E* v6x; // eax
	char v7; // dl
	int v8; // esi
	signed __int16 v9; // ax
	__int16 v10; // si
	//int v11; // eax
	//int v12; // ecx
	//int v13; // edx
	type_entity_0x6E8E* v14x; // esi
	//__int16 v15; // di
	int v16; // esi
	__int16 v17; // cx
	//int v18; // eax
	//int v19; // eax
	//int v20; // eax
	//int v21; // edx
	//int v22; // eax
	__int16 v23; // ax
	int v24; // edx
	//int v25; // esi
	//int v26; // ecx
	signed int result; // eax
	char v28; // [esp+0h] [ebp-4h]

	char dataPath[MAX_PATH];

	v28 = 0;
	switch (a1x->levelPar3_0x46_70)
	{
	case 0:
		SetMenuCursorPosition_52E90(&world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc], 0, false);
		if (a1x->playerData_0xA4_164x->playerColorIndex_0x38_56 == world_D41A0.LevelIndex_0xc)
		{
			PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 41);
			SetLocalPaletteSubMode_5C800(a1x, 6);
		}
		a1x->playerData_0xA4_164x->mobilizeCounter_0x14E_334 = 0;
		a1x->playerData_0xA4_164x->moveSpeed_0x14C_332 = 0;
		a1x->playerData_0xA4_164x->moveBoost_0x1E_30 = 0;
		a1x->playerData_0xA4_164x->strafeSpeed_0x10_16 = 0;
		v1 = a1x->playerData_0xA4_164x->str_611.SpellsEnabled_0x333_819x.SpellEnabled[3];
		if (v1)
			Entities_EA3E4[v1]->stage_0x2E_46 = 0;
		//v2 = (int)x_D41A0_BYTEARRAY_0;
		world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] |= 0x20u;
		v3 = a1x->actionIndex_0x45_69;
		a1x->targetId_0x96_150 = 0;
		if (v3 >= 0xBu)
		{
			if (v3 <= 0xBu)
			{
				v5 = world_D41A0.switchOnceEntity_0x36DFC;
				if (v5)
					a1x->targetId_0x96_150 = v5;
			}
			else if (v3 == 12)
			{
				v4 = world_D41A0.switchChainEntity_0x36DFE;
				if (v4)
					a1x->targetId_0x96_150 = v4;
			}
		}
		v6x = Entities_EA3E4[a1x->targetId_0x96_150];
		if (v6x > Entities_EA3E4[0])
		{
			if (v6x->class_0x3F_63 != 14 || (v7 = v6x->model_0x40_64, v7 != 4) && v7 != 3)
				a1x->targetId_0x96_150 = 0;
		}
		a1x->levelPar3_0x46_70 = 1;
		break;
	case 1://change lightness
		predictedAxis_EB398ar = a1x->position_0x4C_76;
		v8 = a1x->actSpeed_0x82_130;
		MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, a1x->actSpeed_0x82_130);
		if (MoveTest_5D0A0(a1x))
			CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		else
			a1x->actSpeed_0x82_130 = 0;
		if (abs(v8) <= 4)
		{
			a1x->actSpeed_0x82_130 = 0;
		}
		else
		{
			if (v8 <= 0)
				v9 = 1;
			else
				v9 = -1;
			a1x->actSpeed_0x82_130 += 4 * v9;
		}
		if (!a1x->actSpeed_0x82_130)
			a1x->levelPar3_0x46_70 = (a1x->targetId_0x96_150 == 0) + 3;
		break;
	case 3://change rotation
		v10 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &Entities_EA3E4[a1x->targetId_0x96_150]->position_0x4C_76);
		if ((unsigned __int16)AngleDifference_582B0(a1x->yaw_0x1C_28, v10) <= 0xBu)
		{
			a1x->levelPar3_0x46_70 = 4;
			a1x->yaw_0x1C_28 = v10;
		}
		else
		{
			a1x->yaw_0x1C_28 += TurnTowardsAngle_58350(a1x->yaw_0x1C_28, v10, 0, 0xBu);
			a1x->yaw_0x1C_28 &= 0x7ff;
		}
		break;
	case 4:
		a1x->param_0x10_16 = 12;//sequence steps
		//v11 = (int)x_D41A0_BYTEARRAY_0;
		a1x->levelPar3_0x46_70 = 5;
		world_D41A0.gameFlags_0x36E0B |= 2u;
		goto LABEL_34;
	case 5:
	LABEL_34:
		//v12 = a1x->dword_0x10_16 - 1;
		a1x->param_0x10_16--;// = v12;
		if (a1x->param_0x10_16)
		{
			if (a1x->param_0x10_16 > 4)
				world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].positions_0x1d1_2BDE_11695[0].rotation__2BDE_11701.fov--;

			//--*(x_WORD *)(2124 * D41A0_BYTESTR_0.word_0xc + x_D41A0_BYTEARRAY_0 + 11707);
		}
		else
		{
			if (a1x->targetId_0x96_150)
				a1x->levelPar3_0x46_70 = 6;
			else
				a1x->levelPar3_0x46_70 = 8;
			if (ptrMemoryBuffer_E9C3C && (world_D41A0.terrain_2FECE.MapType == MapType_t::Day))
			{
				StopCdPlayback_86860(cdDrive_1803EC);
				sprintf(dataPath, "%s/%s", cdDataPath.c_str(), "DATA/GTD2.DAT");
				DataFileIO::ReadFileAndDecompress(dataPath, &tables2_FAEE0); //fix it
			}
		}
		break;
	case 6:
		a1x->param_0x10_16 = 512;
		a1x->actSpeed_0x82_130 = 100;
		a1x->levelPar3_0x46_70 = 7;
		v28 = 1;
		goto LABEL_44;
	case 7:
	LABEL_44:
		//v13 = a1x->dword_0x10_16 - 1;
		a1x->param_0x10_16--;// = v13;
		if (!a1x->param_0x10_16)
			goto LABEL_51;
		predictedAxis_EB398ar = a1x->position_0x4C_76;
		v14x = Entities_EA3E4[a1x->targetId_0x96_150];
		a1x->yaw_0x1C_28 = Maths::GetYawAngle_581E0(&a1x->position_0x4C_76, &v14x->position_0x4C_76);
		MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, a1x->actSpeed_0x82_130);
		//v15 = a1x->word_0x82_130 + 8;
		a1x->actSpeed_0x82_130 += 8;// = v15;
		if (a1x->actSpeed_0x82_130 < 0)
			a1x->actSpeed_0x82_130 = 0;
		if (a1x->actSpeed_0x82_130 > 200)
			a1x->actSpeed_0x82_130 = 200;
		if (!MoveTest_5D0A0(a1x))
			goto LABEL_51;
		CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		if (Maths::EuclideanDistXYZ_58490(&a1x->position_0x4C_76, &v14x->position_0x4C_76) < 0x180)
			goto LABEL_51;
		break;
	case 8:
		a1x->param_0x10_16 = 128;
		a1x->actSpeed_0x82_130 = 100;
		a1x->levelPar3_0x46_70 = 9;
		v28 = 1;
		goto LABEL_53;
	case 9:
	LABEL_53:
		v16 = a1x->param_0x10_16 - 1;
		a1x->param_0x10_16 = v16;
		if (!v16)
			goto LABEL_51;
		predictedAxis_EB398ar = a1x->position_0x4C_76;
		MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, a1x->actSpeed_0x82_130);
		v17 = a1x->actSpeed_0x82_130 + 8;
		a1x->actSpeed_0x82_130 = v17;
		if (v17 < 0)
			a1x->actSpeed_0x82_130 = 0;
		if (a1x->actSpeed_0x82_130 > 200)
			a1x->actSpeed_0x82_130 = 200;
		if (MoveTest_5D0A0(a1x))
			CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		else
			LABEL_51:
		a1x->levelPar3_0x46_70 = 10;
		break;
	case 0xA:
		a1x->levelPar3_0x46_70 = 11;
		//v18 = a1x->dword_0xA4_164;
		a1x->param_0x10_16 = 32;
		if (a1x->playerData_0xA4_164x->playerColorIndex_0x38_56 == world_D41A0.LevelIndex_0xc)
			SetLocalPaletteSubMode_5C800(a1x, 5);
		goto LABEL_62;
	case 0xB:
	LABEL_62:
		//v19 = a1x->dword_0x10_16 - 1;
		a1x->param_0x10_16--;// = v19;
		if (a1x->param_0x10_16 && gameState_D41A4.paletteSubMod_180)
		{
			predictedAxis_EB398ar = a1x->position_0x4C_76;
			MoveEntity_57FA0(&predictedAxis_EB398ar, a1x->yaw_0x1C_28, 0, 2);
			if (MoveTest_5D0A0(a1x))
				CopyEntityPosition_57CF0(a1x, &predictedAxis_EB398ar);
		}
		else
		{
			a1x->levelPar3_0x46_70 = 12;
		}
		break;
	case 0xC:
		//v20 = (int)x_D41A0_BYTEARRAY_0;
		//v21 = 2124 * D41A0_BYTESTR_0.word_0xc;
		world_D41A0.miscSettings_0x21AE.xxxx_0x21B1 = 0;
		world_D41A0.m_GameSettings.m_Display.xxxx_0x2191 = 0;
		world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] = 10;
		if (a1x->actionIndex_0x45_69 == 11)
			gameState_D41A4.setting_38545 &= 0xFBu;
		if (a1x->actionIndex_0x45_69 == 11 || gameState_D41A4.setting_38545 & 0x10)
			world_D41A0.players_0x2BDE[world_D41A0.LevelIndex_0xc].stateFlags_0_2BDE_11230.byte[2] |= 0x10u;
		break;
	default:
		break;
	}
	if (v28)
	{
		if (a1x->playerData_0xA4_164x->playerColorIndex_0x38_56 == world_D41A0.LevelIndex_0xc)
		{
			PrepareEventSound_6E450(a1x - world_D41A0.entities_0x6E8E, -1, 19);
			if (ptrMemoryBuffer_E9C3C)
			{
				//v22 = (int)x_D41A0_BYTEARRAY_0;
				world_D41A0.miscSettings_0x21AE.xxxx_0x21B1 = 1;
				world_D41A0.m_GameSettings.m_Display.xxxx_0x2191 = 2;
			}
		}
	}
	v23 = GetTerrainAlt_10C40(&a1x->position_0x4C_76);
	v24 = a1x->position_0x4C_76.z;
	if (v24 <= v23 + 256)
	{
		if (v24 >= v23)
			a1x->position_0x4C_76.z = v23 + 128;
		else
			a1x->position_0x4C_76.z += 128;
	}
	else
	{
		a1x->position_0x4C_76.z -= 128;
	}
	//v25 = a1x->dword_0xA4_164;

	//a1x->playerData_0xA4_164x->word_0x155_341 -= (signed int)(a1x->playerData_0xA4_164x->word_0x155_341-(__CFSHL__(a1x->playerData_0xA4_164x->word_0x155_341 >> 32, 3)+ 8 * (a1x->playerData_0xA4_164x->word_0x155_341 >> 32))) >> 3;
	a1x->playerData_0xA4_164x->roll_0x155_341 -= (signed int)(a1x->playerData_0xA4_164x->roll_0x155_341 - (my_sign32(a1x->playerData_0xA4_164x->roll_0x155_341) * 7)) >> 3;

	//my_sign(a1x->playerData_0xA4_164x->word_0x155_341) * 7))

	//v26 = a1x->dword_0xA4_164;
	//result = (signed int)(a1x->playerData_0xA4_164x->word_0x157_343- (__CFSHL__(a1x->playerData_0xA4_164x->word_0x157_343 >> 32, 2)+ 4 * (a1x->playerData_0xA4_164x->word_0x157_343 >> 32))) >> 2;
	result = (signed int)(a1x->playerData_0xA4_164x->pitch_0x157_343 - (my_sign32(a1x->playerData_0xA4_164x->pitch_0x157_343) * 3)) >> 2;

	//my_sign(a1x->playerData_0xA4_164x->word_0x157_343) * 3))

	a1x->playerData_0xA4_164x->pitch_0x157_343 -= result;
	return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// D41A4: using guessed type int x_DWORD_D41A4;
// E9C3C: using guessed type int ptrMemoryBuffer_E9C3C;
// EA3E4: using guessed type int Entities_EA3E4[];
// EB398: using guessed type __int16 x_WORD_EB398;
// 1803EC: using guessed type __int16 cdDrive_1803EC;

//----- (0005EF70) --------------------------------------------------------
type_entity_0x6E8E* SetWizardHitTimer_5EF70(type_entity_0x6E8E* a1x)//23ff70
{
	type_entity_0x6E8E* resultx; // eax

	resultx = a1x;
	if (a1x > Entities_EA3E4[0] && a1x->class_0x3F_63 == 3 && !a1x->model_0x40_64)
	{
		//resultx = (uint8_t*)a1x->dword_0xA4_164;
		a1x->playerData_0xA4_164x->hitTimer_0x36_54 = 100;
	}
	return resultx;
}
// EA3E4: using guessed type int Entities_EA3E4[];

//----- (000686D0) --------------------------------------------------------
void CreditKillToWizard_686D0(type_entity_0x6E8E* a1x, type_entity_0x6E8E* a2x)//2496d0
{
	//uint8_t* result; // eax
	type_entity_0x6E8E* v3x; // ebx
	char v4; // dh

	//result = a2;
	v3x = Entities_EA3E4[a1x->id_0x1A_26];
	if (v3x > Entities_EA3E4[0] && v3x->class_0x3F_63 == 3)
	{
		v4 = v3x->model_0x40_64;
		if (!v4 || v4 == 1)
		{
			if (a2x)
			{
				if (a2x > Entities_EA3E4[0])
				{
					v3x->targetId_0x96_150 = a2x - world_D41A0.entities_0x6E8E;
				}
			}
		}
	}
	//return result;
}
// D41A0: using guessed type int x_D41A0_BYTEARRAY_0;
// EA3E4: using guessed type int Entities_EA3E4[];
