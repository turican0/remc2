#include "Network.h"

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
			debug_net_printf("STUCK: %s pending %lds - cmd=0x%02X ret=0x%02X lsn=%d name=[%.16s] call=[%.16s]\n",
				what, waited, (int)ncb->ncb_command_0, (int)ncb->ncb_retcode_1, (int)ncb->ncb_lsn_2,
				ncb->ncb_name_26, ncb->ncb_callName_10);
		}
	}
	if (reported)
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
	for (i = 0; maxPlayers_E127A > i; i++)
		connected_E12CE[i] = 0;
	for (i = 0; maxPlayers_E127A > i; i++)
	{
		WaitForNcb_diag(connection_E12AE[i], "join: player NCB before add name");
	}
	WaitForNcb_diag(mainConnection_E12AA, "join: main NCB before add name");//AddNameNotSet?

	// clear stale per-match network state left over from a previous
	// game before starting this one, otherwise the peers fail to reconnect.
	ResetNetworkGameState();

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
				x_DWORD_17DE38str.array_BYTE_17DE68x[i].makeUpdate_0 = 1;
				x_DWORD_17DE38str.array_BYTE_17DE68x[i].action_9 = 1;
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
	signed int v2;
	if (a1 == IndexInNetwork_E1276)
		v2 = 2;
	else
		v2 = connection_E12AE[a1]->ncb_lsn_2 && !connection_E12AE[a1]->ncb_cmd_cplt_49;
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
				D41A0_0.NumberOfPlayers_0xe = x_DWORD_17DE38str.x_WORD_17DEFE;
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