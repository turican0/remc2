Now let’s move on to something else.

Specifically, we’ll add support for the games:

Magic Carpet 1 and Magic Carpet 1 Hidden Worlds (collectively known as Magic Carpet Plus)

If the game is launched with the -MC1 parameter,

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
wire the `-MC1` command line switch to it, and use DOSBox (`dosbox-x-remc1`) to compare with the original.
