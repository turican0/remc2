
#include <stddef.h>
#include "c_types.h"

#undef OPL3EMU_EXPORT
#define OPL3EMU_EXPORT _declspec(dllexport)

#ifdef __cplusplus
extern "C" {
#endif


/* == Context-dependent functions == */


OPL3EMU_EXPORT opl3emu_context opl3emu_create_context(void *instance_data);

OPL3EMU_EXPORT void opl3emu_free_context(opl3emu_context context);

OPL3EMU_EXPORT void opl3emu_init(opl3emu_context context, int samplerate);

OPL3EMU_EXPORT void opl3emu_writemididata(opl3emu_context context, DWORD dwdata);
	
OPL3EMU_EXPORT void opl3emu_getsample(opl3emu_context context, short *samplem, int len);

OPL3EMU_EXPORT void opl3emu_boardreset(opl3emu_context context);

OPL3EMU_EXPORT void opl3emu_playsysex(opl3emu_context context, Bit8u *bufpos, DWORD len);

OPL3EMU_EXPORT void opl3emu_setpatch_sbi(opl3emu_context context, Bit8u bankNum, Bit8u patchNum, Bit8u *buf, DWORD len);

OPL3EMU_EXPORT void opl3emu_setbank(opl3emu_context context, int banknum);

OPL3EMU_EXPORT int opl3emu_getactivevoicecount(opl3emu_context context);

OPL3EMU_EXPORT void opl3emu_setvolume(opl3emu_context context, WORD volume_atten);

OPL3EMU_EXPORT BYTE opl3emu_getpatch(opl3emu_context context, BYTE channel);

OPL3EMU_EXPORT void opl3emu_usenukedopl3(opl3emu_context context, bool value);


#ifdef __cplusplus
} // extern "C"
#endif

