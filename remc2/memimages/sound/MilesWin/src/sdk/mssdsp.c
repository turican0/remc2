#include "mss.h"
#include "imssapi.h"

/* @cdep pre
  $requires(bandpass.cpp)
  $requires(allpass.cpp)
  $requires(capture.cpp)
  $requires(chorus.cpp)
  $requires(compress.cpp)
  $requires(flange.cpp)
  $requires(highpass.cpp)
  $requires(lowpass.cpp)
  $requires(parmeq.cpp)
  $requires(phaser.cpp)
  $requires(reson.cpp)
  $requires(ringmod.cpp)
  $requires(delay.cpp)
  $requires(shelfeq.cpp)
  $requires(volramp.cpp)
  $requires(gainblk.cpp)
*/

extern S32 BandPassMain( HPROVIDER provider_handle, U32 up_down );
extern S32 AllPassMain( HPROVIDER provider_handle, U32 up_down );
#if defined( IS_WIN32API ) && !defined(FORNONWIN)
extern S32 CaptureMain( HPROVIDER provider_handle, U32 up_down );
#endif
extern S32 ChorusMain( HPROVIDER provider_handle, U32 up_down );
extern S32 CompressMain( HPROVIDER provider_handle, U32 up_down );
extern S32 FlangeMain( HPROVIDER provider_handle, U32 up_down );
extern S32 HighPassMain( HPROVIDER provider_handle, U32 up_down );
extern S32 LowPassMain( HPROVIDER provider_handle, U32 up_down );
extern S32 DelayMain( HPROVIDER provider_handle, U32 up_down );
extern S32 ParmEqMain( HPROVIDER provider_handle, U32 up_down );
extern S32 PhaserMain( HPROVIDER provider_handle, U32 up_down );
extern S32 ResonMain( HPROVIDER provider_handle, U32 up_down );
extern S32 RingModMain( HPROVIDER provider_handle, U32 up_down );
extern S32 ShelfEqMain( HPROVIDER provider_handle, U32 up_down );
extern S32 RampMain( HPROVIDER provider_handle, U32 up_down );
extern S32 GainMain( HPROVIDER provider_handle, U32 up_down );

DXDEF S32 AILEXPORT RIB_MAIN_NAME(DSP)( HPROVIDER provider_handle, U32 up_down )
{
  static HPROVIDER bandpassPH, 
                   allpassPH,
#if defined( IS_WIN32API ) && !defined(FORNONWIN)
                   capturePH, 
#endif
                   chorusPH, compressPH, flangePH,
                   highpassPH, lowpassPH, delayPH, parmeqPH,
                   phaserPH, resonPH, ringmodPH, shelfeqPH, rampPH, gainPH;

  if ( up_down )
  {
    rampPH = provider_handle;
    bandpassPH = RIB_alloc_provider_handle(0);
    allpassPH  = RIB_alloc_provider_handle(0);
#if defined( IS_WIN32API ) && !defined(FORNONWIN)
    capturePH = RIB_alloc_provider_handle(0);
#endif
    chorusPH = RIB_alloc_provider_handle(0);
    compressPH = RIB_alloc_provider_handle(0);
    flangePH = RIB_alloc_provider_handle(0);
    highpassPH = RIB_alloc_provider_handle(0);
    lowpassPH = RIB_alloc_provider_handle(0);
    delayPH = RIB_alloc_provider_handle(0);
    parmeqPH = RIB_alloc_provider_handle(0);
    phaserPH = RIB_alloc_provider_handle(0);
    resonPH = RIB_alloc_provider_handle(0);
    ringmodPH = RIB_alloc_provider_handle(0);
    shelfeqPH = RIB_alloc_provider_handle(0);
    gainPH = RIB_alloc_provider_handle(0);
  }

  BandPassMain( bandpassPH , up_down );
  AllPassMain( allpassPH , up_down );
#if defined( IS_WIN32API ) && !defined(FORNONWIN)
  CaptureMain( capturePH, up_down );
#endif
  ChorusMain( chorusPH, up_down );
  CompressMain( compressPH, up_down );
  FlangeMain( flangePH, up_down );
  HighPassMain( highpassPH, up_down );
  LowPassMain( lowpassPH, up_down );
  DelayMain( delayPH, up_down );
  ParmEqMain( parmeqPH, up_down );
  PhaserMain( phaserPH, up_down );
  ResonMain( resonPH, up_down );
  RingModMain( ringmodPH, up_down );
  ShelfEqMain( shelfeqPH, up_down );
  RampMain( rampPH, up_down );
  GainMain( gainPH, up_down );

  return( TRUE );
}

