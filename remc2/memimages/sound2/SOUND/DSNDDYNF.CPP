///////////////////////////////////////////////////////////////////////////////
// $Source: x:/prj/tech/libsrc/sound/RCS/dsnddynf.cpp $
// $Log: dsnddynf.cpp $
//Revision 1.2  1996/07/30  17:35:36  PATMAC
//Minor cleanups
//
//Revision 1.1  1996/07/30  11:27:12  PATMAC
//Initial revision
//
#include <windows.h>
#include <dsound.h>

#include "dsnddynf.h"

//
// DirectSoundCreate
//
ImplDynFunc(DirectSoundCreate, "dsound.dll", "DirectSoundCreate", 0);


BOOL LoadDirectSound()
{
    return DynFunc(DirectSoundCreate).Load();
}
