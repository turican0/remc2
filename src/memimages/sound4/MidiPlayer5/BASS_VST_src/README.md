
BASS_VST
===============================================================================

BASS_VST allows the usage of VST plugins in BASS.  BASS_VST was written to work
with Silverjuke(R).  Any other usage is for your own risk - but you're welcome
and I'm sure it will work :-)




Files
===============================================================================

- README.md - this file
- LICENSE - BASS_VST license information (BASS_VST ist released under LGPL)
- bass_vst.h - BASS_VST C/C++ header file and documentation
- bass_vst.dll - binary BASS_VST module
- bass_vst.lib - binary BASS_VST import library to access bass_vst.dll easily
- source - the source files are placed in this directory

If the binary files are missing, you can find them at 
http://www.un4seen.com/forum/?topic=5559.0 or you can build them from the source 
directory by creating a simple DLL project using your favorite IDE.  In tihs 
case you'll need some more files not included in this package:

- bass.h, bass-addon.h, bassmidi.h - you'll find them on http://www.un4seen.com
- aeffectx.h - get this file on https://www.steinberg.net/en/company/developer.html




Usage in a glance
===============================================================================

Example:

    #include <bass.h>
    #include <bass_vst.h>

    // create a normal BASS stream
    DWORD ch = BASS_StreamCreateFile(false, "c:\\sth.mp3", 0, 0, 0);

    // assign a VST plugin DLL to the channel -- the returned handle can be used
    // with all BASS_VST_*() functions
    DWORD dsp = BASS_VST_ChannelSetDSP(ch, "c:\\equalizer.dll", 0, 0);

    // that's all
    BASS_ChannelPlay(ch, false);

    // ... wait for finish ...

    // delete the stream - this will also delete the VST effect
    BASS_StreamFree(ch);



That's all for using VST effects, which will probably the most common usage.
However, you can also use VST instruments (VSTi plugins) with BASS_VST:

    // create a VST instrument stream -- the returned handle can be used with
    // all BASS_Channel*() and all BASS_VST_*() functions
    DWORD ch = BASS_VST_ChannelCreate(44100, 2, "c:\\instr.dll", 0);

    // the returned handle can be used like any other BASS channel handle -
    // eg. you can add a VST effect and start the stream
    BASS_VST_ChannelSetDSP(ch, "c:\\equalizer.dll", 0, 0);
    BASS_ChannelPlay(ch);

    // ... wait for finish ... trigger notes using BASS_VST_ProcessEvent() ...

    // when done, delete the VST instrument stream
    BASS_VST_ChannelFree(ch);




Further information
===============================================================================

For all functions and parameter descriptions, please refer to bass_vst.h -
another documentation is not available at the moment (and probably will never
be). Also note the version history and the disclaimer in this file.

(C) Bjoern Petersen Software Design and Development
VST PlugIn Interface Technology by Steinberg Media Technologies GmbH

Contact: dr.silver@silverjuke.net - http://www.silverjuke.net

You can donate via Paypal to paypal@silverjuke.net

Our page on un4seen: http://www.un4seen.com/forum/?topic=5559.0

