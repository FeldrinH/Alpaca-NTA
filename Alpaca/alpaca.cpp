#include <chrono>
#include "pathOption.h"
#include <iostream>
#include "sortPath.h"
#include <algorithm>
#include "..\Frontend\main.h"
#include "insertion_queue.h"
#include "..\Frontend\cmdHandler.h"
#include "alpaca.h"

double Alpaca(std::vector<int>* drawArray, std::string &cmdIn)
{
	std::cout << VERSION_NAME " began..." << std::endl;
	std::chrono::high_resolution_clock::time_point beginTime = std::chrono::high_resolution_clock::now();

	std::multiset<int> minPathSet;
	nodeArray[0].preCalcStart();
	for (int i = 1; i < nodeArray.size(); i++)
	{
		nodeArray[i].preCalcDistance(i,&minPathSet);
	}
	std::cout << "Pre-Calculations finished." << std::endl;

	pathOption bestFinished = pathOption(minPathSet);

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
		bestFinished.addNodeForce(smallest);
	}
	std::cout << "Quasi-best path: " << bestFinished.curDistance << std::endl;
	bestFinished.printPath();
	int quasiBest = bestFinished.curDistance;

	//MAIN LOOP
	int loopNum = 0;

	insertion_queue nodeQueue;

	nodeQueue.push(pathOption(minPathSet));

	pathOption curOption = pathOption(minPathSet);
	pathOption pushOption = pathOption(minPathSet);

	while (!nodeQueue.empty() && cmdIn != "stop")
	{
		curOption = nodeQueue.pull_back();

		for (std::set<int>::iterator i = curOption.openNodes.begin(); i != curOption.openNodes.end(); i++)
		{
			pushOption = curOption;
			if (pushOption.addNode(*i, bestFinished))
			{
				nodeQueue.push(pushOption);
			}
			else if (pushOption.curPath.size() >= pathOption::maxDepth && pushOption.curDistance < bestFinished.curDistance)
			{
				bestFinished = pushOption;
				std::cout << "New best path: " << pushOption.curDistance << " Loop #" << loopNum << std::endl;
			}
		}

		//nodeQueue.refreshSegments();
		++loopNum;
		//std::cout << loopNum << " " << curOption.getEstimate() << " " << curOption.getDepth() << " " << nodeQueue.segments() << std::endl;

	}

	std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> execTime = std::chrono::duration_cast<std::chrono::duration<double>>(endTime - beginTime);

	*drawArray = bestFinished.curPath;

	clearNodeArrays();

	std::cout << "..." VERSION_NAME " ended." << std::endl;
	std::cout << "Quasi-best path: " << quasiBest << std::endl;
	std::cout << "Best path: " << bestFinished.curDistance << std::endl;
	bestFinished.printPath();
	std::cout << "Node count: " << pathOption::maxDepth - 1 << " + 1" << std::endl;
	std::cout << "Total loops: " << loopNum << std::endl;
	std::cout << "Time: " << execTime.count() << std::endl;
	return execTime.count();
}