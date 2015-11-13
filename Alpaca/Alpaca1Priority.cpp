#include <vector>
#include <chrono>
#include "pathOption.h"
#include <iostream>
#include <queue>
#include "sortPath.h"
#include <algorithm>
#include "Main.h"

int Alpaca1Priority(int radius, std::vector<int>* drawArray)
{
	std::cout << "Alpaca 1.0 began..." << std::endl;
	std::cout << "Node count: " << pathOption::maxDepth << std::endl;
	std::chrono::high_resolution_clock::time_point beginTime = std::chrono::high_resolution_clock::now();

	std::vector<int> minPathArray;
	for (int i = 0; i < nodeArray.size(); i++)
	{
		nodeArray[i].preCalcDistance(i,&minPathArray);
	}

	std::cout << "Pre-Calculations finished." << std::endl;

	pathOption bestFinished = pathOption(&minPathArray);

	int smallest;
	for (int i = 1; i < pathOption::maxDepth; i++)
	{
		smallest = *(bestFinished.openNodes.begin());
		for (std::set<int>::iterator i = ++bestFinished.openNodes.begin(); i != bestFinished.openNodes.end(); i++)
		{
			if (bestFinished.getLastNode().distanceArray[*i] < bestFinished.getLastNode().distanceArray[smallest])
			{
				smallest = *i;
			}
		}
		bestFinished.addNode(smallest, &bestFinished);
	}
	std::cout << "Quasi-best path: " << bestFinished.curDistance << std::endl;
	bestFinished.printPath();
	int quasiBest = bestFinished.curDistance;

	//Main loop
	int loopNum = 0;
	pathOption curOption = pathOption(&minPathArray);
	pathOption pushOption = pathOption(&minPathArray);
	std::priority_queue < pathOption, std::vector<pathOption>, shorterFirst> nodeQueue;
	nodeQueue.push(pathOption(&minPathArray));

	do
	{
		curOption = nodeQueue.top();
		nodeQueue.pop();

		std::cout << ++loopNum << " " << curOption.curDistance << " " << curOption.getDepth() << std::endl;

		for (std::set<int>::iterator i = curOption.openNodes.begin(); i != curOption.openNodes.end(); i++)
		{
			pushOption = curOption;
			if (pushOption.addNode(*i, &bestFinished))
			{
				nodeQueue.push(pushOption);
			}
			else if (pushOption.curDistance < bestFinished.curDistance)
			{
				std::cout << "Old best path: " << bestFinished.curDistance;
				bestFinished = pushOption;
				std::cout << " New best path: " << pushOption.curDistance << std::endl;
			}
		}
	} while (nodeQueue.size() > 0 /*|| nodeQueue.top().getDepth() < pathOption::maxDepth */ /*|| bestFinished->getPrecise() == false*/);

	std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> execTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - beginTime);

	*drawArray = bestFinished.curPath;

	bestFinished.printPath();
	std::cout << "Node count: " << pathOption::maxDepth << std::endl;
	std::cout << "Best path: " << bestFinished.curDistance << std::endl;
	std::cout << "Quasi-best path: " << quasiBest << std::endl;
	std::cout << "...Alpaca 1.0 ended." << std::endl;
	std::cout << "Time: " << execTime.count() << std::endl;
	return bestFinished.curDistance;
}