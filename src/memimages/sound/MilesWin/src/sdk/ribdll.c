#include "mss.h"
#include "imssapi.h"

BOOL __stdcall DllMain( HINSTANCE hinstDll, DWORD fdwRreason, LPVOID plvReserved)
{
  return( 1 );
}

/* @cdep pre

   $define( STDLIBs, )
   $requires( radlibc.c )
   $addtocswitches( -DUSE_MSS )

   $if($equals($clipdir($outname),mss32.dll), ,$requiresbinary(mss32.lib) )
*/
