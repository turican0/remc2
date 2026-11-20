We continue the port of Magic Carpet 1 and Hidden Worlds into remc2 (branch `magic-carpet-1`).

Read first:
- `prompts/remc1-1.md` - the rules of the port (names, structs, shared code, comments, builds).
- `remc1/docs/handover.md` - the state, where the code is, the DOSBox branch, the tools and where to continue.

Sources of truth:
- The asm listings in `remc1/asm/` (`CARPET.EXE.asm` MC1, `HIDDEN.EXE.asm` Hidden Worlds, `NETHERW.asm` MC2) are the
  bible - every function is written from the listing and checked against it; the maps `mc1_hw_map.txt`,
  `mc1_mc2_map.txt` say what is the same in HW / MC2. There is no IDA.
- DOSBox `turican0/dosbox-x-remc2` branch `mc1` with the `REMC1_DUMP` / `REMC1_POKE` hooks for comparing the port
  with the original (`remc1/tools/dbxdump.py`).

Work:
- Port the next not ported functions (stubs in `remc1/engine/Mc1NotPorted.cpp`, the order in the handover: the
  creature actions first), one function tree at a time, faithful to the asm, both MC1 and HW.
- Verify each step against DOSBox (a dump at a code address vs the same state in the port, a temporary hook in the
  port that is removed afterwards) and say what was and was not verified.
- MC2 must not change: shared code only when it is the same, the renderer changes behind `RenderMc1()`, the MC2
  renderer tests when a renderer changes.
- Build Release x64 (`-m`), GCC check through WSL, no commits unless asked.
