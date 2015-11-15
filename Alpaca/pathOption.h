#pragma once

#include <vector>
#include "Node.h"
#include <set>

class pathOption
{
public:
	bool operator < (const pathOption& other);
	bool operator > (const pathOption& other);
	bool operator == (const pathOption& other);

	int curDistance;
	static int maxDepth;

	pathOption(std::multiset<int> minPathSet);
	~pathOption();

	int getDepth() const;
	int getEstimate() const;

	bool addNode(int newNode, pathOption &curBest);
	void addNodeForce(int newNode);
//	void makePrecise();
	void printPath() const;
	node getLastNode();
	void updateMinPath();

	std::multiset<int> minPathSet;
	int minPathEstimate;
	std::set<int> openNodes;
	std::vector<int> curPath;
};

