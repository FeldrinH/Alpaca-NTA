#pragma once

#include <vector>
#include "Node.h"
#include <set>
#include <cstdint>

class pathOption
{
public:
	bool operator < (const pathOption& other);
	bool operator > (const pathOption& other);
	bool operator == (const pathOption& other);

	int curDistance;
	static int maxDepth;

	pathOption(std::multiset<uint16_t> minPathSet);
	~pathOption();

	int getDepth() const;
	int getEstimate() const;

	bool addNode(int newNode, pathOption &curBest);
	void addNodeForce(int newNode);
//	void makePrecise();
	void printPath() const;
	node getLastNode();
	void updateMinPath();

	std::multiset<uint16_t> minPathSet;
	int minPathEstimate;
	std::set<uint8_t> openNodes;
	std::vector<uint8_t> curPath;
};

