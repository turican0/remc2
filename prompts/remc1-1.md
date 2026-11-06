Now let’s move on to something else.

Specifically, we’ll add support for the games:

Magic Carpet 1 and Magic Carpet 1 Hidden Worlds (collectively known as Magic Carpet Plus)

If the game is launched with the -game parameter (`-game mc1` Magic Carpet 1, `-game mc1hw` Hidden Worlds,
`-game select` the Magic Carpet Plus selector, `-game mc2` or no parameter the classic MC2),

the code will switch to this version.

Everything specific to MC1 will be in the remc1 directory.

The structure will be similar to that of remc2.

Unique functions will remain here.

Functions that are identical will have names in the .h file that include the addresses from remc1 (and remc1hw, if they differ), but they will be pointers to the original functions.

The same applies to global variables, which will be the same.

The portable code will definitely be shared.

Data files have the prefix MC1

(see c:\\prenos\\remc2-dev2\\remc2\\x64\\Release\\MC1\\)

We’ll start with the switch; it’s in the file:

SELECT.EXE.c

But gradually separate the code from it into separate functions.

We always want clean functions—no GOTO statements, well-separated structs, named variables, etc.

## Sources of truth and references

- The asm listing is the bible. The decompiled `.c` is only a hint; every function is checked against the asm.
  Exceptions are allowed only in the portability layer (SDL, files, timer, sound...).
- IDA 9.1: `c:\prenos\ida9.1\` (batch listing: `idat.exe -A -B -L<log> <file.EXE>`).
- Listings: `c:\prenos\dosbox-x-remc1\ida\` (`SELECT.EXE.asm`, `.i64`; CARPET.EXE and HIDDEN.EXE to follow).
- Original game files: `c:\prenos\dosbox-x-remc1\mc1-orig\` (copy of `x64\Release\MC1\`).
- DOSBox for comparing with the original: `c:\prenos\dosbox-x-remc1` = clone of `turican0/dosbox-x-remc2`,
  branch `mc1` (from `master`). `vs2015\dosbox.conf` mounts `mc1-orig` as C: and starts `SELECT.EXE`.
  Engine hooks (`src\engine\engine.cpp`) are keyed by EIP of NETHERW.EXE (MC2) and must be adapted for MC1.
- Reference code (local clones in `c:\prenos\mc1-ref\`):
  - https://github.com/turican0/remc1 - REMC1 decompilation of the MC1 CD executable.
  - https://github.com/drdanick/Magic-Carpet-1-SDL/tree/port/64bit - playable 64-bit SDL port built on REMC1
    (recordings, regression tools, `docs/original-bugs.md`).
- remc2 work for MC1 is done on branch `magic-carpet-1`.
- Structs: remc2 structs (and their member names) are a good inspiration, but MC1 did not have all the features
  of MC2 yet and some things were laid out differently - always check sizes and offsets against the MC1 asm.

## What SELECT.EXE does

The Magic Carpet Plus selector: loads `DATA\SELP0-0.DAT` (palette) and `DATA\SEL0-0.DAT` (graphics), or the
`1-0` pair for the other resolution, lets the player pick Magic Carpet or Hidden Worlds with the mouse and writes
the choice to `C:\CARPET.CD\CP.DAT` (removed when cancelled); a batch file then starts CARPET.EXE or HIDDEN.EXE.
Part of the code only creates the install directories when installing from the CD.

## How the game code is written

- Game logic is written primarily from the IDA listings (CARPET.EXE and HIDDEN.EXE); the GitHub projects
  (REMC1, Magic-Carpet-1-SDL) help with names, structs and understanding, but are not copied blindly.
- Function maps (in `c:\prenos\dosbox-x-remc1\ida\`): `mc1_hw_map.txt` (CARPET <-> HIDDEN functions),
  `mc1_hw_data_map.txt` (data labels), `mc1_mc2_map.txt` (CARPET <-> NETHERW, identical / similar).
- Shared with remc2 where the code is the same: rasterizer, 2D blitters, cursor, lines, VGA/palette, RNC, FLI.
- One renderer for MC1, MC1 HW and MC2: renderers get the game state through a scene interface instead of
  reading MC2 globals directly; what MC1 has extra or different is added to the renderer as variants.
  The same applies to the in-game 2D graphics (shared drawing, game-specific UI logic).

## Milestones and not ported functions

- The port goes by runnable milestones: start -> intro, menu, level + game loop, renderer (scene interface).
- Functions already called but not ported yet are temporary stubs with the final name and signature in
  `remc1/engine/Mc1NotPorted.cpp`, each marked `//!!!!! not ported yet` and logging it when called.
  A stub is removed (moved to its real file) when the function is ported from IDA.
- File names in `remc1` must not clash with remc2 file names (same MSBuild project, object files would collide):
  MC1 files start with `Mc1` (e.g. `Mc1Game.cpp`).

## Sound

MC1 uses the HMI (SOS) sound library, MC2 uses Miles (MSS/AIL). The MC1 port is rewritten to use MSS,
i.e. the remc2 MSS emulation (the AIL layer over SDL, including AWE32), not HMI.

## Addresses

- CARPET.EXE / HIDDEN.EXE in DOSBox: code = IDA + 0x1D1000, data = IDA + 0x1BE000
  (e.g. `TopProcedure_340B0_34470` = DOSBox 2050B0 / 205470, `begTextDat_AE41C_AE40C` = 26C41C / 26C40C).
- REMC1 / Magic-Carpet-1-SDL already use the same style of names with MC1 and HW addresses.

## MC1 and Hidden Worlds

CARPET.EXE (MC1) and HIDDEN.EXE (MC1 Hidden Worlds) share most of the code, but at different addresses.
- Identical functions/globals exist once and carry both addresses in the name (MC1 first, then HW),
  e.g. `DrawSprite_1A2B0_1A3C0`.
- Functions/globals that differ are duplicated, one per game, each with its own address.
- Everything is verified against both executables (both asm listings, both games in DOSBox).

## Memory

The original did not care much about freeing memory (one exe ended and started the next one). In the port every
allocation must have a matching release: init/shutdown pairs, owned buffers, nothing left behind when the
selector returns to the caller.

## Rules carried over from remc2

- Names: functions `WordsWithCapitals_ADDR` (IDA address), globals `camelCase_ADDR`,
  struct members `camelName_0xOFF_DEC`; `main`/`sub_main` and port-only functions without an address.
- No calling conventions in code (`__fastcall`, `__usercall`, `__noreturn`, `@<reg>`).
- Comments in English and short: the address (DOSBox) and the instruction, no long explanations.
- Every function, global and struct is declared in the `.h` with the same name as the `.cpp` that defines it.
- Include layering: type headers (only types) -> module headers (declarations, include only type headers they need)
  -> `X.cpp` includes `X.h` first, then the module headers it really uses.
- Portability must not include engine headers (platform-dependent code stays independent).
- Do not move code between directories without explicit approval; moving between files is fine.
- Bugs of the original are not fixed silently: mark them with `//!!!!!` and a note, and ask.
- Before a commit: MSBuild x64 Release and Debug (remc2, regression test, editor), x86 Release
  (clang `--target=i686-pc-windows-msvc -fsyntax-only` is a quick check), Linux GCC syntax check through WSL.
  x86 MSBuild changes the tracked `remc2\Release\remc2.pch` - restore it. Regressions only when asked.

## First step

Start with SELECT: port it into `remc1\` as clean functions verified against `SELECT.EXE.asm`,
wire the `-game` command line option to it, and use DOSBox (`dosbox-x-remc1`) to compare with the original.
