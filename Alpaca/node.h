#pragma once

#include <set>
#include <vector>

struct node
{
	int x;
	int y;
	std::vector<int> distanceArray;

	node(int x, int y);
	void preCalcStart();
	void preCalcDistance(int index, std::multiset<int>* minPathArray);
	void draw(int index, int radius);
};

void clearNodeArrays();