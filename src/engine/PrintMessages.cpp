#include "PrintMessages.h"
//----- (00070890) --------------------------------------------------------
void PrintHeader_70890()//251890
{
	myprintf("Copyright (c) 1995 Bullfrog Productions Ltd.\n");
	myprintf("All rights reserved.\n");
	myprintf("%s \n", "Magic Carpet 2 (Netherworlds)");
}

//----- (00070910) --------------------------------------------------------
void PrintTextMessage_70910(char* text)//251910
{
	if (x_D41A0_BYTEARRAY_4_struct.setting_byte2_23 & 1)//fix it
		myprintf("%s\n", text);
}


