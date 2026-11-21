#pragma once

#include <cstdint>
#include <string>
#include <vector>

// port: the regression tests of MC1 and HW (remc2-regression-test, memimages/regressions-mc1 / regressions-mc1hw):
// the input of player 0 from input.rec, the state compared with reference.binz of DOSBox (dosbox-x-remc2 branch mc1,
// REMC1_DUMP_EVERY at 20FB0) every reference interval, the test ended by End_thread (20 passed)

// a record of the state in the DOS layout: the seed, the entities (A4h each), the players, the settings, the terrain maps
constexpr uint32_t mc1RegressionSettingsSize = 0x8E7E;
constexpr uint32_t mc1RegressionRecordSize = 4 + 1000 * 0xA4 + 8 * 0x801 + mc1RegressionSettingsSize + 3 * 0x10000;

extern std::string mc1RegressionFolder; // the folder of the test, empty no test

// the file of the input of the test, empty none
std::string Mc1RegressionInput();
// before DrawFrame_20FB0: the comparison at the frames of the reference
void Mc1RegressionFrame();
// the frames of the reference of the folder (0 no reference)
int32_t Mc1RegressionFrames(const std::string& folder);
// reference.binz from the dumps of DOSBox (rand.bin, entities.bin, players.bin, settings.bin, terrain.bin, height.bin, angle.bin)
bool PackMc1Reference(const std::string& dumpFolder, const std::string& outFile, uint32_t every);
