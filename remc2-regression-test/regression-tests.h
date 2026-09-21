#pragma once
#include "../remc2/sub_main.h"
#include "../remc2/engine/engine_support.h"
#include "../remc2/engine/CommandLineParser.h"
#include "../remc2/engine/MenusAndIntros.h"
#include "../remc2/engine/Network.h"

std::string RegressionsPath();
int run_regtest(int level, int testType = 0, int index = 1, int saveIndex = 0, const char* recordName = "",int maxSteps=20,bool turnOnIntervalSave=false, const char* recordFolder = "");
