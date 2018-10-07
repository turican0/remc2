#ifndef AIL_SOUND_ACTIVE
#define AIL_SOUND_ACTIVE
#include "../dosbox_files/config.h"

#define MSS_STRUCT struct

typedef MSS_STRUCT                     // VDI interface register structure
{
	Bit16s AX;
	Bit16s BX;
	Bit16s CX;
	Bit16s DX;
	Bit16s SI;
	Bit16s DI;
}
VDI_CALL;


#endif //AIL_SOUND_
