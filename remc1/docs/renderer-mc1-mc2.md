# Renderer: MC1 vs MC2

Comparison of the world drawing tree of CARPET.EXE (MC1) and NETHERW.EXE (MC2), IDA addresses.
`body` = similarity of normalized instructions, `opcodes` = similarity of the opcode sequence.
Listings and maps: `c:\prenos\dosbox-x-remc1\ida\`. HIDDEN.EXE functions are in `mc1_hw_map.txt`.

## Tree

| MC1 (CARPET) | lines | MC2 (remc2 name) | lines | body | opcodes | note |
|---|---|---|---|---|---|---|
| sub_30D90 | 657 | DrawWorld_411A0 | 469 | 0.45 | 0.58 | same role |
| sub_79495 | 28 | SetRenderViewPortSize_BCD45 | 28 | 1.00 | 1.00 | identical |
| sub_2A700 | 1978 | DrawTerrainAndParticles_3C080 | 2589 | 0.29 | 0.56 | same role, biggest difference |
| sub_29C30 | 670 | SetBillboards_3B560 | 701 | 0.66 | 0.71 | same role |
| sub_410CE | 30 | sub_7277A | 30 | 1.00 | 1.00 | identical |
| sub_30730 | 158 | DrawSky_40950 | 173 | 0.46 | 0.78 | same role |
| sub_729A3 | 7339 | DrawTriangleInProjectionSpace_B6253 | 7339 | 0.99 | 1.00 | identical (only jump table labels differ) |
| sub_2FC50 | 743 | DrawTileSpriteReflections_3FD60 | 808 | 0.61 | 0.83 | same role |
| sub_2DCB0 | 1374 | DrawSprites_3E360 | 1718 | 0.43 | 0.50 | same role |
| sub_2F170 | 705 | - | | 0.25 | 0.50 | MC1 only, one more pass over the sprites (to check: shadows?) |
| sub_2C410 | 1882 | DrawSprite_41BD3 | 2093 | 0.37 | 0.69 | same role; MC2 also draws names and health bars (88740, 2CB30) |
| sub_724C0 | 106 | sub_B5C60 | 106 | 1.00 | 1.00 | identical |
| sub_59050 | 47 | MainInitTmaps_71520 | 49 | 0.81 | 0.90 | tmap cache, similar tree below |

MC1 tree: 20955 asm lines, MC2 tree: 23099.

## Findings

- The structure is the same: DrawWorld -> DrawTerrainAndParticles -> billboards, sky, rasterizer,
  sprites, reflections; DrawSprite at the bottom; the tmap cache below.
- Identical and shared directly: viewport, rasterizer, sub_410CE/sub_7277A, sub_724C0/sub_B5C60.
- Same role, different code: the rest. The differences have to be read function by function; the biggest
  one is DrawTerrainAndParticles (0.29).
- MC1 has one extra sprite pass (sub_2F170); MC2 has names/health bars over wizards in DrawSprite.

## Plan

1. Scene interface: the renderers (Original, HD, NG, HW) stop reading `world_D41A0` and the other MC2
   globals directly and get camera, terrain, sprites and sky from the game.
2. MC1 differences go into the renderer as variants (sky, billboards, extra pass, terrain loop),
   so MC1 gets all four renderers.
3. Each step is checked against both MC1 asm listings and, for MC2, with the regression tests.
