# Magic Carpet 1 / Hidden Worlds in remc2 - handover

State of the port on branch `magic-carpet-1` (2026-11-15) and how to continue. The rules of the work are in
`prompts/remc1-1.md` (read it first); the prompt to start a new session is `prompts/remc1-continue.md`.

## What runs

- `remc2.exe -game mc1|mc1hw|select|mc2` (`-custom -level N` starts a level directly, `REMC2_SKIPINTRO=1` skips the
  intros). The selector, the intros, the main menu with its dialogs, the sound (HMI SOS emulated over the remc2 MSS
  layer, AWE32), the level loading (map, features, models, entities, tmaps) and the game loop run.
- In the game: the player's wizard flies (keys, mouse, gamepad through the remc2 SDL layer), the spells of the mouse
  buttons work as far as the wizard goes, the frame (20FB0) draws the world, the minimap, the texts, the help, the
  spells grid, the map mode and the demo credits; the palette fades in (34690).
- Renderers: Original, NG, HD and HW draw MC1 (`render` in config.json). With the entity state of DOSBox injected,
  level 0 is pixel identical for Original, NG and HD (config with `viewDistanceScale: 1`); HW runs (GPU, not
  compared pixel by pixel).
- Not verified yet: the MC2 renderer tests after the sprite interface in HD, NG and HW (only Original was run
  against the DOSBox screens of MC2: level 4 identical, level 2 / record005 only the known 1 px minimap dots of the
  original); the palette fade in of the level (34690) and the HW textures of MC1 were only seen running.
- What is still missing in the game (stubs in `remc1/engine/Mc1NotPorted.cpp`, logged once when called):
  - the actions of the creatures (19550, 1B200, 1B510, 1DDE0, 1EED0, 1F640, 1FAC0: about 1500 asm lines of a
    shared core) - the creatures do not move yet, which is also why a frame differs from DOSBox in their positions;
  - the computer wizard (class 3 action 1, 13170), class 10/11/12 actions (26A60, 27030, 28DC0, 59A80, 59D40,
    56250 ...);
  - the icons of the frame (23D40, 22E50, 24230, 23CF0, 24D20, 24DA0, 22880), the frame end texts (5A560),
    415C0, 55100;
  - the sounds of the game (55370, 55890, 20D00), the save / load in the game (3E690, 3E750, 3E7F0, 3E8C0 - they
    save the whole state with pointers, needs a pointer free shadow state like remc2 `Convert_from_shadow_*`), the
    network, the VFX1 head tracker, the joystick calibration;
  - the motion blur by the speed in the renderers.

## Where things are

- `remc1/` - all MC1 code (`Mc1*.cpp`, names `Name_MC1ADDR_HWADDR`), `remc1/engine/Mc1Shared.h` maps MC1 names to the
  shared remc2 code (constexpr pointers), `remc1/engine/Mc1GameState.h` the state / settings / entity structs.
- Main game files: `Mc1World.cpp` (game loop 34530, world tick), `Mc1Frame.cpp` (20FB0), `Mc1Input.cpp` (17C20,
  16B00, joystick 3FCA0), `Mc1Wizard.cpp` (the player's wizard 45C90), `Mc1Minimap.cpp` (49300, 48710),
  `Mc1PlayerEvents.cpp` (3C9D0), `Mc1Render.cpp` (the adapter to the remc2 renderers), `Mc1TextureMaps.cpp`
  (the tmap cache), `Mc1Screen.cpp` (graphics, palette 34690).
- Renderers: MC1 differences are switched by `RenderMc1()` (`remc2/engine/GameRenderInterface.h`). The sprites are read
  through the scene interface (`RenderEntity`, `RenderLoadedTmap`, `RenderLoadTmap`, `RenderTmapUsed`,
  `RenderSpriteDef`, `RenderTmapAnimation`); MC1 sets its source while it draws (`Mc1Render.cpp`, the MC1 entities
  converted to the MC2 entity layout, only the fields the sprites read). The HW renderer gets its textures by events:
  `Mc1Render.cpp` sends the block atlas (256 tiles), the sky and the loaded tmaps when it creates the renderer,
  `LoadTmapGroup_58B30` the next tmaps.
- `remc1/asm/` - the IDA listings (there is no IDA needed): `CARPET.EXE.asm` (MC1), `HIDDEN.EXE.asm` (Hidden Worlds),
  `SELECT.EXE.asm`, `NETHERW.asm` (MC2, for comparing), function / data maps `mc1_hw_map.txt` (MC1 <-> HW functions),
  `mc1_hw_data_map.txt` (data), `mc1_mc2_map.txt` (MC1 <-> MC2: `same` / `similar` - check by hand, the map ignores
  constants), `*.funcs.txt` (function lists).
- `remc1/docs/` - `game-mc1-mc2.md/.tsv` (the game code of MC1 against MC2), `renderer-mc1-mc2.md`, this file.

## DOSBox for comparing with the original

- https://github.com/turican0/dosbox-x-remc2 branch `mc1` (build `vs2015/dosbox-x.sln`, Release x64). The hooks in
  `src/debug/debug.cpp` (note: `src/debug` is in `.gitignore`, add changes with `git add -f`) work with the normal
  core and environment variables (hex, linear addresses: code = IDA + 1D1000h, data = IDA + 1BE000h):
  - `REMC1_DUMP=<code>,<data>,<length>,<file>[;...]` writes the memory when the code address runs,
    `REMC1_DUMP_HIT=n` the n-th time, `REMC1_DUMP_EXIT=1` ends DOSBox after the last dump;
  - `REMC1_POKE=<code>,<data>,<hex bytes>[;...]` writes bytes when the code address runs (once);
  - `REMC1_NOISE=<code>[,...]` the stereogram noise of the port; the original bug at 592D7 / 597E7 is fixed as in
    the port.
- The original game files: the MC1 CD content (also `x64/Release/MC1/` of remc2) as drive C:, a conf with
  `cputype=pentium`, `core=normal` and the autoexec `CARPET.EXE -custom -level N` (`tools/mkconf.py` makes it from
  `vs2015/dosbox.conf`).

## Tools (`remc1/tools/`)

Python 3 and Git Bash; some scripts still have the paths of the original machine (`C:/prenos/remc2-dev2/remc2`,
`c:\prenos\dosbox-x-remc1`) - change them at the top. The test outputs are written next to the scripts.

- `dbxdump.py <mc1|hw> <code IDA> <outdir> name=<IDA data>:<len> | name=@<linear>:<len> ...` - runs DOSBox and
  dumps memory at the code address (env `DBXCONF` = the conf, `REMC1_DUMP_HIT`, `REMC1_POKE` pass through).
- `mkconf.py "<CARPET.EXE args>" <conf>` - a DOSBox conf for a level.
- Comparisons port vs DOSBox (they need a temporary hook in the port: read the script, the hook reads
  `MC1_TEMP*` env variables in `GameFrame_34530` and is removed after the check - nothing temporary is committed):
  - `verframe.sh / cmpframe.py` - the state after 3C9D0 (maps, entities, players);
  - `frametest.sh <level> <mode>` - the screen after 20FB0 with the player mode forced;
  - `sprtest.sh <level> <mode>` - the same with the entities and the entity map of DOSBox injected (renderer check);
  - `wiztest.sh <level> <frame> <scancode>` - the player and the wizard after n frames with a key held;
  - `keytest.sh`, `enttest.sh`, `verent.sh / cmpent.py` - keys, entities.
- `pairdiff.py <MC1 sub> <MC2 sub> [show]` - an MC1 function against an MC2 one (normalized): only compiler
  differences = the remc2 function can be shared (then verify with a DOSBox dump).
- `unported.py carpet_calls2.txt <sub>` - the unported asm lines under a function (what a port of it pulls in).
- `mkgcc.py <out.sh> <files>` + `gcc_render.sh` - Linux GCC syntax check through WSL.

## How the work was done (keep it)

- The asm is the bible: every function is written from the listing, branch by branch, and verified against DOSBox
  (a dump at a code address in the original vs the same point in the port). Portability exceptions follow the MC2
  port.
- Shared with remc2 only what is 100 % the same code (`pairdiff.py`, then a DOSBox check); otherwise an own MC1 function.
- Bugs of the original: fixed in the port and in the DOSBox branch (so the comparisons stay equal), or marked `//!!!!!`.
- Short English comments; build with `MSBuild remc2.sln -t:remc2 -p:Configuration=Release -p:Platform=x64 -m`; GCC
  check through WSL before a commit.

## Where to continue (suggested order)

1. The actions of the creatures (the shared core 19550 ... 1FAC0), then the frame matches DOSBox without injection.
2. The icons of the frame (23D40, 22E50 ...) and 5A560, 415C0, 55100.
3. The computer wizard 13170 and the other class actions.
4. The sounds of the game (55370 ...), then save / load with a shadow state.
5. HW renderer: compare visually (a GPU frame cannot be dumped), the motion blur.
