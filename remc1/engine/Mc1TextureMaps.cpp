#include "Mc1TextureMaps.h"

#include <cstring>
#include <string>

#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1Install.h"
#include "Mc1Blocks.h"
#include "Mc1System.h"
#include <algorithm>
#include "../../remc2/utilities/DataFileIO.h"
#include "../../remc2/utilities/DataFileRNC.h"

Type_Mc1SpriteDef_99BA0 spriteDefs_99BA0_99BA0[286] = {
	{ 8, "DRAW_SCALED_ARCHER", 0, 250, 0, 1, 0 },
	{ 11, "DRAW_SCALED_ARCHER_SHOOT", 0, 250, 0, 1, 0 },
	{ 46, "DRAW_SCALED_BADGUY", 0, 150, 0, 1, 0 },
	{ 38, "DRAW_SCALED_BEE", 0, 150, 0, 1, 0 },
	{ 38, "DRAW_SCALED_BLANK1", 0, 150, 0, 1, 0 },
	{ 38, "DRAW_SCALED_BLANK2", 0, 150, 0, 1, 0 },
	{ 38, "DRAW_SCALED_BLANK3", 0, 150, 0, 1, 0 },
	{ 94, "DRAW_SCALED_BOOM1", 0, 250, 0, -1, 0 },
	{ 117, "DRAW_SCALED_BLACK_ROCK", 0, 150, 0, -1, 0 },
	{ 127, "DRAW_SCALED_BLACK_SMOKE1", 0, 50, 2, 1, 0 },
	{ 127, "DRAW_SCALED_BLACK_SMOKE2", 0, 100, 2, 1, 0 },
	{ 127, "DRAW_SCALED_BLACK_SMOKE3", 0, 150, 2, 1, 0 },
	{ 127, "DRAW_SCALED_BLACK_SMOKE4", 0, 200, 2, 1, 0 },
	{ 127, "DRAW_SCALED_BLACK_SMOKE5", 0, 250, 2, 1, 0 },
	{ 127, "DRAW_SCALED_BLACK_SMOKE6", 0, 300, 2, 1, 0 },
	{ 127, "DRAW_SCALED_BLACK_SMOKE7", 0, 350, 2, 1, 0 },
	{ 127, "DRAW_SCALED_BLACK_SMOKE8", 0, 400, 2, 1, 0 },
	{ 139, "DRAW_SCALED_BLOOD_SPLAT", 0, 150, 0, 1, 0 },
	{ 118, "DRAW_SCALED_BLUE_BALL", 0, 150, 0, 1, 0 },
	{ 112, "DRAW_SCALED_BODY1", 0, 150, 0, -1, 0 },
	{ 112, "DRAW_SCALED_BODY2", 0, 165, 0, 1, 0 },
	{ 112, "DRAW_SCALED_BODY3", 0, 185, 0, 1, 0 },
	{ 112, "DRAW_SCALED_BODY4", 0, 200, 0, 1, 0 },
	{ 112, "DRAW_SCALED_BODY5", 0, 210, 0, 1, 0 },
	{ 112, "DRAW_SCALED_BODY6", 0, 210, 0, 1, 0 },
	{ 112, "DRAW_SCALED_BODY7", 0, 200, 0, 1, 0 },
	{ 112, "DRAW_SCALED_BODY8", 0, 190, 0, 1, 0 },
	{ 112, "DRAW_SCALED_BODY9", 0, 170, 0, 1, 0 },
	{ 112, "DRAW_SCALED_BODY10", 0, 160, 0, 1, 0 },
	{ 112, "DRAW_SCALED_BODY11", 0, 150, 0, 1, 0 },
	{ 112, "DRAW_SCALED_BODY12", 0, 130, 0, 1, 0 },
	{ 112, "DRAW_SCALED_BODY13", 0, 110, 0, 1, 0 },
	{ 112, "DRAW_SCALED_BODY14", 0, 90, 0, 1, 0 },
	{ 112, "DRAW_SCALED_BODY15", 0, 70, 0, 1, 0 },
	{ 112, "DRAW_SCALED_BODY16", 0, 60, 0, 1, 0 },
	{ 141, "DRAW_SCALED_DEAD_TREE", 0, 300, 0, 3, 0 },
	{ 143, "DRAW_SCALED_DIRT1", 0, 150, 0, 1, 0 },
	{ 144, "DRAW_SCALED_DIRT2", 0, 150, 0, 1, 0 },
	{ 145, "DRAW_SCALED_DIRT3", 0, 150, 0, 1, 0 },
	{ 147, "DRAW_SCALED_DOLMEN1", 0, 400, 0, 1, 0 },
	{ 54, "DRAW_SCALED_DRAGON", 0, 200, 0, 1, 0 },
	{ 110, "DRAW_SCALED_EXPLODE", 0, 250, 0, -1, 0 },
	{ 111, "DRAW_SCALED_FIRBALL", 0, 100, 0, -1, 0 },
	{ 151, "DRAW_SCALED_FIRE_FALL", 0, 150, 0, -1, 0 },
	{ 0, "DRAW_SCALED_FLYER1", 0, 200, 0, -1, 0 },
	{ 124, "DRAW_SCALED_GREEN", 0, 150, 0, -1, 0 },
	{ 125, "DRAW_SCALED_GREY_BALL", 0, 150, 0, -1, 0 },
	{ 14, "DRAW_SCALED_GRIFFON", 0, 300, 0, 1, 0 },
	{ 149, "DRAW_SCALED_HOUSE1", 0, 100, 0, 1, 0 },
	{ 22, "DRAW_SCALED_KRAKEN", 0, 300, 0, 1, 0 },
	{ 119, "DRAW_SCALED_KRAKEN_BODY", 0, 100, 0, 1, 0 },
	{ 129, "DRAW_SCALED_LIGHTNING_SPARK", 0, 150, 0, -1, 0 },
	{ 115, "DRAW_SCALED_MANA_GEM", 0, 50, 0, -1, 0 },
	{ 115, "DRAW_SCALED_MANA_GEM", 0, 100, 0, 1, 0 },
	{ 115, "DRAW_SCALED_MANA_GEM", 0, 150, 0, 1, 0 },
	{ 115, "DRAW_SCALED_MANA_GEM", 0, 200, 0, 1, 0 },
	{ 115, "DRAW_SCALED_MANA_GEM", 0, 250, 0, 1, 0 },
	{ 115, "DRAW_SCALED_MANA_GEM", 0, 300, 0, 1, 0 },
	{ 115, "DRAW_SCALED_MANA_GEM", 0, 350, 0, 1, 0 },
	{ 115, "DRAW_SCALED_MANA_GEM", 0, 400, 0, 1, 0 },
	{ 152, "DRAW_SCALED_PALM_TREE1", 0, 150, 0, 4, 0 },
	{ 153, "DRAW_SCALED_PALM_TREE2", 0, 150, 0, 4, 0 },
	{ 122, "DRAW_SCALED_RAIN_CLOUD", 0, 150, 0, 1, 0 },
	{ 78, "DRAW_SCALED_RAVEN", 0, 100, 0, 1, 0 },
	{ 126, "DRAW_SCALED_RED_ROCK", 0, 150, 0, 1, 0 },
	{ 150, "DRAW_SCALED_RIP", 0, 200, 0, -1, 0 },
	{ 148, "DRAW_SCALED_SHIELD", 0, 200, 0, 1, 0 },
	{ 114, "DRAW_SCALED_SMOKE1", 0, 50, 2, -1, 0 },
	{ 114, "DRAW_SCALED_SMOKE2", 0, 100, 2, 1, 0 },
	{ 114, "DRAW_SCALED_SMOKE3", 0, 150, 2, 1, 0 },
	{ 114, "DRAW_SCALED_SMOKE4", 0, 200, 2, 1, 0 },
	{ 114, "DRAW_SCALED_SMOKE5", 0, 250, 2, 1, 0 },
	{ 114, "DRAW_SCALED_SMOKE6", 0, 300, 2, 1, 0 },
	{ 114, "DRAW_SCALED_SMOKE7", 0, 350, 2, 1, 0 },
	{ 114, "DRAW_SCALED_SMOKE8", 0, 400, 2, 1, 0 },
	{ 70, "DRAW_SCALED_SNAKE", 0, 150, 0, 1, 0 },
	{ 128, "DRAW_SCALED_SNAKE_BODY", 0, 350, 0, 1, 0 },
	{ 116, "DRAW_SCALED_SPELL_GEM", 0, 150, 0, -1, 0 },
	{ 113, "DRAW_SCALED_STEAM1", 0, 150, 3, -1, 0 },
	{ 146, "DRAW_SCALED_STONE1", 0, 400, 0, -1, 0 },
	{ 155, "DRAW_SCALED_STONE2_DEAD", 0, 150, 0, -1, 0 },
	{ 154, "DRAW_SCALED_STONE2_PLAIN", 0, 150, 0, -1, 0 },
	{ 123, "DRAW_SCALED_THUNDER_CLOUD", 0, 150, 0, 1, 0 },
	{ 140, "DRAW_SCALED_TREE1", 0, 300, 0, 4, 0 },
	{ 156, "DRAW_SCALED_TREE2", 0, 300, 0, 4, 0 },
	{ 86, "DRAW_SCALED_TROLL", 0, 250, 0, 1, 0 },
	{ 30, "DRAW_SCALED_VULTURE", 0, 100, 0, 1, 0 },
	{ 62, "DRAW_SCALED_WALK", 0, 150, 0, 1, 0 },
	{ 62, "DRAW_SCALED_WORM", 0, 150, 0, 1, 0 },
	{ 130, "DRAW_SCALED_WORM_BODY1", 0, 150, 0, 1, 0 },
	{ 130, "DRAW_SCALED_WORM_BODY2", 0, 165, 0, 1, 0 },
	{ 130, "DRAW_SCALED_WORM_BODY3", 0, 185, 0, 1, 0 },
	{ 130, "DRAW_SCALED_WORM_BODY4", 0, 200, 0, 1, 0 },
	{ 130, "DRAW_SCALED_WORM_BODY5", 0, 210, 0, 1, 0 },
	{ 130, "DRAW_SCALED_WORM_BODY6", 0, 210, 0, 1, 0 },
	{ 130, "DRAW_SCALED_WORM_BODY7", 0, 200, 0, 1, 0 },
	{ 130, "DRAW_SCALED_WORM_BODY8", 0, 190, 0, 1, 0 },
	{ 130, "DRAW_SCALED_WORM_BODY9", 0, 170, 0, 1, 0 },
	{ 130, "DRAW_SCALED_WORM_BODY10", 0, 160, 0, 1, 0 },
	{ 130, "DRAW_SCALED_WORM_BODY11", 0, 150, 0, 1, 0 },
	{ 130, "DRAW_SCALED_WORM_BODY12", 0, 130, 0, 1, 0 },
	{ 130, "DRAW_SCALED_WORM_BODY13", 0, 110, 0, 1, 0 },
	{ 130, "DRAW_SCALED_WORM_BODY14", 0, 90, 0, 1, 0 },
	{ 130, "DRAW_SCALED_WORM_BODY15", 0, 70, 0, 1, 0 },
	{ 130, "DRAW_SCALED_WORM_BODY16", 0, 60, 0, 1, 0 },
	{ 131, "DRAW_SCALED_MANA_GEM_01", 0, 50, 0, -1, 0 },
	{ 131, "DRAW_SCALED_MANA_GEM_01", 0, 100, 0, 1, 0 },
	{ 131, "DRAW_SCALED_MANA_GEM_01", 0, 150, 0, 1, 0 },
	{ 131, "DRAW_SCALED_MANA_GEM_01", 0, 200, 0, 1, 0 },
	{ 131, "DRAW_SCALED_MANA_GEM_01", 0, 250, 0, 1, 0 },
	{ 131, "DRAW_SCALED_MANA_GEM_01", 0, 300, 0, 1, 0 },
	{ 131, "DRAW_SCALED_MANA_GEM_01", 0, 350, 0, 1, 0 },
	{ 131, "DRAW_SCALED_MANA_GEM_01", 0, 400, 0, 1, 0 },
	{ 132, "DRAW_SCALED_MANA_GEM_02", 0, 50, 0, -1, 0 },
	{ 132, "DRAW_SCALED_MANA_GEM_02", 0, 100, 0, 1, 0 },
	{ 132, "DRAW_SCALED_MANA_GEM_02", 0, 150, 0, 1, 0 },
	{ 132, "DRAW_SCALED_MANA_GEM_02", 0, 200, 0, 1, 0 },
	{ 132, "DRAW_SCALED_MANA_GEM_02", 0, 250, 0, 1, 0 },
	{ 132, "DRAW_SCALED_MANA_GEM_02", 0, 300, 0, 1, 0 },
	{ 132, "DRAW_SCALED_MANA_GEM_02", 0, 350, 0, 1, 0 },
	{ 132, "DRAW_SCALED_MANA_GEM_02", 0, 400, 0, 1, 0 },
	{ 133, "DRAW_SCALED_MANA_GEM_03", 0, 50, 0, -1, 0 },
	{ 133, "DRAW_SCALED_MANA_GEM_03", 0, 100, 0, 1, 0 },
	{ 133, "DRAW_SCALED_MANA_GEM_03", 0, 150, 0, 1, 0 },
	{ 133, "DRAW_SCALED_MANA_GEM_03", 0, 200, 0, 1, 0 },
	{ 133, "DRAW_SCALED_MANA_GEM_03", 0, 250, 0, 1, 0 },
	{ 133, "DRAW_SCALED_MANA_GEM_03", 0, 300, 0, 1, 0 },
	{ 133, "DRAW_SCALED_MANA_GEM_03", 0, 350, 0, 1, 0 },
	{ 133, "DRAW_SCALED_MANA_GEM_03", 0, 400, 0, 1, 0 },
	{ 134, "DRAW_SCALED_MANA_GEM_04", 0, 50, 0, -1, 0 },
	{ 134, "DRAW_SCALED_MANA_GEM_04", 0, 100, 0, 1, 0 },
	{ 134, "DRAW_SCALED_MANA_GEM_04", 0, 150, 0, 1, 0 },
	{ 134, "DRAW_SCALED_MANA_GEM_04", 0, 200, 0, 1, 0 },
	{ 134, "DRAW_SCALED_MANA_GEM_04", 0, 250, 0, 1, 0 },
	{ 134, "DRAW_SCALED_MANA_GEM_04", 0, 300, 0, 1, 0 },
	{ 134, "DRAW_SCALED_MANA_GEM_04", 0, 350, 0, 1, 0 },
	{ 134, "DRAW_SCALED_MANA_GEM_04", 0, 400, 0, 1, 0 },
	{ 135, "DRAW_SCALED_MANA_GEM_05", 0, 50, 0, -1, 0 },
	{ 135, "DRAW_SCALED_MANA_GEM_05", 0, 100, 0, 1, 0 },
	{ 135, "DRAW_SCALED_MANA_GEM_05", 0, 150, 0, 1, 0 },
	{ 135, "DRAW_SCALED_MANA_GEM_05", 0, 200, 0, 1, 0 },
	{ 135, "DRAW_SCALED_MANA_GEM_05", 0, 250, 0, 1, 0 },
	{ 135, "DRAW_SCALED_MANA_GEM_05", 0, 300, 0, 1, 0 },
	{ 135, "DRAW_SCALED_MANA_GEM_05", 0, 350, 0, 1, 0 },
	{ 135, "DRAW_SCALED_MANA_GEM_05", 0, 400, 0, 1, 0 },
	{ 136, "DRAW_SCALED_MANA_GEM_06", 0, 50, 0, -1, 0 },
	{ 136, "DRAW_SCALED_MANA_GEM_06", 0, 100, 0, 1, 0 },
	{ 136, "DRAW_SCALED_MANA_GEM_06", 0, 150, 0, 1, 0 },
	{ 136, "DRAW_SCALED_MANA_GEM_06", 0, 200, 0, 1, 0 },
	{ 136, "DRAW_SCALED_MANA_GEM_06", 0, 250, 0, 1, 0 },
	{ 136, "DRAW_SCALED_MANA_GEM_06", 0, 300, 0, 1, 0 },
	{ 136, "DRAW_SCALED_MANA_GEM_06", 0, 350, 0, 1, 0 },
	{ 136, "DRAW_SCALED_MANA_GEM_06", 0, 400, 0, 1, 0 },
	{ 137, "DRAW_SCALED_MANA_GEM_07", 0, 50, 0, -1, 0 },
	{ 137, "DRAW_SCALED_MANA_GEM_07", 0, 100, 0, 1, 0 },
	{ 137, "DRAW_SCALED_MANA_GEM_07", 0, 150, 0, 1, 0 },
	{ 137, "DRAW_SCALED_MANA_GEM_07", 0, 200, 0, 1, 0 },
	{ 137, "DRAW_SCALED_MANA_GEM_07", 0, 250, 0, 1, 0 },
	{ 137, "DRAW_SCALED_MANA_GEM_07", 0, 300, 0, 1, 0 },
	{ 137, "DRAW_SCALED_MANA_GEM_07", 0, 350, 0, 1, 0 },
	{ 137, "DRAW_SCALED_MANA_GEM_07", 0, 400, 0, 1, 0 },
	{ 138, "DRAW_SCALED_MANA_GEM_08", 0, 50, 0, -1, 0 },
	{ 138, "DRAW_SCALED_MANA_GEM_08", 0, 100, 0, 1, 0 },
	{ 138, "DRAW_SCALED_MANA_GEM_08", 0, 150, 0, 1, 0 },
	{ 138, "DRAW_SCALED_MANA_GEM_08", 0, 200, 0, 1, 0 },
	{ 138, "DRAW_SCALED_MANA_GEM_08", 0, 250, 0, 1, 0 },
	{ 138, "DRAW_SCALED_MANA_GEM_08", 0, 300, 0, 1, 0 },
	{ 138, "DRAW_SCALED_MANA_GEM_08", 0, 350, 0, 1, 0 },
	{ 138, "DRAW_SCALED_MANA_GEM_08", 0, 400, 0, 1, 0 },
	{ 157, "DRAW_SCALED_BALLOON_01", 0, 800, 0, -16, 0 },
	{ 158, "DRAW_SCALED_BALLOON_02", 0, 800, 0, -16, 0 },
	{ 159, "DRAW_SCALED_BALLOON_03", 0, 800, 0, -16, 0 },
	{ 160, "DRAW_SCALED_BALLOON_04", 0, 800, 0, 1, 0 },
	{ 161, "DRAW_SCALED_BALLOON_05", 0, 800, 0, 1, 0 },
	{ 162, "DRAW_SCALED_BALLOON_06", 0, 800, 0, 1, 0 },
	{ 163, "DRAW_SCALED_BALLOON_07", 0, 800, 0, 1, 0 },
	{ 164, "DRAW_SCALED_BALLOON_08", 0, 800, 0, 1, 0 },
	{ 165, "DRAW_SCALED_FLAG_01", 0, 400, 0, -1, 0 },
	{ 166, "DRAW_SCALED_FLAG_02", 0, 400, 0, -1, 0 },
	{ 167, "DRAW_SCALED_FLAG_03", 0, 400, 0, -1, 0 },
	{ 168, "DRAW_SCALED_FLAG_04", 0, 400, 0, 1, 0 },
	{ 169, "DRAW_SCALED_FLAG_05", 0, 400, 0, 1, 0 },
	{ 170, "DRAW_SCALED_FLAG_06", 0, 400, 0, 1, 0 },
	{ 171, "DRAW_SCALED_FLAG_07", 0, 400, 0, 1, 0 },
	{ 172, "DRAW_SCALED_FLAG_08", 0, 400, 0, 1, 0 },
	{ 173, "DRAW_SCALED_CRAB1", 0, 80, 0, 1, 0 },
	{ 173, "DRAW_SCALED_CRAB2", 0, 140, 0, 1, 0 },
	{ 173, "DRAW_SCALED_CRAB3", 0, 200, 0, 1, 0 },
	{ 173, "DRAW_SCALED_CRAB4", 0, 280, 0, 1, 0 },
	{ 173, "DRAW_SCALED_CRAB5", 0, 360, 0, 1, 0 },
	{ 173, "DRAW_SCALED_CRAB6", 0, 440, 0, 1, 0 },
	{ 173, "DRAW_SCALED_CRAB7", 0, 520, 0, 1, 0 },
	{ 173, "DRAW_SCALED_CRAB8", 0, 600, 0, 1, 0 },
	{ 119, "DRAW_SCALED_KRAKEN_BODY2", 0, 50, 0, 1, 0 },
	{ 192, "DRAW_SCALED_RINGO", 0, 100, 0, 1, 0 },
	{ 193, "DRAW_SCALED_ARROW", 0, 60, 0, -1, 0 },
	{ 117, "DRAW_SCALED_SMALL_ROCK", 0, 100, 0, -1, 0 },
	{ 191, "DRAW_SCALED_WATER_BALL", 0, 100, 0, -1, 0 },
	{ 198, "DRAW_SCALED_TROLL_THROW", 0, 250, 0, 1, 0 },
	{ 181, "DRAW_SCALED_APE", 0, 400, 0, 1, 0 },
	{ 201, "DRAW_SCALED_GENIE", 0, 256, 0, 1, 0 },
	{ 209, "DRAW_SCALED_SKELETON", 0, 256, 0, 1, 0 },
	{ 212, "DRAW_SCALED_SKELETON_SHOOT", 0, 256, 0, 1, 0 },
	{ 215, "DRAW_SCALED_FLAME_ARROW", 0, 60, 0, -1, 0 },
	{ 220, "DRAW_SCALED_LIZARD", 0, 256, 0, 1, 0 },
	{ 228, "DRAW_SCALED_CRAB_EGG", 0, 256, 0, 1, 0 },
	{ 229, "DRAW_SCALED_ARCHER_SHOOT2", 0, 240, 0, 1, 0 },
	{ 232, "DRAW_SCALED_BIG_DRAGON", 0, 1500, 0, 1, 0 },
	{ 240, "DRAW_SCALED_EMU_RIDER", 0, 300, 0, 1, 0 },
	{ 260, "DRAW_SCALED_POCESSION", 0, 150, 3, -1, 0 },
	{ 261, "DRAW_SCALED_VOLCANO", 0, 150, 0, -1, 0 },
	{ 262, "DRAW_SCALED_CRATER", 0, 150, 0, -1, 0 },
	{ 263, "DRAW_SCALED_TELEPORT", 0, 150, 0, -1, 0 },
	{ 264, "DRAW_SCALED_RUBBERBAND", 0, 150, 0, -1, 0 },
	{ 265, "DRAW_SCALED_STEALMANA", 0, 150, 0, -1, 0 },
	{ 266, "DRAW_SCALED_WATERSHOT", 0, 150, 0, -1, 0 },
	{ 267, "DRAW_SCALED_LIGHTNING", 0, 50, 3, -1, 0 },
	{ 248, "DRAW_SCALED_CIVVY", 0, 250, 0, 1, 0 },
	{ 251, "DRAW_SCALED_WOMEN", 0, 250, 0, 1, 0 },
	{ 254, "DRAW_SCALED_MERCHANT", 0, 250, 0, 1, 0 },
	{ 276, "DRAW_SCALED_MAGIC_FLAME", 0, 250, 0, 1, 0 },
	{ 257, "DRAW_SCALED_BUILDER", 0, 250, 0, 1, 0 },
	{ 277, "DRAW_SCALED_LIGHTNING_HIT", 0, 250, 0, 1, 0 },
	{ 286, "DRAW_SCALED_TELEPORT_RING1", 0, 500, 0, 1, 0 },
	{ 286, "DRAW_SCALED_TELEPORT_RING2", 0, 500, 3, 1, 0 },
	{ 286, "DRAW_SCALED_TELEPORT_RING3", 0, 500, 0, 1, 0 },
	{ 299, "DRAW_SCALED_BURNT_TREE1", 0, 300, 0, 4, 0 },
	{ 300, "DRAW_SCALED_BURNT_TREE2", 0, 300, 0, 4, 0 },
	{ 142, "DRAW_SCALED_FLAME1", 0, 50, 0, -1, 0 },
	{ 142, "DRAW_SCALED_FLAME2", 0, 100, 0, 1, 0 },
	{ 142, "DRAW_SCALED_FLAME3", 0, 150, 0, 1, 0 },
	{ 142, "DRAW_SCALED_FLAME4", 0, 200, 0, 1, 0 },
	{ 142, "DRAW_SCALED_FLAME5", 0, 250, 0, 1, 0 },
	{ 142, "DRAW_SCALED_FLAME6", 0, 300, 0, 1, 0 },
	{ 142, "DRAW_SCALED_FLAME7", 0, 350, 0, 1, 0 },
	{ 142, "DRAW_SCALED_FLAME8", 0, 400, 0, 1, 0 },
	{ 287, "DRAW_SCALED_OLD_MAN", 0, 250, 0, 1, 0 },
	{ 290, "DRAW_SCALED_SKELINI", 0, 250, 0, 1, 0 },
	{ 270, "DRAW_SCALED_GOAT", 0, 250, 0, 1, 0 },
	{ 273, "DRAW_SCALED_SMALL_BOY", 0, 250, 0, 1, 0 },
	{ 301, "DRAW_SCALED_CAMPFIRE", 0, 250, 0, 1, 0 },
	{ 302, "DRAW_SCALED_CIVILIAN_GREEN", 0, 250, 0, 1, 0 },
	{ 305, "DRAW_SCALED_CIVILIAN_BROWN", 0, 250, 0, 1, 0 },
	{ 308, "DRAW_SCALED_PLASTER", 0, 250, 0, 1, 0 },
	{ 325, "DRAW_SCALED_SPLASH", 0, 300, 0, -1, 0 },
	{ 336, "DRAW_SCALED_SELETON_SIT", 0, 160, 0, -1, 0 },
	{ 337, "DRAW_SCALED_SKEL1", 0, 250, 0, 1, 0 },
	{ 340, "DRAW_SCALED_SKEL1_SHOOT", 0, 250, 0, 1, 0 },
	{ 343, "DRAW_SCALED_SKEL1_INIT", 0, 250, 0, 1, 0 },
	{ 352, "DRAW_SCALED_SKEL2", 0, 250, 0, 1, 0 },
	{ 355, "DRAW_SCALED_SKEL2_SHOOT", 0, 250, 0, 1, 0 },
	{ 358, "DRAW_SCALED_SKEL2_INIT", 0, 250, 0, 1, 0 },
	{ 367, "DRAW_SCALED_SKEL3", 0, 250, 0, 1, 0 },
	{ 370, "DRAW_SCALED_SKEL3_SHOOT", 0, 250, 0, 1, 0 },
	{ 373, "DRAW_SCALED_SKEL3_INIT", 0, 250, 0, 1, 0 },
	{ 382, "DRAW_SCALED_SKEL4", 0, 250, 0, 1, 0 },
	{ 385, "DRAW_SCALED_SKEL4_SHOOT", 0, 250, 0, 1, 0 },
	{ 388, "DRAW_SCALED_SKEL4_INIT", 0, 250, 0, 1, 0 },
	{ 397, "DRAW_SCALED_SKEL5", 0, 250, 0, 1, 0 },
	{ 400, "DRAW_SCALED_SKEL5_SHOOT", 0, 250, 0, 1, 0 },
	{ 403, "DRAW_SCALED_SKEL5_INIT", 0, 250, 0, 1, 0 },
	{ 412, "DRAW_SCALED_SKEL6", 0, 250, 0, 1, 0 },
	{ 415, "DRAW_SCALED_SKEL6_SHOOT", 0, 250, 0, 1, 0 },
	{ 418, "DRAW_SCALED_SKEL6_INIT", 0, 250, 0, 1, 0 },
	{ 427, "DRAW_SCALED_SKEL7", 0, 250, 0, 1, 0 },
	{ 430, "DRAW_SCALED_SKEL7_SHOOT", 0, 250, 0, 1, 0 },
	{ 433, "DRAW_SCALED_SKEL7_INIT", 0, 250, 0, 1, 0 },
	{ 442, "DRAW_SCALED_SKEL8", 0, 250, 0, 1, 0 },
	{ 445, "DRAW_SCALED_SKEL8_SHOOT", 0, 250, 0, 1, 0 },
	{ 448, "DRAW_SCALED_SKEL8_INIT", 0, 250, 0, 1, 0 },
	{ 457, "DRAW_SCALED_BAD_STONE", 0, 400, 0, 1, 0 },
	{ 458, "DRAW_SCALED_CIVVY2", 0, 250, 0, 1, 0 },
	{ 461, "DRAW_SCALED_BIG_LIGHT", 0, 250, 0, 1, 0 },
	{ 470, "DRAW_SCALED_FLYER2", 0, 200, 0, -1, 0 },
	{ 502, "DRAW_SCALED_FLYER3", 0, 200, 0, -1, 0 },
	{ 462, "DRAW_SCALED_FLYER4", 0, 200, 0, 1, 0 },
	{ 510, "DRAW_SCALED_FLYER5", 0, 200, 0, 1, 0 },
	{ 494, "DRAW_SCALED_FLYER6", 0, 200, 0, 1, 0 },
	{ 486, "DRAW_SCALED_FLYER7", 0, 200, 0, 1, 0 },
	{ 478, "DRAW_SCALED_FLYER8", 0, 200, 0, 1, 0 },
	{ 518, "DRAW_SCALED_BONUS_SPELL", 0, 150, 0, -1, 0 },
	{ 519, "DRAW_SCALED_BONES_SHOT", 0, 150, 0, 1, 0 },
	{ 520, "DRAW_SCALED_SECOND_FLAME", 0, 150, 0, 1, 0 },
	{ 527, "DRAW_SCALED_STEEL_HIT", 0, 150, 0, 1, 0 },
	{ 528, "DRAW_SCALED_RUBBER_HIT", 0, 150, 0, 1, 0 },
	{ 0, nullptr, 0, 0, 0, 0, 0 },
};

FILE* tmapsFile_968EC_968EC = nullptr;

static uint32_t TmapOffset(uint16_t index)
{
	// 10 byte records of the tmaps TAB, the DAT offset at +4
	uint32_t offset;
	memcpy(&offset, tmapsTab_12D744_12D734 + index * 10 + 4, sizeof(offset));
	return offset;
}

uint8_t OpenTmaps_587E0_58CF0()
{
	const char* name = mc1Variant == MC1_VARIANT_HW ? "tmaps1-0" : "tmaps0-0";
	// C:\carpet.cd\data first, then the CD
	std::string path = Mc1InstallPath() + "/data/" + name + ".dat";
	tmapsFile_968EC_968EC = DataFileIO::CreateOrOpenFile(path.c_str(), 0x200);
	if (!tmapsFile_968EC_968EC)
	{
		path = Mc1DataFilePath((std::string("data/") + name + ".dat").c_str());
		tmapsFile_968EC_968EC = DataFileIO::CreateOrOpenFile(path.c_str(), 0x200);
	}
	return 1;
}

int32_t ReadTmap_58860_58D70(uint16_t index, uint8_t* buffer)
{
	if (!tmapsFile_968EC_968EC)
		return -1;
	DataFileIO::Seek(tmapsFile_968EC_968EC, TmapOffset(index), 0);
	int32_t length = (int32_t)(TmapOffset(index + 1) - TmapOffset(index));
	DataFileIO::Read(tmapsFile_968EC_968EC, buffer, length); // the read length is not checked
	int32_t result = DecompressRnc_62B60_63070(buffer, buffer);
	if (result < 0)
	{
		printf("ERROR decompressing tmap%03d\n", index);
		return -2;
	}
	if (result == 0)
		return length;
	return result;
}

void CloseTmaps_58F70_59480()
{
	if (tmapsFile_968EC_968EC)
	{
		DataFileIO::Close(tmapsFile_968EC_968EC);
		tmapsFile_968EC_968EC = nullptr;
	}
}

void ProcessTmaps_58F90_594A0()
{
	OpenTmaps_587E0_58CF0();
	uint8_t* buffer = *workScreen_12EFF4_12EFE4;
	for (Type_Mc1SpriteDef_99BA0* def = spriteDefs_99BA0_99BA0; def->width_0x06_6 || def->height_0x08_8; def++)
	{
		if (ReadTmap_58860_58D70(def->textureIndex_0x00_0, buffer) == -1)
		{
			*(uint16_t*)(buffer + 2) = 255;
			*(uint16_t*)(buffer + 4) = 255;
			buffer[1] = 1;
		}
		uint16_t tmapWidth = *(uint16_t*)(buffer + 2);
		uint16_t tmapHeight = *(uint16_t*)(buffer + 4);
		// unlike MC2 a zero tmap size is not replaced, the division would fail
		if (def->width_0x06_6 == 0)
			def->width_0x06_6 = (uint16_t)((int32_t)(def->height_0x08_8 * tmapWidth) / (int32_t)tmapHeight);
		else if (def->height_0x08_8 == 0)
			def->height_0x08_8 = (uint16_t)((int32_t)(def->width_0x06_6 * tmapHeight) / (int32_t)tmapWidth);
		def->kind_0x0C_12 = buffer[1];
	}
	CloseTmaps_58F70_59480();
}

// 989C6: 0x22 byte records class, model, sprites to FFFFh (the records 5/4 and 5/10 run into the next one), the same in HW
static const int16_t modelSprites_989C6_989C6[697] = {
	5, 0, 19, 40, 42, 7, 244, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 1, 86, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 2, 3, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 3, 89, 88, 42, 7, 244, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 4, 0, 1, 206, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 5, 185, 42, 7, 244, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 6, 193, 50, 49, 216, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 7, 85, 198, 85, 196, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 8, 47, 42, 7, 244, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 9, 220, 237, 202, 201, 245, 201, 203, -1, 0, 0, 0, 0, 0, 0, 0,
	5, 10, 208, 194, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 11, 200, 42, 7, 244, 209, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 12, 221, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 13, 217, 218, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 14, 219, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 15, 0, 1, 206, 195, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 16, 207, 42, 7, 244, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 17, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 18, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	5, 19, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 9, 210, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 34, 212, 223, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 39, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 44, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	10, 52, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 0, 42, 7, 244, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 3, 209, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 6, 211, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 7, 76, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 8, 210, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 9, 211, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 10, 212, 223, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 11, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 12, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 13, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 14, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 15, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 16, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 17, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	12, 19, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	-1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

uint8_t usedTmapGroups_12E9E1_12E9D1[0x211];

void ClearUsedTmapGroups_59140_59650()
{
	memset(usedTmapGroups_12E9E1_12E9D1, 0, sizeof(usedTmapGroups_12E9E1_12E9D1));
}

void MarkModelSprites_59160_59670(uint16_t entityClass, int32_t model, int32_t sprite)
{
	// the last record of the class and model, else the sprite
	const int16_t* found = nullptr;
	for (const int16_t* record = modelSprites_989C6_989C6; record[0] >= 0; record += 17)
		if (record[0] == entityClass && record[1] == (uint16_t)model)
			found = record + 2;
	if (found)
	{
		for (; *found >= 0; found++)
			MarkSpriteTmapGroup_591E0_596F0(*found);
	}
	else if ((int16_t)sprite >= 0)
		MarkSpriteTmapGroup_591E0_596F0((int16_t)sprite);
}

void MarkSpriteTmapGroup_591E0_596F0(uint16_t sprite)
{
	// the group of the tmap (tab +8) gets the level sprite group of the sprite
	const Type_Mc1SpriteDef_99BA0& def = spriteDefs_99BA0_99BA0[sprite];
	uint16_t group;
	memcpy(&group, tmapsTab_12D744_12D734 + def.textureIndex_0x00_0 * 10 + 8, sizeof(group));
	usedTmapGroups_12E9E1_12E9D1[group] = (uint8_t)def.levelSpriteGroup_0x0B_11;
}

// the cache of the tmaps: InitMemory gives it its heap; the entries (14 bytes) and their order (by the place in the
// data) as MC2, 211h tmaps (MC2 1F8h)
type_x_DWORD_E9C28_str* tmapCache_AE3EC_AE3DC = nullptr;
type_E9C08* animations_AE3E0_AE3D0 = nullptr;
int32_t tmapLastUse_12CF00_12CEF0[0x211];
subtype_x_DWORD_E9C28_str* tmapEntries_12D748_12D738[0x211];
Type_Sprite** loadedTmaps_12DF8C_12DF7C[0x211];
uint8_t lockedTmaps_12E7D0_12E7C0[0x211];
uint8_t byte_968E8_968E8 = 0;

// 12D744: the records of tmaps?-0.tab (10 bytes: the size, the offset, the group = the first tmap of the group)
static uint32_t TmapSize(uint16_t tmap)
{
	uint32_t size;
	memcpy(&size, tmapsTab_12D744_12D734 + tmap * 10, sizeof(size));
	return size;
}

static uint16_t TmapGroup(uint16_t tmap)
{
	uint16_t group;
	memcpy(&group, tmapsTab_12D744_12D734 + tmap * 10 + 8, sizeof(group));
	return group;
}

type_x_DWORD_E9C28_str* InitTmapCache_36700_36AC0(uint32_t size, uint16_t count, uint8_t* buffer)
{
	type_x_DWORD_E9C28_str* cache;
	if (buffer)
	{
		// the header, the entries and their order at the start of the buffer, the data after them (DOS 1Ah + 14 and 4
		// bytes each; the port sizes of the structures as the MC2 port, 71B40)
		const uint32_t entries = count * sizeof(subtype_x_DWORD_E9C28_str);
		const uint32_t order = count * sizeof(subtype_x_DWORD_E9C28_str*);
		const uint32_t free = size - (entries + sizeof(type_x_DWORD_E9C28_str) + order);
		if (!free)
			return nullptr;
		cache = (type_x_DWORD_E9C28_str*)buffer;
		cache->usedCount_20 = 0;
		cache->allocMode_24 = 2;
		cache->capacity_0 = free;
		cache->freeSize_4 = free;
		cache->maxEntries_22 = count;
		cache->entries_8 = cache->data;
		cache->entryPtrs_12x = (subtype_x_DWORD_E9C28_str**)((uint8_t*)cache->data + entries);
		cache->buffer_16x = (uint32_t*)((uint8_t*)cache->data + entries + order);
		return cache;
	}
	cache = (type_x_DWORD_E9C28_str*)Malloc_42540_42880(0x1A);
	if (cache)
	{
		cache->buffer_16x = (uint32_t*)Malloc_42540_42880(size);
		if (cache->buffer_16x)
		{
			cache->entries_8 = (subtype_x_DWORD_E9C28_str*)Malloc_42540_42880(count * 14);
			if (cache->entries_8)
			{
				cache->entryPtrs_12x = (subtype_x_DWORD_E9C28_str**)Malloc_42540_42880(count * 4);
				if (cache->entryPtrs_12x)
				{
					cache->usedCount_20 = 0;
					cache->allocMode_24 = 1;
					cache->maxEntries_22 = count;
					cache->capacity_0 = size;
					cache->freeSize_4 = size;
					for (int i = count - 1; i >= 0; i--)
						cache->entries_8[i].size_4 = 0;
					return cache;
				}
			}
		}
		if (cache->buffer_16x)
		{
			if (cache->entries_8)
				Free_426E0_42A20(cache->entries_8);
			Free_426E0_42A20(cache->buffer_16x);
		}
		Free_426E0_42A20(cache);
	}
	exit(1);
}

int32_t GetTmapCacheFreeSize_369D0_36D90(type_x_DWORD_E9C28_str* cache)
{
	// IDA: W?cause$_failure$_ios$n_x__i (a library name matched by mistake)
	return cache->freeSize_4;
}

subtype_x_DWORD_E9C28_str* AddTmapCacheEntry_369E0_36DA0(type_x_DWORD_E9C28_str* cache, uint32_t size, int16_t tmap)
{
	if (size >= (uint32_t)cache->freeSize_4)
		return nullptr;
	const int16_t index = (int16_t)GetIndex_36850_36C10(cache);
	if (index <= -1)
		return nullptr;
	subtype_x_DWORD_E9C28_str& entry = cache->entries_8[index];
	entry.Index = index;
	entry.size_4 = size;
	entry.partstr_0 = (Type_Sprite*)((uint8_t*)cache->buffer_16x + cache->capacity_0 - cache->freeSize_4);
	entry.tmapIndex_12 = tmap;
	cache->freeSize_4 -= size;
	entry.slotIndex_8 = cache->usedCount_20;
	cache->entryPtrs_12x[(uint16_t)cache->usedCount_20] = &entry;
	cache->usedCount_20++;
	return &entry;
}

void FreeTmapCacheEntry_36A90_36E50(type_x_DWORD_E9C28_str* cache, subtype_x_DWORD_E9C28_str* entry)
{
	if ((uint16_t)entry->Index >= (uint16_t)cache->maxEntries_22)
		return;
	subtype_x_DWORD_E9C28_str& freed = cache->entries_8[(uint16_t)entry->Index];
	if (!freed.size_4)
		return;
	// the data after it moved down, the order closed up
	uint16_t slot = (uint16_t)freed.slotIndex_8;
	cache->freeSize_4 += freed.size_4;
	freed.size_4 = 0;
	uint8_t* target = (uint8_t*)freed.partstr_0;
	uint16_t used;
	for (;;)
	{
		slot++;
		used = (uint16_t)cache->usedCount_20;
		if (slot >= used)
			break;
		cache->entryPtrs_12x[slot - 1] = cache->entryPtrs_12x[slot];
		subtype_x_DWORD_E9C28_str* moved = cache->entryPtrs_12x[slot];
		moved->slotIndex_8 = (int16_t)(slot - 1);
		const uint8_t* source = (const uint8_t*)moved->partstr_0;
		moved->partstr_0 = (Type_Sprite*)target;
		memmove(target, source, moved->size_4);
		target += moved->size_4;
	}
	cache->usedCount_20 = (int16_t)(used - 1);
}

void FreeTmapCache_36C10_36FD0(type_x_DWORD_E9C28_str** cache)
{
	if (!*cache)
		return;
	if ((*cache)->allocMode_24 == 1)
	{
		Free_426E0_42A20((*cache)->buffer_16x);
		Free_426E0_42A20((*cache)->entries_8);
		Free_426E0_42A20((*cache)->entryPtrs_12x);
		Free_426E0_42A20(*cache);
	}
	*cache = nullptr;
}

type_E9C08* AllocateAnimations_10CB0_10CB0(uint16_t count)
{
	SetAllocationTag_42530_42870(9);
	type_E9C08* animations = (type_E9C08*)Malloc_42540_42880(6);
	if (animations)
	{
		animations->animations_2 = (type_animations1*)Malloc_42540_42880(count * 0x1C);
		if (animations->animations_2)
		{
			animations->count_0 = count;
			for (int i = count - 1; i >= 0; i--)
			{
				animations->animations_2[i].Sprite_4 = nullptr;
				animations->animations_2[i].active_0 = 0;
			}
		}
		else
		{
			Free_426E0_42A20(animations);
			animations = nullptr;
		}
	}
	SetAllocationTag_42530_42870(10);
	return animations;
}

type_animations1* InitTmapAnimation_10D40_10D40(type_E9C08* animations, Type_Sprite** sprite, int16_t index)
{
	// a record without a sprite (the last one), else one not active
	int16_t free = -1;
	int16_t inactive = -1;
	if (!animations->count_0)
		return nullptr;
	for (int16_t i = 0; i < (uint16_t)animations->count_0; i++)
	{
		type_animations1& record = animations->animations_2[i];
		if (!record.Sprite_4)
			free = i;
		else if (!record.active_0)
			inactive = i;
	}
	if (free <= 0)
		free = inactive;
	if (free <= -1)
		return nullptr;
	// the frames follow the first one: a word with their count after it
	const Type_Sprite* first = *sprite;
	const uint32_t frameSize = first->width * first->height;
	uint16_t frames;
	memcpy(&frames, (const uint8_t*)first + frameSize + 6, sizeof(frames));
	type_animations1& record = animations->animations_2[free];
	record.Sprite_4 = (Type_Sprite*)sprite;
	record.word_12 = 6;
	record.NextFrameOffset_14 = (uint16_t)(frameSize + 6);
	record.CountOfFrames_16 = (int16_t)frames;
	record.Width_18 = (int16_t)first->width;
	record.Height_20 = (int16_t)first->height;
	record.frameOffset_8 = (int32_t)(frameSize + 6);
	record.FrameIndex_22 = 1;
	record.active_0 = 1;
	record.word_24 = free;
	record.spriteIndex_26 = index;
	return &record;
}

void AnimateSprite_10EB0_10EB0(type_animations1* animation)
{
	// the next frame decoded over the image (MC2 72350 without the big sprites of the port)
	if ((uint16_t)animation->FrameIndex_22 > (uint16_t)animation->CountOfFrames_16)
	{
		animation->FrameIndex_22 = 1;
		animation->frameOffset_8 = animation->NextFrameOffset_14;
	}
	int8_t* image = (int8_t*)*(Type_Sprite**)animation->Sprite_4 + 6;
	SetAnimationPosition_10010_10010(animation->Width_18, 0);
	int8_t* next = DecodeAnimationFrame_1002D_1002D(image + animation->frameOffset_8, image);
	animation->frameOffset_8 = (int32_t)(next - image);
	animation->FrameIndex_22++;
}

void FreeAnimations_11010_11010(type_E9C08** animations)
{
	if (!*animations)
		return;
	Free_426E0_42A20((*animations)->animations_2);
	(*animations)->animations_2 = nullptr;
	Free_426E0_42A20(*animations);
	*animations = nullptr;
}

// 58910 / 58A00: the tmaps of the group out of the cache (an animation stopped)
static void UnloadGroup(uint16_t group)
{
	for (uint16_t tmap = group; tmap < 0x211 && TmapGroup(tmap) == group; tmap++)
	{
		if (!loadedTmaps_12DF8C_12DF7C[tmap])
			continue;
		if ((*loadedTmaps_12DF8C_12DF7C[tmap])->flags_0 & 1)
			ResetAnimation_10F10_10F10(GetAnimationByIndex_10FD0_10FD0(animations_AE3E0_AE3D0, (int16_t)tmap));
		FreeTmapCacheEntry_36A90_36E50(tmapCache_AE3EC_AE3DC, tmapEntries_12D748_12D738[tmap]);
		loadedTmaps_12DF8C_12DF7C[tmap] = nullptr;
		tmapEntries_12D748_12D738[tmap] = nullptr;
		tmapLastUse_12CF00_12CEF0[tmap] = 0;
	}
}

uint8_t ResetTmapGroup_58910_58E20(uint16_t tmap)
{
	// not the locked ones
	const uint16_t group = TmapGroup(tmap);
	if (lockedTmaps_12E7D0_12E7C0[group] || !loadedTmaps_12DF8C_12DF7C[group])
		return 0;
	UnloadGroup(group);
	return 1;
}

uint8_t UnloadTmapGroup_58A00_58F10(uint16_t tmap)
{
	const uint16_t group = TmapGroup(tmap);
	if (!loadedTmaps_12DF8C_12DF7C[group])
		return 0;
	UnloadGroup(group);
	return 1;
}

uint32_t GetTmapGroupSize_58AD0_58FE0(uint16_t tmap)
{
	const uint16_t group = TmapGroup(tmap);
	uint32_t size = 0;
	for (uint16_t i = group; i < 0x211 && TmapGroup(i) == group; i++)
		size += TmapSize(i) + 10;
	return size;
}

void LoadTmapGroup_58B30_59040(uint16_t tmap)
{
	const uint16_t group = TmapGroup(tmap);
	const int32_t frame = (int32_t)mc1Settings_AE408_AE3F8->frames_0x04_4;
	for (uint16_t i = group; i < 0x211 && TmapGroup(i) == group; i++)
	{
		if (loadedTmaps_12DF8C_12DF7C[i])
			continue;
		subtype_x_DWORD_E9C28_str* entry = AddTmapCacheEntry_369E0_36DA0(tmapCache_AE3EC_AE3DC, ((TmapSize(i) + 0x0D) >> 2) * 4 & 0xFFFF, (int16_t)i);
		if (!entry || ReadTmap_58860_58D70(i, (uint8_t*)entry->partstr_0) == -1)
			continue;
		tmapEntries_12D748_12D738[i] = entry;
		loadedTmaps_12DF8C_12DF7C[i] = (Type_Sprite**)entry;
		tmapLastUse_12CF00_12CEF0[i] = frame;
		if ((*loadedTmaps_12DF8C_12DF7C[i])->flags_0 & 1)
			InitTmapAnimation_10D40_10D40(animations_AE3E0_AE3D0, loadedTmaps_12DF8C_12DF7C[i], (int16_t)i);
	}
}

uint32_t EvictLeastUsedTmaps_58C30_59140(uint32_t needed)
{
	// the 5 groups used the longest ago; the locked ones only when no other is loaded
	uint32_t times[5];
	int32_t groups[5];
	for (int i = 0; i < 5; i++)
	{
		times[i] = 0xFFFFFFFF;
		groups[i] = -1;
	}
	uint8_t onlyLocked = 1;
	for (uint16_t i = 0; i < 0x211; i++)
	{
		const uint16_t group = TmapGroup(i);
		if (loadedTmaps_12DF8C_12DF7C[group] && !lockedTmaps_12E7D0_12E7C0[group])
			onlyLocked = 0;
		while (i + 1 < 0x211 && TmapGroup(i + 1) == group)
			i++;
	}
	for (uint16_t i = 0; i < 0x211; i++)
	{
		const uint16_t group = TmapGroup(i);
		if ((!lockedTmaps_12E7D0_12E7C0[group] || onlyLocked) && loadedTmaps_12DF8C_12DF7C[group])
		{
			uint32_t time = (uint32_t)tmapLastUse_12CF00_12CEF0[group];
			int32_t candidate = group;
			for (int k = 0; k < 5; k++)
				if (time < times[k])
				{
					std::swap(time, times[k]);
					std::swap(candidate, groups[k]);
				}
		}
		while (i + 1 < 0x211 && TmapGroup(i + 1) == group)
			i++;
	}
	uint32_t freed = 0;
	for (int k = 0; k < 5; k++)
	{
		if (freed >= needed || groups[k] <= -1)
			return freed;
		const uint8_t done = onlyLocked ? UnloadTmapGroup_58A00_58F10((uint16_t)groups[k]) : ResetTmapGroup_58910_58E20((uint16_t)groups[k]);
		if (done)
			freed += GetTmapGroupSize_58AD0_58FE0((uint16_t)groups[k]);
	}
	return freed;
}

void InitTmapsAndSprites_58E70_59380()
{
	Type_Mc1Settings_AE408_AE3F8* settings = mc1Settings_AE408_AE3F8;
	tmapCache_AE3EC_AE3DC = InitTmapCache_36700_36AC0(settings->heapSize_0xAC_172, 0x211, settings->heap_0xA8_168);
	if (tmapCache_AE3EC_AE3DC)
		animations_AE3E0_AE3D0 = AllocateAnimations_10CB0_10CB0(0x211);
	OpenTmaps_587E0_58CF0();
	memset(loadedTmaps_12DF8C_12DF7C, 0, sizeof(loadedTmaps_12DF8C_12DF7C));
	memset(tmapEntries_12D748_12D738, 0, sizeof(tmapEntries_12D748_12D738));
	memset(tmapLastUse_12CF00_12CEF0, 0, sizeof(tmapLastUse_12CF00_12CEF0));
	LockLevelSprites_59320_59830();
	LoadLockedTmaps_59370_59880();
}

void FreeTmaps_58F00_59410()
{
	FreeTmapCache_36C10_36FD0(&tmapCache_AE3EC_AE3DC);
	FreeAnimations_11010_11010(&animations_AE3E0_AE3D0);
	ClearUsedTmapGroups_59140_59650();
	memset(loadedTmaps_12DF8C_12DF7C, 0, sizeof(loadedTmaps_12DF8C_12DF7C));
	memset(tmapEntries_12D748_12D738, 0, sizeof(tmapEntries_12D748_12D738));
	memset(tmapLastUse_12CF00_12CEF0, 0, sizeof(tmapLastUse_12CF00_12CEF0));
	byte_968E8_968E8 = 0;
	// no return in the original: it goes on in 58F70
	CloseTmaps_58F70_59480();
}

int32_t MainInitTmaps_59050_59560(uint16_t tmap)
{
	int32_t needed = (int32_t)GetTmapGroupSize_58AD0_58FE0(tmap) - GetTmapCacheFreeSize_369D0_36D90(tmapCache_AE3EC_AE3DC) + 0x14;
	for (uint16_t tries = 0; needed > 0 && tries < 4; tries++)
		needed -= (int32_t)EvictLeastUsedTmaps_58C30_59140((uint32_t)needed);
	if (needed <= 0)
	{
		LoadTmapGroup_58B30_59040(tmap);
		mc1Settings_AE408_AE3F8->tmapsLoaded_0x95_149 = 5;
	}
	return loadedTmaps_12DF8C_12DF7C[tmap] != nullptr;
}

void UpdateSpriteAnimations_590D0_595E0()
{
	// the animated sprites drawn in the frame (flag 8) step on
	for (uint16_t i = 0; i < 0x211; i++)
	{
		if (!loadedTmaps_12DF8C_12DF7C[i])
			continue;
		Type_Sprite* sprite = *loadedTmaps_12DF8C_12DF7C[i];
		if (!(sprite->flags_0 & 8))
			continue;
		sprite->flags_0 &= ~8;
		if (sprite->flags_0 & 1)
			AnimateSprite_10EB0_10EB0(GetAnimationByIndex_10FD0_10FD0(animations_AE3E0_AE3D0, (int16_t)i));
	}
}

void LoadLevelTmaps_59230_59740()
{
	if (!tmapCache_AE3EC_AE3DC)
		InitTmapsAndSprites_58E70_59380();
	memset(tmapLastUse_12CF00_12CEF0, 0, sizeof(tmapLastUse_12CF00_12CEF0));
	// the groups the level does not use out, then the used ones by their level sprite group from 0FFh down
	for (uint16_t i = 0; i < 0x211; i++)
		if (!usedTmapGroups_12E9E1_12E9D1[i])
			ResetTmapGroup_58910_58E20(i);
	uint8_t full = 0;
	for (uint8_t priority = 0xFF; priority && !full; priority--)
		for (uint16_t i = 0; i < 0x211 && !full; i++)
		{
			if (priority != usedTmapGroups_12E9E1_12E9D1[i] || loadedTmaps_12DF8C_12DF7C[i])
				continue;
			if (GetTmapGroupSize_58AD0_58FE0(i) < (uint32_t)GetTmapCacheFreeSize_369D0_36D90(tmapCache_AE3EC_AE3DC))
				LoadTmapGroup_58B30_59040(i);
			// 592D7: the original compares 400h with the address of 369D0, not its result: never full
		}
}

void LockLevelSprites_59320_59830()
{
	memset(lockedTmaps_12E7D0_12E7C0, 0, sizeof(lockedTmaps_12E7D0_12E7C0));
	if (!byte_968F0_968F0)
		return;
	for (const Type_Mc1SpriteDef_99BA0& def : spriteDefs_99BA0_99BA0)
	{
		if (!def.width_0x06_6 && !def.height_0x08_8)
			return;
		if ((uint8_t)def.levelSpriteGroup_0x0B_11 == 0xFF)
			lockedTmaps_12E7D0_12E7C0[def.textureIndex_0x00_0] = 0xFF;
	}
}

void LoadLockedTmaps_59370_59880()
{
	for (uint16_t i = 0; i < 0x211; i++)
		if (lockedTmaps_12E7D0_12E7C0[i])
			LoadTmapGroup_58B30_59040(i);
}

void MarkLoadedTmaps_593B0_598C0()
{
	// for the movie: 1 loaded, 2 loaded and locked
	uint8_t* used = mc1State_AE400_AE3F0->usedTmaps_0x2C_44;
	memset(used, 0, sizeof(mc1State_AE400_AE3F0->usedTmaps_0x2C_44));
	for (uint16_t i = 0; i < 0x211; i++)
	{
		if (!loadedTmaps_12DF8C_12DF7C[i])
			continue;
		used[i] = 1;
		if (lockedTmaps_12E7D0_12E7C0[i])
			used[i]++;
	}
}

void ReloadTmaps_59420_59930()
{
	// after the state of a movie: the tmaps it had (593B0), the locked ones first
	if (!tmapCache_AE3EC_AE3DC)
		InitTmapsAndSprites_58E70_59380();
	const uint8_t* used = mc1State_AE400_AE3F0->usedTmaps_0x2C_44;
	for (uint16_t i = 0; i < 0x211; i++)
		if (!used[i])
			ResetTmapGroup_58910_58E20(i);
	uint8_t full = 0;
	for (uint8_t priority = 2; priority && !full; priority--)
		for (uint16_t i = 0; i < 0x211 && !full; i++)
		{
			if (priority != used[i] || loadedTmaps_12DF8C_12DF7C[i])
				continue;
			if (GetTmapGroupSize_58AD0_58FE0(i) < (uint32_t)GetTmapCacheFreeSize_369D0_36D90(tmapCache_AE3EC_AE3DC))
				LoadTmapGroup_58B30_59040(i);
			else if ((uint32_t)GetTmapCacheFreeSize_369D0_36D90(tmapCache_AE3EC_AE3DC) < 0x400)
				full = 1;
		}
}
