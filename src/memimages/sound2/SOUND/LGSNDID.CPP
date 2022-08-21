#include <comtools.h>
#include <initguid.h>

#include <lgsndid.h>

// Instantiate DirectSound guids which we need to QueryInterface on, which
// otherwise would be unresolved because we don't statically link to dsound

// copied from dsound.h
// Yes, I shall roast a while longer in Hell for this one - patmc

//DirectSound3DListener 279afa84-4981-11ce-a521-0020af0be560
DEFINE_GUID(IID_IDirectSound3DListener,0x279AFA84,0x4981,0x11CE,0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60);
//DirectSound3DBuffer 279afa86-4981-11ce-a521-0020af0be560
DEFINE_GUID(IID_IDirectSound3DBuffer,0x279AFA86,0x4981,0x11CE,0xA5,0x21,0x00,0x20,0xAF,0x0B,0xE5,0x60);

// A3d Class ID! {D8F1EEE0-F634-11cf-8700-00A0245D918B}
DEFINE_GUID(CLSID_A3d, 
0xd8f1eee0, 0xf634, 0x11cf, 0x87, 0x0, 0x0, 0xa0, 0x24, 0x5d, 0x91, 0x8b);

// A3d Interface ID! {D8F1EEE1-F634-11cf-8700-00A0245D918B}
DEFINE_GUID(IID_IA3d,
0xd8f1eee1, 0xf634, 0x11cf, 0x87, 0x0, 0x0, 0xa0, 0x24, 0x5d, 0x91, 0x8b);


