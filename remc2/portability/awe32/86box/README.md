# EMU8000 chip from 86Box

`snd_emu8k.c` is the EMU8000 emulation from [86Box](https://github.com/86Box/86Box)
(`src/sound/snd_emu8k.c`), with the AWE32Emu changes measured on a real
AWE32 (marked `AWE32Emu:` in the code): filter, envelopes, interpolation,
effects, output level and the fixes of upstream bugs. It is the chip behind
`--chip 86box`, the default when `--rom` is given.

`include/86box/` holds `snd_emu8k.h` and small stand-ins for the 86Box
headers the file includes (`86box.h`, `device.h`, `io.h`, `mem.h`, `rom.h`,
`sound.h`, `timer.h`, `plat_unused.h`) and the empty variants of the trace
hooks (`snd_emu8k_trace.h`, `awe32_trace.h`) - enough to build the chip
outside 86Box.

**License:** 86Box, and so this file, is licensed under the GNU General
Public License, version 2 or later. A build of AWE32Emu that includes it is a
derivative work of 86Box.

**Relation to the 86Box tree:** the DOS test VM is built from its own copy in
the 86Box source tree. The two copies are separate files; a change of the
chip has to go into both. `chipcheck.py` (AWE32Emu against `emu8k_ref`,
which is built from the 86Box tree) shows whether they still produce the
same output.
