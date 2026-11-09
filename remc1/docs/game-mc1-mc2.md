# Game: MC1 vs MC2

Comparison of the in-game code of CARPET.EXE (MC1) with NETHERW.EXE (MC2), IDA addresses, as the base for the
decisions what to share with remc2 and what to port separately. Per function list: `game-mc1-mc2.tsv`
(MC1 function, asm lines, how the pair was found, MC2 function / remc2 name, `seq`, `bag`, already ported).

## Method

- Roots: the game loop `sub_34610` and what the top procedure calls around it (`407A0` level load, `408D0` level
  end, `34460` loading screen, `448E0` after the level, `4E5B0` menu case 5, `59720`). Functions already ported in
  remc1 and their subtrees are left out.
- Pairs: `mc1_mc2_map.txt` (identical / similar) plus role pairs found by aligning the call lists of paired parents;
  seeds `34610/47320`, `34530/47560` (game loop), `20FB0/2BE30` (DrawGameFrame) and the renderer pairs of
  `renderer-mc1-mc2.md`.
- `seq` = similarity of the instruction sequence after normalizing registers, stack frames and saved registers
  away (the two executables were compiled with a different register allocation; e.g. `33500 / 45AA0` differs only
  by `ebp` frames). `bag` = the same, order ignored. `seq >= 0.9` means the same logic in practice.

## Subsystems (asm lines)

| subsystem | funcs | asm lines | seq >= 0.9 | 0.6-0.9 | < 0.6 | no MC2 pair |
|---|---|---|---|---|---|---|
| renderer (DrawWorld 30D90) | 32 | 16815 | 7887 | 2728 | 6200 | 0 |
| level load (407A0) | 69 | 5291 | 1551 | 1841 | 873 | 1026 |
| level end (408D0) | 1 | 60 | 0 | 0 | 0 | 60 |
| loading screen (34460) | 12 | 585 | 513 | 40 | 0 | 32 |
| after the level (448E0) | 1 | 177 | 0 | 0 | 0 | 177 |
| menu case 5 (4E5B0) | 78 | 6657 | 1411 | 1097 | 1205 | 2944 |
| level start / palette (34690) | 3 | 294 | 0 | 0 | 294 | 0 |
| input (17C20) | 43 | 5255 | 736 | 340 | 1502 | 2677 |
| world update (3C9D0) | 54 | 4915 | 515 | 629 | 2953 | 818 |
| entities (41780) | 4 | 552 | 0 | 0 | 441 | 111 |
| sound events (55100) | 9 | 565 | 0 | 0 | 307 | 258 |
| 2D UI of the frame (20FB0 without the renderer) | 32 | 6135 | 449 | 65 | 3546 | 2075 |
| debug text (5A560) | 2 | 560 | 0 | 0 | 560 | 0 |
| blit (30B30) | 5 | 402 | 260 | 142 | 0 | 0 |
| rest of the loop (34610) | 12 | 472 | 299 | 0 | 114 | 59 |

## Findings

- Renderer: the bottom is the same code (`729A3` / `DrawTriangleInProjectionSpace_B6253` 7339 lines 0.99,
  `60D65` / `DrawBitmapFinal_8F935` 1.00); the layers above are similar (DrawTerrainAndParticles 0.55,
  DrawSprites 0.53, DrawSprite 0.49, reflections 0.73, sky 0.79, billboards 0.64, DrawWorld 0.68).
- Level generation: the terrain passes are the same logic (TruncTerrainHeight, SetMax4Tiles, ApplySnowLine,
  MarkFlatLowlandClass2, RemoveIsolatedTerrainClasses ... 0.90-1.00), only the order / set of passes in
  `31AA0` / `GenerateLevelMap_43830` differs (0.52). The level maps are the same arrays as in MC2
  (`CC1E0` mapTerrainType_10B4E0, `DC1E0` mapHeightmap_11B4E0, `EC1E0` mapShading_12B4E0, `FC1E0` mapAngle_13B4E0,
  `10C1E0` mapEntityIndex_15B4E0, cleared in the same order by `40550` / `ClearSettings_567C0`).
- Game logic (input, world update, entities, sound events, 2D UI): mostly below 0.6 or without an MC2 pair -
  MC1 has its own code there, ported from the MC1 asm.
- Structures: the MC1 entity is 0xA4 bytes (MC2 0xA8), 1000 of them at state + 0x7463 (MC2 0x6E8E); the level
  data at state + 0x2F503. The MC1 state needs its own types; the shared remc2 functions need the same data
  (e.g. the terrain passes read the level parameters and the maps).
- Save in the game: the state contains pointers - saved through shadow structures with the 32-bit DOS layout and
  converted to the real pointers and back, as remc2 does (`Convert_from_shadow_D41A0_BYTESTR_0`).
- MC1 renderer extras: an extra sprite pass (`2F170`) and the 3D modes of MC1 (stereo / VFX1 output) - to be added
  to the renderer later, not part of the first game milestone.

## Level map (31AA0 / GenerateLevelMap_43830) - done

- Shared with remc2 (the normalized diff has only compiler differences, then checked against DOSBox):
  DecompressTerrainMapLevel, TruncTerrainHeight, MarkLandAndRivers (+ SmoothTiles), SetMax4Tiles, ApplySnowLine,
  MarkFlatLowland, SpreadTerrainClass4, MarkFlatLowlandClass2, RemoveIsolatedTerrainClasses, AddRivers.
- MC1 own: GenerateLevelMap (order, no caves, scanned.rmd), SmoothTerrain 31BB0, MarkWaterEdges 31D40,
  SetTerrainAngles 32560, ComputeTerrainShading 329C0, MarkSteepTerrain 33180 (HW marks 1), HW MarkFlatHighland 31C10.
- Check: the five maps, the seed and the angle table after 31AA0 equal DOSBox byte for byte, MC1 and HW,
  levels 0, 3, 10, 20 (`-custom -level N`, dump at the entry of 371C0 / 37580).

## Small differences to discuss (a switch in remc2 could share them)

| MC1 | remc2 | difference |
|---|---|---|
| ComputeTerrainShading 329C0 | ComputeTerrainShading_44D00 | MC2 has one more branch (caves) |
| MarkSteepTerrain 33180 | MarkSteepTerrain_45600 | the mark value (MC1 6, HW 1) and the neighbour rule |
| SmoothTerrain 31BB0 | SmoothTerrain_43970 + GetSmoothedHeight_439A0 | MC1 inlined, to compare |
| MarkWaterEdges 31D40 | MarkWaterEdges_43D50 | 0.79, to compare |
| GenerateLevelMap 31AA0 | GenerateLevelMap_43830 | order of the passes, MarkFlatHighland only in HW / MC2 |

## Open decisions

1. Renderer: use the MC2 renderer (Original/HD/NG/HW) with MC1 variants, or port the MC1 one - after this analysis.
2. Same-logic functions (terrain passes, loading screen, blit ...): call the remc2 functions (constexpr pointers in
   `Mc1Shared.h`), which needs MC1 to keep the data they use in the remc2 structures, or port them for the MC1
   structures.
