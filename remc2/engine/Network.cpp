#include "Network.h"

// Defined in EventsFunctions.cpp; declared here rather than by including that header, which
// this file does not otherwise need.
void DestroyPlayerCastle(int player);

//#define PRINT_UPDATE_CONNECTIONS

//char x_BYTE_E126D = 0; // weak
char x_BYTE_E1274 = 0; // weak
char x_BYTE_E1275 = 0; // weak
int16_t IndexInNetwork_E1276 = -1; // 0x2B2276
__int16 countConnected_E1278 = 0; // 0x2B2278
__int16 maxPlayers_E127A = 8; // 0x2B227A
uint8_t* networkBuffer_E127E = nullptr; // weak
uint8_t* paket_E1282 = nullptr; // weak
uint8_t* packetArray_E1286[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr }; // idb
uint8_t connected_E12A6 = 0; // weak
int16_t IndexInNetwork2_E12A8 = 0; // weak
myNCB* mainConnection_E12AA = nullptr; // weak //array size 66 //0x2b22aa

myNCB* connection_E12AE[8] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };//0x2B22AE
char connected_E12CE[8] = { 0, 0, 0, 0, 0, 0, 0, 0 }; // idb


char nethID[7] = "TESTER";

// True while the level is running.  A peer that disappears has to be dealt with differently
// in the lobby (the menu code notices) and in the game (nobody does).
bool g_inGameLoop = false;

// See Network.h.  Bumped when an automated match ends and another is due.
int g_autotest_match = 0;

__int16 x_WORD_E131A = 0; // weak

bool Iam_server = false;
bool Iam_client = false;
//int ClientMPort = 3491;
int NetworkPort = 15001;
int ServerPort = -1;
char serverIP[256] = "127.0.0.1";

type_str_word_26 str_WORD_E1F70 = { 430, 78, 0, 0, 80, 0, 413, 0, 0 }; // idb // for scoll dialog drawing

TColor str_BYTE_E1711[2][18] = { {//players Palette colors is halfed
{0x36,0x36,0x36},//0 //grey
{0x00,0x26,0x00},//1 //green
{0x2D,0x00,0x00},//2 //red
{0x23,0x00,0x21},//3 //violet
{0x3F,0x0F,0x28},//4 //violet2
{0x16,0x0F,0x00},//5 //brown
{0x00,0x00,0x17},//6 //blue
{0x3F,0x1F,0x00},//7 //orange
{0x00,0x00,0x00}},{//8
{0x22,0x22,0x22},//27-0 //darkgrey
{0x00,0x16,0x00},//27-1 //darkgreen
{0x1B,0x00,0x00},//27-2 //darkred
{0x14,0x00,0x16},//27-3 //darkviolet
{0x31,0x00,0x19},//27-4 //darkviolet2
{0x0E,0x0A,0x00},//27-5 //darkbrown
{0x00,0x00,0x1E},//27-6 //darkblue
{0x3C,0x1C,0x00},//27-7 //darkorange
{0x00,0x00,0x00}}//27-8
}; // weak//2b2711

// Waiting for an NCB to complete blocks the game thread outright - nothing is drawn and no
// input is read - so a step of the handshake that never completes is indistinguishable from
// a hung game.  Nothing bounds these waits (LISTEN and RECEIVE never expire, see
// port_net.cpp), which is by design: the game really does have to wait for the peer.  What
// was missing is any record of it, so a report of "both instances froze" left nothing to go
// on.  Report what is being waited for, and keep waiting exactly as before.
//
// The message goes through debug_net_printf, which writes its file whether or not
// --network_debug was given, so a freeze is recorded even on an ordinary run.
void WaitForNcb_diag(myNCB* ncb, const char* what)
{
	const long start = (long)j___clock();
	long reported = 0;
	while (ncb->ncb_cmd_cplt_49 == 0xff)
	{
		const long waited = ((long)j___clock() - start) / 100;//seconds
		if (waited >= 3 && waited - reported >= 3)
		{
			reported = waited;
			if (CommandLineParams.DoNetworkDebug())
				debug_net_printf("STUCK: %s pending %lds - cmd=0x%02X ret=0x%02X lsn=%d name=[%.16s] call=[%.16s]\n",
					what, waited, (int)ncb->ncb_command_0, (int)ncb->ncb_retcode_1, (int)ncb->ncb_lsn_2,
					ncb->ncb_name_26, ncb->ncb_callName_10);
		}
	}
	if (reported && CommandLineParams.DoNetworkDebug())
		debug_net_printf("STUCK: %s completed after %lds with 0x%02X\n",
			what, ((long)j___clock() - start) / 100, (int)ncb->ncb_cmd_cplt_49);
}

//----- (00072D04) --------------------------------------------------------
void NetworkDisallocation_72D04()
{
	if (x_BYTE_E1274)
	{
		for (int i = 0; maxPlayers_E127A > i; i++)
		{
			WaitForNcb_diag(connection_E12AE[i], "shutdown: player NCB");
		}
		WaitForNcb_diag(mainConnection_E12AA, "shutdown: main NCB");
		FreeMem_83E80((uint8_t*)mainConnection_E12AA);
		for (int j = 0; j < 8; j++)
		{
			FreeMem_83E80((uint8_t*)connection_E12AE[j]);
			FreeMem_83E80((uint8_t*)packetArray_E1286[j]);
		}
		FreeMem_83E80((uint8_t*)networkBuffer_E127E);
		FreeMem_83E80((uint8_t*)paket_E1282);
		x_BYTE_E1274 = 0;
	}
}

//----- (00072DDE) --------------------------------------------------------
uint8 NetworkTestAddName_72DDE(/*signed __int16* a1,*/ int compindex)//253dde
{//253de2
	uint8 result;
	sprintf(printbuffer, "%s%d", nethID, compindex);
	do
	{//253e06
		result = NetworkAddName_74767(connection_E12AE[compindex], printbuffer);//2557bb
		if (result == 13)//253e23
			NetworkDeleteName_74A86(connection_E12AE[compindex], printbuffer);
	} while (result == 13 && !connected_E12A6);
	return result;
}

//----- (00072E70) --------------------------------------------------------
int sub_72E70(signed __int16* a3)//253e70
{
	char connected[8];
	for (int i = 0; maxPlayers_E127A > i; i++)
	{
		if (IndexInNetwork_E1276 != i)
			NetworkCall_74809(i);
	}
	memset(connected, 0, 8);
	int result = 0;
	while (result < 2)
	{
		if (connected[IndexInNetwork_E1276] == maxPlayers_E127A - 1 || connected_E12A6)
			break;
		for (int i = 0; maxPlayers_E127A > i; i++)
		{
			if (IndexInNetwork_E1276 != i)
			{
				if (connection_E12AE[i]->ncb_cmd_cplt_49)
				{
					if (connection_E12AE[i]->ncb_cmd_cplt_49 == 0xff)
					{
						if (result)
							NetworkCancel_748F7(i);
					}
					else if (!connected[i])
					{
						connected[IndexInNetwork_E1276]++;
						connected[i] = 1;
					}
				}
				else if (++result == 1)
				{
					IndexInNetwork2_E12A8 = i;
				}
			}
		}
		WaitToConnect_7C230();
	}
	return result;
}

//----- (00072FBB) --------------------------------------------------------
signed int NetworkTestCall_72FBB()//253fbb
{
	NetworkCall_74809(0);
	for (int i = mygetthousandths(); ; WaitToConnect_7C230())
	{
		if (connection_E12AE[0]->ncb_cmd_cplt_49 != 0xff)
			return 1;//254024
		if ((mygetthousandths() - i) > 120 * 100)//original only 120
			break;//253ffd
	}
	//253fff
	NetworkCancel_748F7(0);
	return 0;
}

//----- (0007302E) --------------------------------------------------------
void NetworkListenAll_7302E()//25402e
{
	for (int i = 0; maxPlayers_E127A > i && !connected_E12A6; i++)
	{
		if (IndexInNetwork_E1276 != i)
			NetworkListen_74B75(i);
	}
}

void ResetRewireState();

//----- (0007308F) --------------------------------------------------------
int NetworkInitConnection_7308F(char* a2, __int16 a3)//25408f
{
	uint8 addNameResult;
	int i;
	int result;
	//254093
	if (!x_BYTE_E1274 || x_BYTE_E1275)
		return -1;
	if (a3)
		maxPlayers_E127A = a3;
	else
		maxPlayers_E127A = 8;
	x_BYTE_E1275 = 0;
	connected_E12A6 = 0;
	strcpy(nethID, (char*)a2);
	extern char oldConnected[8];   // defined next to RemoveDeadClients, further down
	for (i = 0; maxPlayers_E127A > i; i++)
	{
		connected_E12CE[i] = 0;
		// ...and forget what was connected in the PREVIOUS match.  RemoveDeadClients reports
		// a peer as vanished when connected_E12CE drops while oldConnected still says it was
		// there, and oldConnected was never cleared between matches.  So a second match began
		// by declaring every peer from the first one dead: it cancelled and hung up the
		// LISTENs that had just been armed (measured armed at 34988, torn down at 35820),
		// and the peer's data connection then arrived to find nothing to attach to.
		// -1 is the "nothing known yet" value the array starts life with.
		oldConnected[i] = -1;
	}
	for (i = 0; maxPlayers_E127A > i; i++)
	{
		WaitForNcb_diag(connection_E12AE[i], "join: player NCB before add name");
	}
	WaitForNcb_diag(mainConnection_E12AA, "join: main NCB before add name");//AddNameNotSet?

	// clear stale per-match network state left over from a previous
	// game before starting this one, otherwise the peers fail to reconnect.
	ResetNetworkGameState();

	// The token starts at index 0 in every match: index 0 is the side that listens, and
	// the branch below only assigns the token on the node that gets index 0.  In the
	// original game that was enough, because losing a peer ended the session for good -
	// the token could never outlive the match it was moved in.  Now that a survivor plays
	// on and starts another match, a client that took the token over when the previous
	// server disappeared carried the value into the new game and still believed it held it,
	// while the new index 0 believed the same.  Two token holders means two collectors and
	// no sender: both sat in a receive until the test called it stuck.
	IndexInNetwork2_E12A8 = 0;
	ResetRewireState();

	/*
	//wait for Server AddName
	if (!Iam_server)
	{
		char prbuffer[1024];
		snprintf(prbuffer, sizeof(prbuffer), "WAITING FOR SERVER: %s", serverIP);
		VGA_Draw_string(prbuffer);
		bool receiveServerAddName = false;
		while (!ReceiveServerAddName()) {
			mydelay(1000);
			VGA_Draw_string((char*)".");

			//added for network
			if (locNetworkClass) {
				locNetworkClass->UpdateNetworkSingleThread();
			}
			//added for network
		}
	}
	//wait for Server AddName
	*/

	i = 0;
	IndexInNetwork_E1276 = -1;
	while (maxPlayers_E127A > i && IndexInNetwork_E1276 == -1 && !connected_E12A6)
	{//2541a1
		addNameResult = NetworkTestAddName_72DDE(i);
		if (addNameResult)//2541aa
		{
			if (addNameResult == 0xff)
			{
				NetworkCancel_748F7(i);
				i = maxPlayers_E127A;
			}
		}
		else
		{
			IndexInNetwork_E1276 = i;
		}
		i++;
	}
	if (IndexInNetwork_E1276 == -1)//2541e7
		return -1;
	for (i = 0; i < maxPlayers_E127A; i++)
	{
		if (IndexInNetwork_E1276 != i)
		{
			strncpy(connection_E12AE[i]->ncb_name_26, connection_E12AE[IndexInNetwork_E1276]->ncb_name_26, sizeof(connection_E12AE[IndexInNetwork_E1276]->ncb_name_26));
			connection_E12AE[i]->ncb_num_3 = connection_E12AE[IndexInNetwork_E1276]->ncb_num_3;
		}
	}
	if (CommandLineParams.DoTestNetworkChng1()) {
		IndexInNetwork_E1276 = 1;
	}
	if (IndexInNetwork_E1276)//254278
	{
		if (!NetworkTestCall_72FBB()) // it is main connection
			connected_E12A6 = 1;
	}
	else
	{
		NetworkListenAll_7302E(); // it is second connection 
		IndexInNetwork2_E12A8 = IndexInNetwork_E1276;
	}
	if (connected_E12A6 == 1)
	{
		for (i = 0; maxPlayers_E127A > i; i++)
		{
			if (IndexInNetwork_E1276 != i)
				NetworkCancel_748F7(i);
		}
		snprintf(printbuffer, printBufferSize, "%s%d", nethID, IndexInNetwork_E1276);
		NetworkDeleteName_74A86(connection_E12AE[IndexInNetwork_E1276], printbuffer);
		result = -1;
	}
	else
	{
		NetworkUpdateConnections_74F76();
		x_BYTE_E1275 = 1;
		result = IndexInNetwork_E1276;
	}
	return result;
}

//----- (00073669) --------------------------------------------------------
void NetworkCanceling_73669(__int16 a1)//254669
{
	if (IndexInNetwork_E1276 == a1)
	{
		for (int i = 0; i < maxPlayers_E127A; i++)
		{
			if (i != IndexInNetwork_E1276)
			{
				NetworkCancel_748F7(i);
				NetworkHangUp_74B19(connection_E12AE[i]);
			}
		}
		snprintf(printbuffer, printBufferSize, "%s%d", nethID, IndexInNetwork_E1276);
		NetworkDeleteName_74A86(connection_E12AE[a1], printbuffer);
		x_BYTE_E1275 = 0;
	}
	else
	{
		NetworkCancel_748F7(a1);
		NetworkHangUp_74B19(connection_E12AE[a1]);
	}
}

// See Network.h for why this is a mask and not the lobby's player count.
int NetworkConnectedMask()
{
	int mask = 0;
	for (int i = 0; i < maxPlayers_E127A && i < 8; i++)
		if (connected_E12CE[i] == 1) mask |= (1 << i);
	if (IndexInNetwork_E1276 >= 0 && IndexInNetwork_E1276 < 8)
		mask |= (1 << IndexInNetwork_E1276);     // ourselves
	return mask;
}

// See Network.h.
bool NetworkAllRosterPeersConnected()
{
	bool present[8];
	const int marked = NetworkRosterPlayers(nethID, present);
	if (marked <= 0)
		return false;                       // the transport has no membership list yet
	const int mask = NetworkConnectedMask();
	for (int i = 0; i < 8; i++)
		if (present[i] && !(mask & (1 << i)))
			return false;                   // somebody on the list is not in a session with us
	return true;
}

// Slots whose node vanished and whose hand-over was already done locally from the roster.
// SetMultiplayerColors_7CE50 uses this to skip the orderly hand-over for them while still
// running the rest of its per-slot bookkeeping.
static bool g_vanishedHandled[8] = { false, false, false, false, false, false, false, false };

// Fill in who is still in the session before the departing node is asked for its own list.
//
// The original hand-over starts by RECEIVING that list from the node that is leaving, which
// works for an orderly departure and not at all for a node that was killed: nothing was
// sent, NetworkReceiveMessage2_7404E quietly does nothing once the peer is marked
// disconnected, and locConnected - an uninitialised stack array - was left to decide who
// takes the token.  In the lobby that handed the token back to the dead player and every
// survivor called a corpse for ever.
//
// connected_E12CE alone cannot answer this either: play is a star around the token holder,
// so a node has no session with - and no view of - the other survivors.  The transport does
// know, from the membership list the server broadcasts, so ask it and fall back to local
// knowledge only if it has nothing to say.  Every node applies the same rule to the same
// list, so they all reach the same answer without talking to each other.
static void SeedSurvivors(uint8_t locConnected[8], int16_t leaving)
{
	bool present[8];
	const int marked = NetworkRosterPlayers(nethID, present);
	for (int i = 0; i < 8; i++)
		locConnected[i] = (marked > 0) ? (uint8_t)(present[i] ? 1 : 0)
		                               : (uint8_t)(connected_E12CE[i] == 1);
	if (leaving >= 0 && leaving < 8)
		locConnected[leaving] = 0;                 // the node being handled here is gone
	locConnected[IndexInNetwork_E1276] = 1;        // we are certainly still here
	if (CommandLineParams.DoNetworkDebug())
		debug_net_printf("SEEDSURV: leaving=%d roster=%d survivors=[%d%d%d%d%d%d%d%d]\n",
			(int)leaving, marked, locConnected[0], locConnected[1], locConnected[2],
			locConnected[3], locConnected[4], locConnected[5], locConnected[6], locConnected[7]);
}

// See Network.h.  Not part of the original - the original never returns from a network match
// to the menu and back into another one within a single run.
void NetworkLeaveSession()
{
	if (!x_BYTE_E1274 || !x_BYTE_E1275) return;   // nothing allocated, or not in a session
	if (IndexInNetwork_E1276 < 0) return;
	if (CommandLineParams.DoNetworkDebug())
		debug_net_printf("MATCHEND: leaving the network session (index %d)\n", (int)IndexInNetwork_E1276);

	NetworkCanceling_73669(IndexInNetwork_E1276);  // hang up peers, drop our name, clear the flag
	if (CommandLineParams.DoNetworkDebug())
		debug_net_printf("MATCHEND: session released\n");
}

//----- (0007373D) --------------------------------------------------------
void NetworkEvent_7373D(int16_t a1)//25473d
{
	uint8_t locConnected[8]; // [esp+54h] [ebp-10h]
	int i; // [esp+5Ch] [ebp-8h]

	if (x_BYTE_E1274 && x_BYTE_E1275)
	{
		if (IndexInNetwork2_E12A8 == a1)
		{
			if (IndexInNetwork_E1276 == a1)
			{
				for (i = 0; maxPlayers_E127A > i; i++)
					locConnected[i] = connected_E12CE[i] == 1;
				for (i = 0; maxPlayers_E127A > i; i++)
				{
					if (locConnected[i] == 1)
					{
						printState2((char*)"Send State 1\n");//debug	
						NetworkSendMessage2_74006(i, locConnected, 8u);
					}
				}
				NetworkCanceling_73669(a1);
			}
			else
			{
				SeedSurvivors(locConnected, a1);
				NetworkReceiveMessage2_7404E(IndexInNetwork2_E12A8, locConnected, 8u);
				NetworkCanceling_73669(a1);
				for (i = 0; maxPlayers_E127A > i; i++)
				{
					if (locConnected[i] == 1)
					{
						IndexInNetwork2_E12A8 = i;
						locConnected[i] = 0;
						break;
					}
				}
				if (IndexInNetwork2_E12A8 == IndexInNetwork_E1276)
				{
					for (i = 0; ; i++)
					{
						if (maxPlayers_E127A <= i)
							break;
						if (IndexInNetwork_E1276 != i && locConnected[i] == 1)
						{
							NetworkListen_74B75(i);
							WaitForNcb_diag(connection_E12AE[i], "player left: LISTEN for the next peer");
							if (connection_E12AE[i]->ncb_cmd_cplt_49)
								sprintf(printbuffer, "Error code (LISTEN) : %d", connection_E12AE[i]->ncb_cmd_cplt_49);
						}
					}
				}
				else
				{
					while (1)
					{
						NetworkCall_74809(IndexInNetwork2_E12A8);
						WaitForNcb_diag(connection_E12AE[IndexInNetwork2_E12A8], "player left: CALL to the new token holder");
						if (!connection_E12AE[IndexInNetwork2_E12A8]->ncb_cmd_cplt_49)
							break;
						sprintf(printbuffer, "Error code (CALL) : %d", connection_E12AE[IndexInNetwork2_E12A8]->ncb_cmd_cplt_49);
					}
				}
			}
		}
		else
		{
			NetworkCanceling_73669(a1);
		}
	}
}

//----- (000739AD) --------------------------------------------------------
void NetworkRemoveClient_739AD(__int16 a1)//2549ad
{
	if (IndexInNetwork_E1276 == a1)
	{
		for (int i = 0; i < maxPlayers_E127A; i++)
		{
			if (i != IndexInNetwork_E1276)
			{
				NetworkCancel_748F7(i);
				NetworkHangUp_74B19(connection_E12AE[i]);
			}
		}
		snprintf(printbuffer, printBufferSize, "%s%d", nethID, IndexInNetwork_E1276);
		NetworkDeleteName_74A86(connection_E12AE[a1], printbuffer);
		x_BYTE_E1275 = 0;
	}
	else
	{
		NetworkCancel_748F7(a1);
		NetworkHangUp_74B19(connection_E12AE[a1]);
		if (IndexInNetwork_E1276 == IndexInNetwork2_E12A8)
			NetworkListen_74B75(a1);
	}
}

//----- (00073AA1) --------------------------------------------------------
void NetworkSomeChange_73AA1(__int16 a1)//254aa1
{
	uint8_t locConnected[8]; // [esp+54h] [ebp-10h]
	int i; // [esp+5Ch] [ebp-8h]

	if (x_BYTE_E1274 && x_BYTE_E1275)
	{
		if (IndexInNetwork2_E12A8 == a1)
		{
			if (IndexInNetwork_E1276 == a1)
			{
				for (i = 0; maxPlayers_E127A > i; i++)
					locConnected[i] = connected_E12CE[i] == 1;
				for (i = 0; maxPlayers_E127A > i; i++)
				{
					if (locConnected[i] == 1)
					{
						printState2((char*)"Send State 2\n");//debug	
						NetworkSendMessage2_74006(i, locConnected, 8);
					}
				}
				NetworkRemoveClient_739AD(a1);
			}
			else
			{
				SeedSurvivors(locConnected, a1);
				NetworkReceiveMessage2_7404E(IndexInNetwork2_E12A8, locConnected, 8);
				NetworkRemoveClient_739AD(a1);
				for (i = 0; maxPlayers_E127A > i; i++)
				{
					if (locConnected[i] == 1)
					{
						IndexInNetwork2_E12A8 = i;
						locConnected[i] = 0;
						break;
					}
				}
				if (IndexInNetwork2_E12A8 == IndexInNetwork_E1276)
				{
					for (i = 0; ; i++)
					{
						if (maxPlayers_E127A <= i)
							break;
						if (IndexInNetwork_E1276 != i && locConnected[i] == 1)
						{
							NetworkListen_74B75(i);
							WaitForNcb_diag(connection_E12AE[i], "peer change: LISTEN for the next peer");
							if (connection_E12AE[i]->ncb_cmd_cplt_49)
								sprintf(printbuffer, "Error code (LISTEN) : %d", connection_E12AE[i]->ncb_cmd_cplt_49);
						}
					}
				}
				else
				{
					while (1)
					{
						NetworkCall_74809(IndexInNetwork2_E12A8);
						WaitForNcb_diag(connection_E12AE[IndexInNetwork2_E12A8], "peer change: CALL to the new token holder");
						if (!connection_E12AE[IndexInNetwork2_E12A8]->ncb_cmd_cplt_49)
							break;
						snprintf(printbuffer, printBufferSize, "Error code (CALL) : %d", connection_E12AE[IndexInNetwork2_E12A8]->ncb_cmd_cplt_49);
					}
				}
			}
		}
		else
		{
			NetworkRemoveClient_739AD(a1);
		}
	}
}

//----- (00073D11) --------------------------------------------------------
void NetworkEnd_73D11(__int16 a1)//254d11
{
	if (IndexInNetwork_E1276 == a1)
	{
		for (int i = 0; i < maxPlayers_E127A; i++)
		{
			if (i != IndexInNetwork_E1276)
			{
				NetworkCancel_748F7(i);
				NetworkHangUp_74B19(connection_E12AE[i]);
			}
		}
		snprintf(printbuffer, printBufferSize, "%s%d", nethID, IndexInNetwork_E1276);
		NetworkDeleteName_74A86(connection_E12AE[a1], printbuffer);
		x_BYTE_E1275 = 0;
	}
	else
	{
		NetworkCancel_748F7(a1);
		NetworkHangUp_74B19(connection_E12AE[a1]);
		if (IndexInNetwork_E1276 == IndexInNetwork2_E12A8)
			NetworkListen_74B75(a1);
	}
}

//----- (00074006) --------------------------------------------------------
void NetworkSendMessage2_74006(unsigned __int16 a1, uint8_t* buffer, unsigned int size)//255006
{
	if (x_BYTE_E1274)
	{
		// Who is actually written to, and who is skipped for being "not connected".  The
		// receiving end already logs what arrives (queued DIRECT_SEND), so the two together
		// say whether a turn was never sent, or sent and never picked up.
		if (CommandLineParams.DoNetworkDebug() && a1 < 8)
		{
			static int lastSentTo[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
			const int connectedNow = (connected_E12CE[a1] == 1) ? 1 : 0;
			if (lastSentTo[a1] != connectedNow)
			{
				lastSentTo[a1] = connectedNow;
				debug_net_printf("SENDTO: slot %d %s (size=%u lsn=%d name=[%.16s] call=[%.16s])\n",
					(int)a1, connectedNow ? "is written to" : "is SKIPPED - not connected",
					size, (int)connection_E12AE[a1]->ncb_lsn_2,
					connection_E12AE[a1]->ncb_name_26, connection_E12AE[a1]->ncb_callName_10);
			}
		}
		if (connected_E12CE[a1] == 1)
			NetworkSendMessage_74EF1(connection_E12AE[a1], buffer, size);
	}
}

//----- (0007404E) --------------------------------------------------------
void NetworkReceiveMessage2_7404E(unsigned __int16 connectionindex, uint8_t* buffer, unsigned int size)//25504e
{
	if (x_BYTE_E1274)
	{
		if (connected_E12CE[connectionindex] == 1)
			NetworkReceiveMessage_74D41(connection_E12AE[connectionindex], buffer, size);
	}
}

//----- (00074374) --------------------------------------------------------
void NetworkUpdateConnections2_74374()//255374
{
	NetworkUpdateConnections_74F76();
}

// Holds this node's own entry of the exchanged array across the send/receive round trip.
// The largest element exchanged is type_str_0x2BDE (2124 bytes).
uint8_t ownSliceBuffer[4096];

//----- (0007438A) --------------------------------------------------------
void ReceiveSendAll_7438A(uint8_t* buffer, unsigned int size)//25538a
{
	RemoveDeadClients();
	// Re-aim the connections if the server role has moved, from HERE.
	//
	// This is where a node in the lobby actually spins: after a hand-over both survivors were
	// found here and NOT in the menu loop, so a check placed there was never reached - measured,
	// the follower stopped reporting for seventy seconds while this exchange kept running.
	//
	// Only in the lobby (!g_inGameLoop) and only while the session is up (x_BYTE_E1275): in a
	// level every pair already holds a session and there is nothing to re-aim, and doing this
	// while a match is being torn down cancels NCBs that are on their way out, which killed an
	// instance outright when this lived in the per-tick update.
	if (!g_inGameLoop && x_BYTE_E1275)
		NetworkRewireAfterServerChange();

	if (x_BYTE_E1274)
	{
		if (IndexInNetwork_E1276 == IndexInNetwork2_E12A8)
		{
			timeState(false, "End");//debug	
			timeState(true, "Begin - pre Send");//debug	
			for (int i = 0; i < countConnected_E1278; i++)
			{
				if (i != IndexInNetwork_E1276)
					NetworkReceiveMessage2_7404E(i, &buffer[size * i], size);
			}
			timeState(true, "After Send, Before Receive");//debug
			for (int j = 0; j < countConnected_E1278; j++)
			{
				if (j != IndexInNetwork_E1276)
				{
					printState2((char*)"Send State 3\n");//debug
					NetworkSendMessage2_74006(j, buffer, size * countConnected_E1278);
				}
			}
			timeState(true, "After Receive");//debug
		}
		else
		{
			timeState(false, "End");//debug
			timeState(true, "Begin - pre Send");//debug
			printState2((char*)"Send State 4\n");//debug
			// Keep our own slice: the echo can still carry the previous turn's copy of it,
			// and rolling our own entry back would drop whatever the local player just did.
			uint8_t* mySlot = &buffer[size * IndexInNetwork_E1276];
			if (size <= sizeof(ownSliceBuffer))
				memcpy(ownSliceBuffer, mySlot, size);
			NetworkSendMessage2_74006(IndexInNetwork2_E12A8, mySlot, size);
			timeState(true, "After Send, Before Receive");//debug
			NetworkReceiveMessage2_7404E(IndexInNetwork2_E12A8, buffer, size * countConnected_E1278);
			// The echoed array is authoritative for the OTHER players - it must be taken as
			// received, otherwise this node loses track of where they are.  Only our own
			// entry is restored, so a locally made choice (menu selection, marking mana,
			// re-casting a castle) is not erased before it has been sent on.
			//
			// Restoring the WHOLE array here - as the code used to do whenever the echo did
			// not match - was what broke the sync: once the echo ran one turn behind, the
			// comparison failed on every following turn, so this node permanently discarded
			// the authoritative data and ran on its own copy, in which the other players'
			// slots are never filled in.  It never recovered from that.
			if (size <= sizeof(ownSliceBuffer))
				memcpy(mySlot, ownSliceBuffer, size);
			timeState(true, "After Receive");//debug
		}
	}
}

//----- (0007449C) --------------------------------------------------------
void NetworkCancelAll_7449C()//25549c
{
	if (x_BYTE_E1274)
	{
		if (IndexInNetwork_E1276 == IndexInNetwork2_E12A8)
		{
			for (int i = 0; i < maxPlayers_E127A; i++)
			{
				if (IndexInNetwork_E1276 != i && connection_E12AE[i]->ncb_cmd_cplt_49 == 0xff)
					NetworkCancel_748F7(i);
			}
		}
	}
}

//----- (00074515) --------------------------------------------------------
int16_t GetIndexNetwork2_74515()//255515
{
	return IndexInNetwork2_E12A8;
}

//----- (00074536) --------------------------------------------------------
int16_t GetIndexNetwork_74536()//255536
{
	return IndexInNetwork_E1276;
}

const int maxSizeOfPacket = 2048 * 30;//original 2048

//----- (00074556) --------------------------------------------------------
uint8_t NetworkAllocation_74556()//255556 push ebp 355250
{
	bool allocation_ok = true;
	if (!x_BYTE_E1274 && !mainConnection_E12AA)
	{
		mainConnection_E12AA = (myNCB*)Malloc_83D70(sizeof(myNCB));
		memset(mainConnection_E12AA, 0, sizeof(myNCB));
		if (mainConnection_E12AA)
		{
			if (NetworkInit_74A11() == -1)//255a11
				return 0;
			networkBuffer_E127E = (uint8_t*)Malloc_83D70(maxSizeOfPacket);
			memset(networkBuffer_E127E, 0, maxSizeOfPacket);
			if (networkBuffer_E127E)
			{
				paket_E1282 = (uint8_t*)Malloc_83D70(maxSizeOfPacket);
				memset(paket_E1282, 0, maxSizeOfPacket);
				if (paket_E1282)
				{
					int i;
					for (i = 0; i < 8; i++)
					{
						packetArray_E1286[i] = (uint8_t*)Malloc_83D70(maxSizeOfPacket);
						memset(packetArray_E1286[i], 0, maxSizeOfPacket);
						if (!packetArray_E1286[i])
						{
							allocation_ok = false;
							break;
						}
						connection_E12AE[i] = (myNCB*)Malloc_83D70(sizeof(myNCB));
						memset(connection_E12AE[i], 0, sizeof(myNCB));
						if (!connection_E12AE[i])
						{
							allocation_ok = false;
							break;
						}
					}
					if (i == 8)
						x_BYTE_E1274 = 1;
				}
				else
				{
					allocation_ok = false;
				}
			}
			else
			{
				allocation_ok = false;
			}
		}
		else
		{
			allocation_ok = false;
		}
	}
	if (!allocation_ok)
	{
		x_BYTE_E1274 = 0;
		if (mainConnection_E12AA)
			FreeMem_83E80((uint8_t*)mainConnection_E12AA);
		if (networkBuffer_E127E)
			FreeMem_83E80(networkBuffer_E127E);
		if (paket_E1282)
			FreeMem_83E80(paket_E1282);
		for (int j = 0; j < 8; j++)
		{
			if (connection_E12AE[j])
				FreeMem_83E80((uint8_t*)connection_E12AE[j]);
			if (packetArray_E1286[j])
				FreeMem_83E80(packetArray_E1286[j]);
		}
	}
	return x_BYTE_E1274;
}

//----- (00074767) --------------------------------------------------------
uint8_t NetworkAddName_74767(/*signed __int16* a1,*/ myNCB* connection, char* name)//255767
{
	strcpy(connection->ncb_name_26, name);
	while (strlen(connection->ncb_name_26) < 0xFu)
		strcat(connection->ncb_name_26, " ");
	connection->ncb_command_0 = 0xb0;//ADD_NAME
	if (setNetbios_75044(connection) == 0xff)
		return 157;
	// Unlike the other waits this one keeps drawing, so it is the "waiting for connection"
	// screen rather than a freeze.  Note it in the log every half minute, so the timeline
	// shows how long a session sat here before a peer turned up.
	const long addNameStart = (long)j___clock();
	long addNameReported = 0;
	while (connection->ncb_cmd_cplt_49 == 0xff && !connected_E12A6)
	{
		const long waited = ((long)j___clock() - addNameStart) / 100;//seconds
		if (waited - addNameReported >= 30)
		{
			addNameReported = waited;
			if (CommandLineParams.DoNetworkDebug())
				debug_net_printf("WAITING: add name [%.16s] not answered for %lds\n", connection->ncb_name_26, waited);
		}
		WaitToConnect_7C230();//25d36d
	}
	return connection->ncb_cmd_cplt_49;
}

//----- (00074809) --------------------------------------------------------
void NetworkCall_74809(__int16 clientIndex)//255809
{
	connection_E12AE[clientIndex]->ncb_command_0 = 0x90;//CALL 
	sprintf(connection_E12AE[clientIndex]->ncb_callName_10, "%s%d", nethID, clientIndex);
	while (strlen(connection_E12AE[clientIndex]->ncb_callName_10) < 0xFu)
		strcat(connection_E12AE[clientIndex]->ncb_callName_10, " ");
	connection_E12AE[clientIndex]->ncb_rto_42 = 0;
	connection_E12AE[clientIndex]->ncb_sto_43 = 0;
	setNetbios_75044(connection_E12AE[clientIndex]);
}

//----- (000748F7) --------------------------------------------------------
signed int NetworkCancel_748F7(__int16 compindex)//2558f7
{
	if (connection_E12AE[compindex]->ncb_cmd_cplt_49 != 0xff)
		return -mainConnection_E12AA->ncb_cmd_cplt_49;
	mainConnection_E12AA->ncb_command_0 = 0x35;//CANCEL 
	mainConnection_E12AA->ncb_buffer_4.p = (uint8_t*)connection_E12AE[compindex];
	if (setNetbios_75044(mainConnection_E12AA) != 0xff)
	{
		do
		{
			WaitForNcb_diag(mainConnection_E12AA, "cancel: main NCB");
			// The original re-checks the cancelled NCB here and spins on the main one again
			// if it is still pending, which is the same wait - only without anything to see.
			WaitForNcb_diag(connection_E12AE[compindex], "cancel: the NCB being cancelled");
		} while (connection_E12AE[compindex]->ncb_cmd_cplt_49 == 0xff);
		return -mainConnection_E12AA->ncb_cmd_cplt_49;
	}
	return -99;
}

int dos_getvect(int vector) {
	if (CommandLineParams.ModeNetwork()) {
		if ((Iam_server) || (Iam_client))
		{
			//get ah from 2b5cb2 - 01
			//335c 
			//int 21
			//get ebx d49
			if (vector == 92)return 0xd49;
		}
	}
	return 0;
}

//----- (00074A11) --------------------------------------------------------
signed int NetworkInit_74A11()//255a11 // netbios
{
	if (dos_getvect(92))
	{
		mainConnection_E12AA->ncb_command_0 = 0x7F;//?
		if (setNetbios_75044(mainConnection_E12AA) == -1)//push ebx:1a6f44,push esi:1a7358,push edi:1a7358,push ebp:355234
			return -1;
		if (mainConnection_E12AA->ncb_retcode_1 == 3)
			return 0;
	}
	return -1;
}

//----- (00074A86) --------------------------------------------------------
void NetworkDeleteName_74A86(myNCB* a1x, char* a2)//255a86
{
	strcpy(a1x->ncb_name_26, a2);
	while (strlen(a1x->ncb_name_26) < 0xFu)
		strcat(a1x->ncb_name_26, " ");
	a1x->ncb_command_0 = 0xb1;//DELETE_NAME
	if (setNetbios_75044(a1x) == -1)
		return;
	WaitForNcb_diag(a1x, "delete name");
}

//----- (00074B19) --------------------------------------------------------
void NetworkHangUp_74B19(myNCB* a1x)//255b19
{
	a1x->ncb_command_0 = 0x92;//HANG_UP
	if (setNetbios_75044(a1x) == -1)
		return;
	WaitForNcb_diag(a1x, "hang up");
	a1x->ncb_lsn_2 = 0;
}

//----- (00074B75) --------------------------------------------------------
signed int NetworkListen_74B75(__int16 a1)//255b75
{
	if (connection_E12AE[a1]->ncb_cmd_cplt_49 == 0xff)
		return -connection_E12AE[a1]->ncb_cmd_cplt_49;
	sprintf(printbuffer, "%s%d", nethID, a1);
	connection_E12AE[a1]->ncb_command_0 = 0x91;//LISTEN 
	strcpy(connection_E12AE[a1]->ncb_callName_10, printbuffer);
	while (strlen(connection_E12AE[a1]->ncb_callName_10) < 0xFu)
		strcat(connection_E12AE[a1]->ncb_callName_10, " ");
	connection_E12AE[a1]->ncb_rto_42 = 0;
	connection_E12AE[a1]->ncb_sto_43 = 0;
	if (setNetbios_75044(connection_E12AE[a1]) != -1)
	{
		connection_E12AE[a1]->ncb_buffer_4.p = NULL;
		return -connection_E12AE[a1]->ncb_cmd_cplt_49;
	}
	return -99;
}

//----- (00074C9D) --------------------------------------------------------
int NetworkReceivePacket_74C9D(myNCB* connection, uint8_t* buffer, int size)//255c9d
{
	connection->ncb_command_0 = 0x95;//RECEIVE
	connection->ncb_buffer_4.p = paket_E1282;
	connection->ncb_bufferLength_8 = size;
	if (setNetbios_75044(connection) == -1)
		return -99;
	// This is where a node ends up when its peer stops sending while the socket stays up:
	// RECEIVE never expires, so the turn never finishes.
	WaitForNcb_diag(connection, "receive packet");
	if (connection->ncb_cmd_cplt_49)
		return -connection->ncb_cmd_cplt_49;
	memcpy((void*)buffer, (void*)paket_E1282, size);
	return connection->ncb_bufferLength_8;
}

//----- (00074D41) --------------------------------------------------------
void NetworkReceiveMessage_74D41(myNCB* connection, uint8_t* inbuffer, unsigned int size)//255d41
{
	unsigned int packedReceived;
	uint8_t* buffer;

	buffer = inbuffer;
	packedReceived = 0;
	while (size > maxSizeOfPacket * (packedReceived + 1))
	{
		if (NetworkReceivePacket_74C9D(connection, buffer, maxSizeOfPacket) != maxSizeOfPacket)
			return;
		packedReceived++;
		buffer += maxSizeOfPacket;
	}
	NetworkReceivePacket_74C9D(connection, buffer, size - maxSizeOfPacket * packedReceived);
}

/*
//----- (00074DD4) --------------------------------------------------------
signed int NetworkReceivePacket_74DD4(myNCB* connection, unsigned __int16 bufferindex)//255dd4
{
	signed int v4; // [esp+0h] [ebp-4h]

	connection->ncb_command_0 = 0x95;//RECEIVE

	connection->ncb_buffer_4 = packetArray_E1286[bufferindex];

	connection->ncb_bufferLength_8 = 2048;
	if (setNetbios_75044(connection) == -1)
		v4 = -99;
	else
		v4 = 1;
	return v4;
}
*/

//----- (00074E6D) --------------------------------------------------------
int NetworkSendPacket_74E6D(myNCB* connection, uint8_t* buffer, int size)//255e6d
{
	memcpy((void*)networkBuffer_E127E, buffer, size);//max 2048
	connection->ncb_command_0 = 0x94;//SEND 

	connection->ncb_buffer_4.p = networkBuffer_E127E;

	connection->ncb_bufferLength_8 = size;
	if (setNetbios_75044(connection) == -1)
		return -99;
	WaitForNcb_diag(connection, "send packet");
	return -connection->ncb_cmd_cplt_49;
}

//----- (00074EF1) --------------------------------------------------------
void NetworkSendMessage_74EF1(myNCB* connection, uint8_t* inbuffer, unsigned int size)//255ef1
{
	unsigned int packedSended = 0;
	uint8_t* buffer = inbuffer;
	while (size > maxSizeOfPacket * (packedSended + 1))
	{
		if (NetworkSendPacket_74E6D(connection, buffer, maxSizeOfPacket) != 0)
			return;
		packedSended++;
		buffer += maxSizeOfPacket;
	}
	NetworkSendPacket_74E6D(connection, buffer, size - maxSizeOfPacket * packedSended);
}

char oldConnected[8] = { -1, -1, -1, -1, -1, -1, -1, -1 }; // idb
// A peer vanished without warning - killed process, lost machine - rather than leaving in
// an orderly way.  NetworkSomeChange_73AA1 cannot be used for this: it starts by receiving
// the survivor list from the node that is going away, and nobody sent one.  Everything is
// therefore worked out locally, and because every node applies the same rule to the same
// facts, they all reach the same answer without talking to each other.
//
// The original does not do any of this.  Its exchange (sub_7438A) neither checks return
// codes nor reacts to them: on a LAN of the day, a machine that died ended the session.
static void NetworkPeerVanished_73AA1b(int16_t lost)
{
	if (!x_BYTE_E1274 || !x_BYTE_E1275)
		return;

	NetworkCanceling_73669(lost);
	D41A0_0.array_0x2BDE[lost].byte_0x006_2BE4_11236 = 0;   // no longer in the game
	if (CommandLineParams.DoNetworkDebug())
		debug_net_printf("PEERGONE: player %d vanished (token holder was %d)\n",
			(int)lost, (int)IndexInNetwork2_E12A8);

	if (IndexInNetwork2_E12A8 != lost)
		return;                                            // the token did not die with it

	// Who is left.  Taken from the roster rather than the in-game flags, so this works in
	// the lobby too - there the flags are not filled in yet, and a node cannot see the
	// players it holds no session with anyway.
	uint8_t survivors[8];
	SeedSurvivors(survivors, lost);

	// The token passes to the lowest-numbered player still in the game - the same rule the
	// control server uses, and computable by everyone from what they already know.
	int16_t next = -1;
	for (int i = 0; i < maxPlayers_E127A; i++)
	{
		if (i == lost) continue;
		if (survivors[i])
		{
			next = (int16_t)i;
			break;
		}
	}
	if (next < 0)
		return;

	IndexInNetwork2_E12A8 = next;
	if (CommandLineParams.DoNetworkDebug())
		debug_net_printf("PEERGONE: token moves to %d (I am %d)\n", (int)next, (int)IndexInNetwork_E1276);

	// The survivors only ever had a session with the node that died, so they have to be
	// introduced to each other again: the new token holder listens, everybody else calls it.
	if (IndexInNetwork2_E12A8 == IndexInNetwork_E1276)
	{
		for (int i = 0; i < maxPlayers_E127A; i++)
		{
			if (i == IndexInNetwork_E1276 || i == lost) continue;
			if (!survivors[i]) continue;
			NetworkListen_74B75(i);
		}
	}
	else
	{
		NetworkCall_74809(IndexInNetwork2_E12A8);
	}
}

// The server role has moved: point everybody's connections at whoever holds it now.
//
// The game's topology is a hub and spokes with the hub pinned to slot 0: NetworkInitConnection
// _7308F has index 0 listen on every other slot and everybody else call index 0, and it runs
// once, when the session is joined.  When the host goes away in the LOBBY that arrangement is
// left pointing at a node that no longer exists - measured: the survivor kept a CALL to
// [NETH200] pending for the rest of the run, the transport happily resolved that name to the
// dead host's port, and the node that had taken the role over sat listening to nobody, so
// neither of them ever started the level.  In a level it does not show, because by then every
// pair already holds a session.
//
// So the same two primitives are used again with the roles as they are now: the new server
// listens on every other slot, and everybody else calls the slot the new server occupies.
// The slot is worked out from the transport's membership list, whose lowest surviving entry is
// exactly the node the hand-over picks.
// Remembered between calls, and cleared for every new match by NetworkInitConnection_7308F:
// a slot remembered from the previous game would make the first look at a freshly built
// set of connections count as a hand-over, and pull them down again.
static int  g_rewireLastServerSlot = -1;
static long g_rewireLastAttempt = 0;
void ResetRewireState() { g_rewireLastServerSlot = -1; g_rewireLastAttempt = 0; }

void NetworkRewireAfterServerChange()
{
	bool present[8];
	const int marked = NetworkRosterPlayers(nethID, present);
	if (marked <= 0)
		return;                       // nothing to go on yet

	int serverSlot = -1;
	for (int i = 0; i < 8; i++)
		if (present[i]) { serverSlot = i; break; }
	if (serverSlot < 0)
		return;

	// Act when the role moves, and then keep trying while this node is still not connected
	// to whoever holds it.  One attempt is not enough: the two sides re-aim independently, and
	// measured, the follower called 1.7 s before the new server had armed its LISTEN, so its
	// connection was parked and timed out with nobody to adopt it - after which nothing tried
	// again and both sat in the lobby.  Retrying stops as soon as the session is up, so this
	// costs nothing once the two have found each other.
	int&  lastServerSlot = g_rewireLastServerSlot;
	long& lastAttempt    = g_rewireLastAttempt;
	// The FIRST observation is not a change.  NetworkInitConnection_7308F has just set the
	// connections up; cancelling and re-issuing them here because this function had not seen a
	// server slot before tore down the handshake that was still in progress, and the clients
	// never reached the level at all.
	if (lastServerSlot < 0)
	{
		lastServerSlot = serverSlot;
		lastAttempt = (long)j___clock();
		return;
	}
	const bool  moved = (serverSlot != lastServerSlot);
	const bool  connectedToServer = (serverSlot == IndexInNetwork_E1276)
		|| (connected_E12CE[serverSlot] == 1);
	if (!moved)
	{
		if (connectedToServer) return;
		if (((long)j___clock() - lastAttempt) < 100) return;   // j___clock() ticks are 1/100 s
	}
	lastServerSlot = serverSlot;
	lastAttempt = (long)j___clock();

	if (CommandLineParams.DoNetworkDebug())
		debug_net_printf("REWIRE: server is now slot %d, I am %d (%s)\n",
			serverSlot, (int)IndexInNetwork_E1276,
			IndexInNetwork_E1276 == serverSlot ? "listening for the rest" : "calling it");

	// First put down the slots of nodes the membership list no longer has.  A slot left over
	// from a node that has gone still carries a LISTEN or a CALL, and a connection meant for
	// somebody else gets adopted into it - measured after a hand-over: the new server accepted
	// the survivor into the dead host's slot and went on writing to "slot 0 ... call=[NETH200]",
	// so the two of them held a session that neither of their player slots knew about and the
	// lobby never counted a second player.
	for (int i = 0; i < maxPlayers_E127A; i++)
	{
		if (i == IndexInNetwork_E1276) continue;
		if (present[i]) continue;                    // still with us
		if (connected_E12CE[i] != 1 && connection_E12AE[i]->ncb_cmd_cplt_49 == 0) continue;
		if (CommandLineParams.DoNetworkDebug())
			debug_net_printf("REWIRE: slot %d is nobody now, putting it down\n", i);
		NetworkCancel_748F7(i);
		NetworkHangUp_74B19(connection_E12AE[i]);
		connected_E12CE[i] = 0;
	}

	// The token goes with the role.  In the lobby everybody sends their slice to the token
	// holder and it hands the assembled array back, so a token left on a node that has gone
	// means every record is addressed to nobody: measured after a hand-over, both survivors
	// still reported "token=0" and their only writes were to "slot 0 ... call=[NETH200]", so
	// neither ever saw the other and the lobby counted one player for ever.
	//
	// NetworkPeerVanished_73AA1b moves it when a peer is noticed dying in a running session;
	// this path is the one where nobody notices, because the slots were already down between
	// matches when the host left.
	if (IndexInNetwork2_E12A8 != serverSlot)
	{
		if (CommandLineParams.DoNetworkDebug())
			debug_net_printf("REWIRE: token moves from %d to %d\n",
				(int)IndexInNetwork2_E12A8, serverSlot);
		// Carry the table the token holder keeps.  Colours live in the token holder's record
		// (FindFreeColorIndex_7D230 reads array_BYTE_17DE68x[token].playerIndex_1[]), and so does
		// the chosen level.  Moving the token to a record that has never held them leaves every
		// slot reading colour 0, and "the first free colour" then hands the same one to everybody
		// - which is what two wizards in one colour looks like.
		// ...but only from a record that still holds something.  The slot the token is leaving
		// belongs to a node that has gone, and its record may already have been cleared - copying
		// that gives every slot colour 0 (so everybody ends up the same colour) and level 0, which
		// is not even a multiplayer level.  The lobby sets a sensible level of its own on entry,
		// so when there is nothing to carry, leaving it alone is right.
		const int oldToken = IndexInNetwork2_E12A8;
		if (oldToken >= 0 && oldToken < 8 && serverSlot < 8
			&& x_DWORD_17DE38str.array_BYTE_17DE68x[oldToken].makeUpdate_0)
		{
			for (int c = 0; c < 8; c++)
				x_DWORD_17DE38str.array_BYTE_17DE68x[serverSlot].playerIndex_1[c] =
					x_DWORD_17DE38str.array_BYTE_17DE68x[oldToken].playerIndex_1[c];
			if (x_DWORD_17DE38str.array_BYTE_17DE68x[oldToken].selectedLevel_10 >= 50)
				x_DWORD_17DE38str.array_BYTE_17DE68x[serverSlot].selectedLevel_10 =
					x_DWORD_17DE38str.array_BYTE_17DE68x[oldToken].selectedLevel_10;
		}
		IndexInNetwork2_E12A8 = (int16_t)serverSlot;
	}

	if (IndexInNetwork_E1276 == serverSlot)
	{
		for (int i = 0; i < maxPlayers_E127A; i++)
		{
			if (i == IndexInNetwork_E1276) continue;
			if (connected_E12CE[i] == 1) continue;      // already talking to that one
			NetworkCancel_748F7(i);                     // drop whatever was aimed at the old host
			NetworkListen_74B75(i);
		}
	}
	else
	{
		if (connected_E12CE[serverSlot] != 1)
		{
			NetworkCancel_748F7(serverSlot);
			NetworkCall_74809(serverSlot);
		}
	}
}

void RemoveDeadClients()
{
	for (int i = 0; i < maxPlayers_E127A; i++)
	{
		if (connected_E12CE[i] != oldConnected[i])
		{
			if ((connected_E12CE[i] == 0) && (oldConnected[i] != -1))
			{
				NetworkCancel_748F7(i);
				NetworkHangUp_74B19(connection_E12AE[i]);
				// The flags stay exactly as the original set them: action_9 = 1 is what makes
				// SetMultiplayerColors_7CE50 blank the palette entry AND clear makeUpdate_0
				// again.  Leaving the previous action in place instead means the slot keeps
				// makeUpdate_0 set for ever - cases 2/3/4 never clear it - and re-runs that
				// stale action every frame, which for a "select character" action re-rolls
				// colours through FindFreeColorIndex_7D230 and lands two wizards on one
				// colour.
				x_DWORD_17DE38str.array_BYTE_17DE68x[i].makeUpdate_0 = 1;
				x_DWORD_17DE38str.array_BYTE_17DE68x[i].action_9 = 1;
				// What must NOT run for this slot is the ORDERLY hand-over that action_9 = 1
				// normally triggers: NetworkSomeChange_73AA1 begins by receiving the survivor
				// list from the node that is leaving, and a node that was killed sent none.
				// Mark the slot so that dispatch is skipped, and do the hand-over here from
				// the roster instead - which is right in the lobby for exactly the same
				// reason it is right in the game.
				g_vanishedHandled[i] = true;
				NetworkPeerVanished_73AA1b((int16_t)i);
				// A node that vanished left its castle behind just as surely as one that walked out
				// of the menu - a dropped connection is the same thing to everyone still playing.
				// Every survivor notices the same peer, so they all do this for the same player and
				// the castle goes on every screen without anything being sent.
				DestroyPlayerCastle(i);

				// ...and leave the slot able to take somebody new.  The original only tore it down:
				// the LISTEN armed for the peer is cancelled and hung up just above, and nothing
				// ever arms another one, so a newcomer that takes the freed name and the freed index
				// calls into a slot nobody is listening on.  Measured before this: the newcomer
				// reported "myIdx=1" and both sides exchanged nothing for the rest of the run.
				//
				// Only the listening node re-arms.  A joining node always calls the first one, and
				// index 0 is the side that listens (NetworkInitConnection_7308F: index 0 listens on
				// every other slot, everybody else calls).
				// Only while the session is still up (x_BYTE_E1275).  Ending a match takes the peers
				// away as well, and arming a LISTEN on a session that is being torn down is pointless
				// at best.  (It was suspected of breaking the next match too - it was not; that was
				// the scores screen waiting for a keypress.  The guard is kept because it is right,
				// not because it fixed that.)
				if (x_BYTE_E1275 && IndexInNetwork_E1276 == 0 && i != IndexInNetwork_E1276)
				{
					if (CommandLineParams.DoNetworkDebug())
						debug_net_printf("SLOTFREE: listening again on slot %d for a newcomer\n", i);
					NetworkListen_74B75(i);
				}
			}
			oldConnected[i] = connected_E12CE[i];
		}
	}
}


//----- (00074F76) --------------------------------------------------------
void NetworkUpdateConnections_74F76()//255f76
{
	for (int i = 0; i < maxPlayers_E127A; i++)
	{
		connected_E12CE[i] = NetworkGetState_74FE1(i);
		if (connected_E12CE[i])
			countConnected_E1278 = i + 1;
	}
	countConnected_E1278 = maxPlayers_E127A;
}

//----- (00074FE1) --------------------------------------------------------
signed int NetworkGetState_74FE1(__int16 a1)//255fe1
{
	// A message longer than the buffer the RECEIVE offered comes back truncated, with this in
	// the completion byte (NRC_BUFLEN in port_net.cpp).  It says nothing about the link: the
	// session is up, the peer is talking, only that one message did not fit.  The test below
	// reads every non-zero completion code as "no longer connected" though, and
	// RemoveDeadClients() answers that by hanging the session up and declaring the peer gone -
	// so a single oversized message ended a perfectly healthy session.
	//
	// It only bit when nothing followed quickly.  Any later command completing with
	// NRC_GOODRET puts the slot back, which is why in the same run the host shrugged its
	// BUFLEN off - a SEND came right behind it - while the third player, with nothing to send,
	// kept 0x06 in the byte, was pronounced dead, and left both survivors talking to nobody.
	const uint8_t NCB_CPLT_BUFLEN = 0x06;

	signed int v2;
	if (a1 == IndexInNetwork_E1276)
		v2 = 2;
	else
	{
		const uint8_t cplt = connection_E12AE[a1]->ncb_cmd_cplt_49;
		v2 = connection_E12AE[a1]->ncb_lsn_2 && (cplt == 0 || cplt == NCB_CPLT_BUFLEN);
	}

	// This is what decides whether a peer is sent anything at all, so when somebody is
	// connected but never spoken to, this is the number to look at.  Reported only when it
	// changes, or the per-turn call would bury the log.
	if (CommandLineParams.DoNetworkDebug() && a1 != IndexInNetwork_E1276)
	{
		static signed int lastState[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
		static uint8_t    lastCplt[8] = { 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE };
		if (a1 >= 0 && a1 < 8
			&& (lastState[a1] != v2 || lastCplt[a1] != connection_E12AE[a1]->ncb_cmd_cplt_49))
		{
			lastState[a1] = v2;
			lastCplt[a1] = connection_E12AE[a1]->ncb_cmd_cplt_49;
			debug_net_printf("SLOTSTATE: slot %d connected=%d lsn=%d cplt=0x%02X cmd=0x%02X name=[%.16s] call=[%.16s]\n",
				(int)a1, (int)v2, (int)connection_E12AE[a1]->ncb_lsn_2,
				(int)connection_E12AE[a1]->ncb_cmd_cplt_49, (int)connection_E12AE[a1]->ncb_command_0,
				connection_E12AE[a1]->ncb_name_26, connection_E12AE[a1]->ncb_callName_10);
		}
	}
	return v2;
}

//----- (00075044) --------------------------------------------------------
int setNetbios_75044(myNCB* connection)//256044
{
	//a1x 0x2b22aa
	type_v2x v2x; // [esp+0h] [ebp-7Ch]
	int v3; // [esp+10h] [ebp-6Ch]
	int v4; // [esp+1Ch] [ebp-60h]
	__int16 v5; // [esp+22h] [ebp-5Ah]
	__int16 v6; // [esp+24h] [ebp-58h]
	//REGS v7x;
	//int v7; // [esp+34h] [ebp-48h]
	//int v8; // [esp+38h] [ebp-44h]
	//char *v9; // [esp+48h] [ebp-34h]
	//REGS v10x;
	//char v10; // [esp+50h] [ebp-2Ch]
	//int v11; // [esp+68h] [ebp-14h]
	//SREGS v12x;
	//char v12; // [esp+6Ch] [ebp-10h]
	int v13; // [esp+78h] [ebp-4h]

	//fix it
	//v11 = 0;
	//fix it

	connection->ncb_cmd_cplt_49 = 0;
	memset(&v2x, 0, sizeof(type_v2x));//35517c
	v6 = connection->ncb_command_0 >> 4;
	v5 = connection->ncb_command_0 >> 4;
	v3 = 0;
	v4 = 256;
	//memset(&v7x, 0, sizeof(REGS));
	//memset(&v10x, 0, sizeof(REGS));
	//memset(&v12x, 0, sizeof(SREGS));
	//segread((SREGS*)&v12);
	/*v12x.es = 0x168;
	v12x.ds = 0x168;
	v12x.fs = 0x168;
	v12x.gs = 0x168;
	v12x.cs = 0x0;
	v12x.ss = 0x20;

	v7x.eax = 0x300;
	v7x.ebx = 0x5C;*/
	//v7x.edx = &v2x;
	simulateInterupt(connection);//Simulate Real Mode Interrupt //network
	/*if (v10x.esi)
		v13 = -1;
	else*/
	v13 = 0;
	return v13;

	/* if (CommandLineParams.ModeNetwork()) {
		a1x->byte_1 = 3;
	}
	return 1;*/
}

//----- (0007C230) --------------------------------------------------------
void WaitToConnect_7C230()//25d230
{
	long actTime;
	if (x_WORD_180660_VGA_type_resolution & 1)
	{
		CopyScreen((void*)x_DWORD_E9C38_smalltit, (void*)pdwScreenBuffer_351628, 320, 200);
	}
	else
	{
		CopyScreen((void*)x_DWORD_E9C38_smalltit, (void*)pdwScreenBuffer_351628, 640, 480);
	}
	sub_7C120_draw_bitmap_640(185, 232, xy_DWORD_17DED4_spritestr[66]);
	DrawMenuAnimations_7AB00();
	if (DrawScrollDialog_7BF20(&str_WORD_E1F70))
	{
		x_WORD_E131A = 1;
		ClearScrollDialogVars_7C020(&str_WORD_E1F70);
	}
	actTime = j___clock();
	if ((actTime - lastTime_E208C) >> 2 >= 1)
	{
		if (maxTimeCounter_E2097 >= timeCounter_E2094)
			timeCounter_E2094++;
		else
			timeCounter_E2094 = minTimeCounter_E2096;
		lastTime_E208C = actTime;
	}
	sub_7C120_draw_bitmap_640(x_DWORD_17DE38str.x_DWORD_17DEE4_mouse_positionx, x_DWORD_17DE38str.x_DWORD_17DEE6_mouse_positiony, xy_DWORD_17DED4_spritestr[timeCounter_E2094]);
	if (x_WORD_180660_VGA_type_resolution & 1)
		sub_90478_VGA_Blit320();
	else
		sub_75200_VGA_Blit640(480);
	sub_7A060_get_mouse_and_keyboard_events();
}

//----- (0007CE50) --------------------------------------------------------
bool SetMultiplayerColors_7CE50()//25de50
{
	__int16 colorIndex; // di
	bool result = false;

	x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].makeUpdate_0 = 1;
	NetworkUpdateConnections2_74374();//some with network
	printState(connection_E12AE);
	ReceiveSendAll_7438A((uint8_t*)x_DWORD_17DE38str.array_BYTE_17DE68x, sizeof(type_BYTE_17DE68x));
	for (int v0 = 0; v0 < 8; v0++)
	{
		for (int j = 0; j < 8; j++)
		{
			x_DWORD_17DE38str.array_BYTE_17DE68x[v0].playerIndex_1[j] = x_DWORD_17DE38str.array_BYTE_17DE68x[GetIndexNetwork2_74515()].playerIndex_1[j];
			x_BYTE_E131C[j] = x_DWORD_17DE38str.array_BYTE_17DE68x[v0].playerIndex_1[j];
		}
	}
	for (int i = 0; i < 8; i++)
	{
		if (x_DWORD_17DE38str.array_BYTE_17DE68x[i].makeUpdate_0)
		{
			switch (x_DWORD_17DE38str.array_BYTE_17DE68x[i].action_9)
			{
			case 1://end of select character
				for (colorIndex = 134; colorIndex < 148; colorIndex++)
				{
					x_DWORD_17DE38str.palette_17DE38x[colorIndex].red = 0;
					x_DWORD_17DE38str.palette_17DE38x[colorIndex].green = 0;
					x_DWORD_17DE38str.palette_17DE38x[colorIndex].blue = 0;
				}
				sub_41A90_VGA_Palette_install(x_DWORD_17DE38str.palette_17DE38x);

				x_DWORD_17DE38str.array_BYTE_17DE68x[i].makeUpdate_0 = 0;
				// A slot whose node vanished has already been handed over from the roster in
				// RemoveDeadClients; the orderly path below would try to receive a survivor
				// list that nobody sent.
				if (g_vanishedHandled[i])
					g_vanishedHandled[i] = false;
				else
					NetworkSomeChange_73AA1(i);
				if (i == x_DWORD_17DE38str.serverIndex_17DEFC)
					result = true;
				break;
			case 2:
				if (GetIndexNetwork2_74515() == GetIndexNetwork_74536())//begin of select character
				{
					if (i <= 0)
						x_DWORD_17DE38str.array_BYTE_17DE68x[GetIndexNetwork2_74515()].playerIndex_1[i] = i;
					else
						x_DWORD_17DE38str.array_BYTE_17DE68x[GetIndexNetwork2_74515()].playerIndex_1[i] = i - 1;
					x_DWORD_17DE38str.array_BYTE_17DE68x[GetIndexNetwork2_74515()].playerIndex_1[i] = FindFreeColorIndex_7D230(1, x_DWORD_17DE38str.array_BYTE_17DE68x[GetIndexNetwork2_74515()].playerIndex_1[i], i);
				}
				break;
			case 3://change to left character
				if (GetIndexNetwork2_74515() == GetIndexNetwork_74536() && x_DWORD_17DE38str.x_WORD_17DEFE < 7)
				{
					x_DWORD_17DE38str.array_BYTE_17DE68x[GetIndexNetwork2_74515()].playerIndex_1[i] = FindFreeColorIndex_7D230(1, x_DWORD_17DE38str.array_BYTE_17DE68x[GetIndexNetwork2_74515()].playerIndex_1[i], i);
				}
				break;
			case 4://change to right character
				if (GetIndexNetwork2_74515() == GetIndexNetwork_74536() && x_DWORD_17DE38str.x_WORD_17DEFE < 7)
				{
					x_DWORD_17DE38str.array_BYTE_17DE68x[GetIndexNetwork2_74515()].playerIndex_1[i] = FindFreeColorIndex_7D230(0, x_DWORD_17DE38str.array_BYTE_17DE68x[GetIndexNetwork2_74515()].playerIndex_1[i], i);
				}
				break;
			case 5://start multiplayer level
				D41A0_0.LevelIndex_0xc = x_DWORD_17DE38str.serverIndex_17DEFC;
				x_D41A0_BYTEARRAY_4_struct.levelnumber_43w = x_DWORD_17DE38str.array_BYTE_17DE68x[GetIndexNetwork2_74515()].selectedLevel_10;
				x_D41A0_BYTEARRAY_4_struct.setting_byte1_22 |= Setting::MULTIPLAYER_MODE;
				// NumberOfPlayers_0xe is used everywhere as a LOOP BOUND over array_0x2BDE,
				// not as a population count, and the two agree only while the players fill
				// slots 0..N-1.  After a server hand-over they do not: the node that died
				// leaves a hole, so the survivors sit at 1 and 2 while the count is 2 - and
				// the player in slot 2 falls outside every loop.  It never spawns, its entity
				// index is never filled in, and drawing its own status bar then divides by a
				// zero read out of unrelated memory (integer divide-by-zero in
				// DrawTopStatusBar_2D710, which killed that node a second into the level).
				//
				// Take the highest occupied slot instead.  For a normal contiguous game this
				// is exactly the old number, so nothing changes there.
				{
					int highestSlot = -1;
					for (int s = 0; s < 8; s++)
						if (x_DWORD_17DE38str.array_BYTE_17DE68x[s].makeUpdate_0) highestSlot = s;
					D41A0_0.NumberOfPlayers_0xe = (int16_t)(highestSlot + 1);
				}
				NetworkCancelAll_7449C();
				m_ExitMenuLoop_E29DC = 1;
				result = true;
				break;
			case 6:
				x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].selectedLevel_10 = x_DWORD_17DE38str.array_BYTE_17DE68x[i].selectedLevel_10;//select next level
				break;
			default:
				continue;
			}
		}
	}
	x_DWORD_17DE38str.array_BYTE_17DE68x[x_DWORD_17DE38str.serverIndex_17DEFC].action_9 = 0;
	x_DWORD_17DE38str.x_WORD_17DEF4 = x_DWORD_17DE38str.array_BYTE_17DE68x[GetIndexNetwork2_74515()].playerIndex_1[x_DWORD_17DE38str.serverIndex_17DEFC];
	DrawNetworkLevelName_7D1F0();
	return result;
}

//----- (0007D1F0) --------------------------------------------------------
void DrawNetworkLevelName_7D1F0()//25e1f0
{
	int index = 0;
	x_DWORD_17DE38str.x_WORD_17DEFE = 0;
	do
	{
		if (x_DWORD_17DE38str.array_BYTE_17DE68x[index].makeUpdate_0)
			x_DWORD_17DE38str.x_WORD_17DEFE++;
		index++;
	} while (index < 8);
}

//----- (0007D310) --------------------------------------------------------
void SetMultiplayerColors_7D310()//25e310
{
	for (int i = 0; i < 8; i++)
	{
		if (x_DWORD_17DE38str.array_BYTE_17DE68x[i].makeUpdate_0)
		{
			if (i != x_DWORD_17DE38str.serverIndex_17DEFC)
			{
				SetPaletteColor_7CDC0(i, x_DWORD_17DE38str.array_BYTE_17DE68x[GetIndexNetwork2_74515()].playerIndex_1[i]);
			}
		}
	}
}

//----- (0007D230) --------------------------------------------------------
int FindFreeColorIndex_7D230(char searchBackward, uint8 startColor, uint8 excludeSlot)
{
	signed int currentColor;     // ebx
	__int16 slotFwd;             // si
	__int16 slotBwd;             // si
	signed __int16 colorTakenBwd; // [esp+0h] [ebp-8h]
	signed __int16 colorTakenFwd; // [esp+4h] [ebp-4h]

	currentColor = startColor;
	if (searchBackward)
	{
		do
		{
			if ((--currentColor & 0x8000u) != 0)
				currentColor = 7;
			slotBwd = 0;
			colorTakenBwd = 0;
			while (slotBwd < 8)
			{
				if (slotBwd != excludeSlot &&
					x_DWORD_17DE38str.array_BYTE_17DE68x[slotBwd].makeUpdate_0 &&
					x_DWORD_17DE38str.array_BYTE_17DE68x[GetIndexNetwork2_74515()].playerIndex_1[slotBwd] == (x_WORD)currentColor)
				{
					colorTakenBwd = 1;
					break;
				}
				slotBwd++;
			}
		} while (colorTakenBwd);
	}
	else
	{
		do
		{
			if ((signed __int16)++currentColor > 7)
				currentColor = 0;
			slotFwd = 0;
			colorTakenFwd = 0;
			while (slotFwd < 8)
			{
				if (slotFwd != excludeSlot &&
					x_DWORD_17DE38str.array_BYTE_17DE68x[slotFwd].makeUpdate_0 &&
					x_DWORD_17DE38str.array_BYTE_17DE68x[GetIndexNetwork2_74515()].playerIndex_1[slotFwd] == (x_WORD)currentColor)
				{
					colorTakenFwd = 1;
					break;
				}
				slotFwd++;
			}
		} while (colorTakenFwd);
	}
	return currentColor;
}

//----- (0007CDC0) --------------------------------------------------------
void SetPaletteColor_7CDC0(uint8 colorIndex, uint8 playerIndex)//25ddc0
{
	((TColor*)*xadatapald0dat2.colorPalette_var28)[134 + 2 * colorIndex] = str_BYTE_E1711[0][playerIndex];
	((TColor*)*xadatapald0dat2.colorPalette_var28)[135 + 2 * colorIndex] = str_BYTE_E1711[1][playerIndex];
}

//----- (0005C450) --------------------------------------------------------
void NetworkDisallocation2_5C450()//23d450
{
	if (x_D41A0_BYTEARRAY_4_struct.isNetwork_216w)
		NetworkDisallocation_72D04();
}

const int StartNetworkTimeout = 5;

void InitNetworkInfo() {

	if (CommandLineParams.ModeNetwork()) {
		std::string exepath = get_exe_path();

		if (CommandLineParams.DoNetworkDebug())
			SetNetworkDebug();

		//debug_net_filename2 = exepath + "/../" + debug_net_filename1;

		//testlib1();
		/*if (Iam_server)
			InitLibNetServer(ServerMPort);
		InitLibNetClient(serverIP, ServerMPort, ClientMPort);*/
		InitMyNetLib(Iam_server, Iam_client, serverIP, NetworkPort, ServerPort);//not dedicate server
		/*
		if (Iam_server)
		{
			while (StartNetworkTimeout>0) {
				mydelay(1000);
				StartNetworkTimeout--;
				myprintf("I wait for clients %d s\n", StartNetworkTimeout);
			}
			SendMessagesRegisterOK();
		}
		bool receive_timeout = false;
		while (!receive_timeout) {
			receive_timeout = ReceiveTimeout();
			mydelay(1000);
		}*/
	}
};

void UpdateNetInfo()
{
#ifdef PRINT_UPDATE_CONNECTIONS
	char buffer[128];
	VGA_GotoXY(0, 20);
	snprintf(buffer, sizeof(buffer), "0: %02d %01d,%01d %02d",
		x_DWORD_17DE38str.array_BYTE_17DE68x[0].action_9,
		x_DWORD_17DE38str.array_BYTE_17DE68x[0].playerIndex_1[0],
		x_DWORD_17DE38str.array_BYTE_17DE68x[0].playerIndex_1[0], x_DWORD_17DE38str.array_BYTE_17DE68x[0].selectedLevel_10);
	VGA_Draw_stringXYtoBuffer(buffer, 0, 20, pdwScreenBuffer_351628);
	VGA_GotoXY(0, 40);
	snprintf(buffer, sizeof(buffer), "1: %02d %01d,%01d %02d",
		x_DWORD_17DE38str.array_BYTE_17DE68x[1].action_9,
		x_DWORD_17DE38str.array_BYTE_17DE68x[1].playerIndex_1[1],
		x_DWORD_17DE38str.array_BYTE_17DE68x[1].playerIndex_1[1], x_DWORD_17DE38str.array_BYTE_17DE68x[1].selectedLevel_10);
	VGA_Draw_stringXYtoBuffer(buffer, 0, 40, pdwScreenBuffer_351628);
#endif
}