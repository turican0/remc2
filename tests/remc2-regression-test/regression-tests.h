#pragma once
#include "sub_main.h"
#include "engine/engine_support.h"
#include "engine/CommandLineParser.h"
#include "engine/MenusAndIntros.h"
#include "engine/Network.h"

int run_regtest(int level, int testType = 0, int index = 1, int saveIndex = 0, const char* recordName = "",int maxSteps=20,bool turnOnIntervalSave=false);
