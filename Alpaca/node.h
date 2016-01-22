#pragma once

#include <set>
#include <vector>
#include <stdint.h>

struct node
{
	int x;
	int y;
	std::vector<uint16_t> distanceArray;

	node(int x, int y);
	void preCalcStart();
	void preCalcDistance(int index, std::multiset<uint16_t>* minPathArray);
	void draw(int index, int radius);
};

void clearNodeArrays();