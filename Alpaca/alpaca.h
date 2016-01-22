#pragma once

#include <vector>
#include "Node.h"

#define VERSION_NAME "AlpacaNTA 1.3.3"

struct alpacaResult
{
	alpacaResult(int distance, double time, int loops) :distance(distance), time(time), loops(loops) {}

	int distance;
	double time;
	int loops;
};

struct alpacaMTResult
{
	alpacaMTResult(int distance, double time) :distance(distance), time(time) {}

	int distance;
	double time;
};

alpacaResult Alpaca(std::vector<uint8_t>* drawArray, std::string &cmdIn);
alpacaMTResult AlpacaMT(std::vector<uint8_t>* drawArray, int threadCount);