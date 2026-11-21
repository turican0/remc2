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
- Level 1 (index 0, also New game) against DOSBox: without input identical for 3000 frames (the wizard is killed by
  the creatures and lies dead); with a synthetic input (flight, all spells by the cheat command, fireball,
  possession, the castle spell: the castle built and grown, its balloon and guards, fires, burning trees) identical
  up to frame 2999 (entities, players, terrain maps, the 320x200 screen); a random input of 8000 frames (seed 7: the
  castle destroyed and rebuilt, falling buildings, the creatures from the buildings) and of 12000 frames (seed 11)
  identical at their ends; with the spells grid, the map and the keys of the spells (4000 frames) too. Only the mark
  of the mouse on the map of the spells grid differs (the mouse position is not in the recording).
  The level lost by the rebirth without a castle (408D0 loads it again) and the level won (command 27, 448E0 the
  statistics, then level 2 loads) match too.
- All actions and models of the class tables are ported: the creatures (`Mc1Creatures.cpp`), the castle
  (`Mc1Castle.cpp`), the spells cast by class 12, their class 9 projectiles and the class 11 triggers of the levels
  (`Mc1Spells.cpp`), the class 10 effects (`Mc1SceneryActions.cpp`), the model creators (`Mc1Models.cpp`) and the
  computer wizard (class 3 action 1, `Mc1ComputerWizard.cpp`); the HW functions that differ are set by
  `PatchClassTablesHw`. A random input casting all spells (cheat) matches DOSBox in MC1 levels 1, 2 (with the
  computer wizard), 4, 8, 14 and 20 (entities, players, maps, the screen; level 20 one sprite of the 3D view differs
  in 164 pixels) and in HW levels 0 and 12 (the state).
  The levels 17 and 39 fill all 1000 entities while loading: the original goes on with the wizard of player 0 at
  DOS address 0 and the entity index FC3Ah (marked `//!!!!!`). The port does the same: `EntityAtAddressZero` lies on
  a copy of the DOS memory 0-FFFh (the interrupt table as in DOSBox, `ResetDosAddressZero`), the entities are
  allocated for 10000h indexes and the index FC3Ah reads a zero entity whose pointers point to that copy.
- The number of the entities: `mc1EntityLimit` in `config.json` (game, also in the configurator), 1000 the original,
  default and at most 32767 (int16 indexes). `mc1Entities` / `mc1EntityCount` (not the array of the state); the
  slots over 1000 have their own free list taken only when the original has no free and no reusable entity, so up to
  1000 entities nothing changes. The regression tests and `REMC1_PLAY` always run with 1000 and without the real
  keyboard, mouse and joystick (`sdlInputLocked`; dosbox-x-remc1 drops them with `REMC1_PLAY` / `REMC1_DUMP`).
  The save of the game (not ported yet) should keep the original format while only the slots under 1000 are used.
- The sounds of the game (`Mc1Sound.cpp`: 55370 the requests by the distance and the angle, 55100, the fades of the
  flight loops, 20D00 the music of the danger) run only without `-custom`; checked only that they leave the state
  equal to DOSBox (forced on) and queue sensible requests, not by listening.
- What is still missing in the game (stubs in `remc1/engine/Mc1NotPorted.cpp`, logged once when called):
  - the debug texts (5A560); the panel, the spell icons, the frame of the joystick cell and the players table of
    the map (22880, checked with the mouse poked to the bottom) are in `Mc1Panel.cpp`;
  - the save / load in the game (3E690, 3E750, 3E7F0, 3E8C0 - they save the whole state with pointers, needs a pointer free shadow state like remc2
    `Convert_from_shadow_*`), the network, the VFX1 head tracker, the joystick calibration;
- Regression tests of MC1 and HW (in remc2-regression-test with the MC2 ones, own folders):
  `memimages/regressions-mc1/level<NNN>` and `memimages/regressions-mc1hw/level<NNN>` (NNN = the level + 1), each with
  `input.rec` (the input of player 0, 10 bytes a frame) and `reference.binz` (the state of DOSBox every 100 frames:
  the seed, the entities without their pointers, the players, the settings with the entity lists as DOS addresses,
  the terrain maps; "MC1REGZ1", LzCompress of each record XOR the previous one). `remc2-regression-test --mc1 N` /
  `--mc1hw N` runs one (`Mc1Regression.cpp` compares before DrawFrame_20FB0 and ends by End_thread), without a
  selection they run in parallel with the MC2 tests. `remc1/tools/mkregression.py <mc1|mc1hw> <levels...>` makes them
  (dosbox-x-remc1 `REMC1_DUMP_EVERY`, then `remc2-regression-test --pack_mc1`); the input is a random flight with all
  spells (the cheat commands 30), `--input idle` only joins the player; `--seed`, `--spells` choose the input,
  `--no_render` runs the original without the 3D view (needed for HW: HIDDEN.EXE overwrites its tmap cache and
  divides by a frame of height 0; the test ignores settings +95h, set by the 3D view only). A crashed original gives
  no reference. All 50 MC1 levels and 25 HW levels match DOSBox for 2000 frames. The build copies `config.json` to
  `x64/Release`, so no build while tests of the port run.
- Verification of a played game: `remc1/tools/dbxrecord.py <level> <file>` records the input of player 0 in DOSBox,
  the port plays it with the env `REMC1_PLAY=<file>`; DOSBox plays it too (`dbxrecord.py ... play`), so the state can
  be dumped at any frame in both (DOSBox `REMC1_DUMP` at 20FB0 with `REMC1_DUMP_HIT=<frame>`, the port with a
  temporary hook before DrawFrame_20FB0) and compared (entities state+7463h, players state+340Bh, the maps CC1E0,
  DC1E0, FC1E0; the work screen is at linear 6D080h, 320x200 in the default video mode 1). The input can also be generated: the command 30 with param 1 gives all spells, 21 / 22 select the
  spells of the buttons, the buttons 1 forward, 2 back, 4 / 8 aside, 10h / 20h the spells, moveX / moveY the mouse.

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
    the port;
  - `REMC1_RECORD=<code>,<pointer>,<offset>,<length>,<file>` appends the bytes at [pointer] + offset each time the
    code address runs, `REMC1_PLAY` (the same arguments) writes them back from the file. The input of player 0:
    `20D9D0,26C400,7413,A,<file>` (3C9D0, state pointer AE400, playerInputs_0x7413, 10 bytes a frame). The port plays
    such a file with the env `REMC1_PLAY=<file>` (PlayerEvents_3C9D0), so a game played in DOSBox
    (`CARPET.EXE -custom -level N`) can be compared frame by frame.
- The original game files: the MC1 CD content (also `x64/Release/MC1/` of remc2) as drive C:, a conf with
  `cputype=pentium`, `core=normal` and the autoexec `CARPET.EXE -custom -level N` (`tools/mkconf.py` makes it from
  `vs2015/dosbox.conf`).

## Tools (`remc1/tools/`)

Python 3 and Git Bash; some scripts still have the paths of the original machine (`C:/prenos/remc2-dev2/remc2`,
`c:\prenos\dosbox-x-remc1`) - change them at the top. The test outputs are written next to the scripts.

- `dbxdump.py <mc1|hw> <code IDA> <outdir> name=<IDA data>:<len> | name=@<linear>:<len> ...` - runs DOSBox and
  dumps memory at the code address (env `DBXCONF` = the conf, `REMC1_DUMP_HIT`, `REMC1_POKE` pass through).
- `mkconf.py "<CARPET.EXE args>" <conf>` - a DOSBox conf for a level.
- `dbxrecord.py <level> <out.rec> [play]` - plays the level in DOSBox and records the input of player 0 (or plays
  the file); the port plays it with the env `REMC1_PLAY=<out.rec>` and `-game mc1 -custom -level <level>`.
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

1. Play level 1 in DOSBox with `dbxrecord.py`, replay it in the port and fix the first differing frame (bisect).
2. Listen to the sounds of the game (New game: `-custom` turns the sound off in the
   original, so the recordings have none).
3. The other spells and creatures (by the levels), the computer wizard 13170.
4. Save / load with a shadow state.
5. HW renderer: compare visually (a GPU frame cannot be dumped), the motion blur.
