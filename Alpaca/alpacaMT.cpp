#include <chrono>
#include "pathOption.h"
#include <iostream>
#include <algorithm>
#include "..\Frontend\main.h"
#include "insertion_queue.h"
#include "..\Frontend\cmdHandler.h"
#include "alpaca.h"
#include <thread>
#include <mutex>

using namespace std;

mutex queueLock;

void AlpacaThread(insertion_queue *nodeQueue, multiset<uint16_t> *minPathSet, int *procCount, pathOption *bestFinished)
{
	unique_lock<mutex> queueHandle(queueLock, defer_lock);

	pathOption curOption = pathOption(*minPathSet);
	pathOption pushOption = pathOption(*minPathSet);

	bool needsLock = true;

	while (!nodeQueue->empty() && *procCount < 1)
	{
		queueHandle.lock();
		if (!nodeQueue->empty())
		{
			++procCount;
			curOption = nodeQueue->pull_back();
			if (nodeQueue->empty())
			{
				needsLock = false;
			}
			else
			{
				queueHandle.unlock();
			}

			for (set<uint8_t>::iterator i = curOption.openNodes.begin(); i != curOption.openNodes.end(); i++)
			{
				pushOption = curOption;
				if (pushOption.addNode(*i, *bestFinished))
				{
					if (needsLock)
					{
						queueHandle.lock();
					}
					else
					{
						needsLock = true;
					}
					nodeQueue->push(pushOption);
					queueHandle.unlock();
				}
				else if (pushOption.curPath.size() >= pathOption::maxDepth && pushOption.curDistance < bestFinished->curDistance)
				{
					*bestFinished = pushOption;
					cout << "New best path: " << pushOption.curDistance << endl;
				}
			}
			--procCount;
		}
		else
		{
			queueHandle.unlock();
			//cout << "Skip!" << endl;
		}
	} 
}

alpacaMTResult AlpacaMT(vector<uint8_t>* drawArray, int threadCount)
{
	cout << VERSION_NAME " Multi-Threaded began..." << endl;
	chrono::high_resolution_clock::time_point beginTime = chrono::high_resolution_clock::now();

	multiset<uint16_t> minPathSet;
	nodeArray[0].preCalcStart();
	for (int i = 1; i < nodeArray.size(); i++)
	{
		nodeArray[i].preCalcDistance(i, &minPathSet);
	}
	cout << "Pre-Calculations finished." << endl;

	pathOption bestFinished = pathOption(minPathSet);

	int smallest;
	for (int i = 1; i < pathOption::maxDepth; i++)
	{
		smallest = *(bestFinished.openNodes.begin());
		for (set<uint8_t>::iterator i = ++bestFinished.openNodes.begin(); i != bestFinished.openNodes.end(); i++)
		{
			if (bestFinished.getLastNode().distanceArray[*i] < bestFinished.getLastNode().distanceArray[smallest])
			{
				smallest = *i;
			}
		}
		bestFinished.addNodeForce(smallest);
	}
	cout << "Quasi-best path: " << bestFinished.curDistance << endl;
	bestFinished.printPath();
	int quasiBest = bestFinished.curDistance;

	//MAIN LOOP
	insertion_queue nodeQueue;
	int procCount = 0;

	nodeQueue.push(pathOption(minPathSet));

	//Threads Init
	vector<thread> threadArray;
	for (int i = 0; i < threadCount; i++)
	{
		threadArray.push_back(thread(AlpacaThread, &nodeQueue, &minPathSet, &procCount, &bestFinished));
	}
	cout << "No. Threads: " << threadArray.size() << endl;
	for (int i = 0; i < threadArray.size(); i++)
	{
		threadArray[i].join();
	}

	chrono::high_resolution_clock::time_point endTime = chrono::high_resolution_clock::now();
	chrono::duration<double> execTime = chrono::duration_cast<chrono::duration<double>>(endTime - beginTime);

	*drawArray = bestFinished.curPath;

	clearNodeArrays();

	cout << "..." VERSION_NAME " Multi-Threaded ended." << endl;
	cout << "Quasi-best path: " << quasiBest << endl;
	cout << "Best path: " << bestFinished.curDistance << endl;
	bestFinished.printPath();
	cout << "Node count: " << pathOption::maxDepth - 1 << " + 1" << endl;
	cout << "Time: " << execTime.count() << endl;
	return alpacaMTResult(bestFinished.curDistance,execTime.count());
}