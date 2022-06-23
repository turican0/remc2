Fork of Bass_VST for Soundfont Midi Player by Zoltan Bacsko(Falcosoft)

version 2.4.0.9 

- Modified IDLE_UNLOAD_PENDING_COUNTDOWN from 10 to 5 sec. 
  Since some plugins require complete unload before reload 10 sec wait time is too much.
- Added cleanUpPlugins() function to remove loaded plugins before unload. It matters when you load/unload bass_vst dynamically.
- Added Visual C++ 6 project for x86 version. Later VC++ versions depend on different version dependendt runtime (msvcrt8x, msvcrt10 etc.).
  Contrary VC6 compiled binaries always depend on msvcrt.dll that is part of every windows operating system. 
  So this way even Win9x/Win2000 is supported by the x86 version. 
  But the x64 version depends on MS Visual C++ 2010 x64 redistributable.
			
version 2.4.0.10

- Added BASS_VST_SetEditKnobMode (knobMode 0: circular, 1: circular relativ, 2: linear)
- Modified BASS_VST_PARAM_INFO members maximum characters to 24. 
  (I know VST specification dictates maximum 8 character but it is ridiculously low and no plugins/hosts respect this limit.)  
- X64 version no longer depends on MS Visual C++ 2010 x64 redistributable.


