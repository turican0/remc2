#ifndef _LGSNDID_H
#define _LGSNDID_H

#ifdef USE_OLD_GOBBLEDYGOOK_SOUND_GUIDS

// {DAB34A41-823E-11cf-872A-00AA00B726A5}
DEFINE_GUID(IID_ISndMixer, 
0xdab34a41, 0x823e, 0x11cf, 0x87, 0x2a, 0x0, 0xaa, 0x0, 0xb7, 0x26, 0xa5);

// {DAB34A42-823E-11cf-872A-00AA00B726A5}
DEFINE_GUID(IID_ISndSample, 
0xdab34a42, 0x823e, 0x11cf, 0x87, 0x2a, 0x0, 0xaa, 0x0, 0xb7, 0x26, 0xa5);

#else

DEFINE_LG_GUID(IID_ISndSample,      0x10);
DEFINE_LG_GUID(IID_ISndMixer,       0x11);
DEFINE_LG_GUID(IID_IMidiSequencer,  0x12);
DEFINE_LG_GUID(IID_IMidiSequence,   0x13);
// spare GUIDs for the sound library:
//DEFINE_LG_GUID(UUID_IDENTIFIER, 0x14);
//DEFINE_LG_GUID(UUID_IDENTIFIER, 0x15);
//DEFINE_LG_GUID(UUID_IDENTIFIER, 0x16);
// guid 0x17 used by CrowdSnd

#endif

#endif
