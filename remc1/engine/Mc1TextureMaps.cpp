#include "Mc1TextureMaps.h"

#include <cstring>
#include <string>

#include "Mc1Files.h"
#include "Mc1Game.h"
#include "Mc1Install.h"
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
	int32_t result = DataFileRNC::Decompress(buffer, buffer);
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
	uint8_t* buffer = workScreen_12EFF4_12EFE4;
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
