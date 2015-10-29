#pragma once

#include <vector>
#include "Node.h"

#define VERSION_NAME "AlpacaNTA 1.3.2"

double Alpaca(std::vector<int>* drawArray, std::string &cmdIn);
double AlpacaMT(std::vector<int>* drawArray, int threadCount);
