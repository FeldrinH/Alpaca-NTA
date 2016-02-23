#include "pathOption.h"
#include "..\Frontend\main.h"
#include <iostream>

int pathOption::maxDepth = 1;
//std::vector<int>* pathOption::minPathArray = nullptr;

pathOption::pathOption()
{
}

pathOption::pathOption(std::multiset<uint16_t> minPathSet)
	:curDistance(0), minPathSet(minPathSet)
{
	curPath.push_back(0);
	for (int i = 1; i < maxDepth; i++)
	{
		openNodes.insert(i);
	}
	updateMinPath();
}

pathOption::~pathOption()
{
}

inline int pathOption::getDepth() const
{
	return curPath.size();
}

inline int pathOption::getEstimate() const
{
	return minPathEstimate + curDistance;
}

inline node pathOption::getLastNode()
{
	return nodeArray[curPath.back()];
}

inline void pathOption::updateMinPath()
{
	std::multiset<uint16_t>::iterator j = minPathSet.begin();
	minPathEstimate = 0;
	for (int i = 0; i < maxDepth - curPath.size() - 1; i++)
	{
		minPathEstimate += *j;
		j++;
	}
	//minPathEstimate += curDistance;
}

bool pathOption::addNode(int newNode, pathOption &curBest)
{
	/*for (std::multiset<int>::iterator i = minPathSet.begin(); i != minPathSet.end(); i++)
	{
		std::cout << *i << " ";
	}
	std::cout << std::endl << minPathEstimate << std::endl;*/

	if (curPath.size() < maxDepth)
	{
		openNodes.erase(newNode);
		curDistance += getLastNode().distanceArray[newNode];
		curPath.push_back(newNode);
		//std::cout << "New path: " << getDepth() << std::endl;
		if (curPath.size() >= maxDepth)
		{
			return false;
		}
	}
	
	if (getEstimate() >= curBest.curDistance)
	{
		//std::cout << "Path removed: " << curDistance << std::endl;
		return false;
	}

	for (std::set<uint8_t>::iterator i = openNodes.begin(); i != openNodes.end(); i++)
	{
		minPathSet.erase(minPathSet.find(getLastNode().distanceArray[*i]));
	}

	updateMinPath();

	return true;
}

void pathOption::printPath() const
{
	for (std::vector<uint8_t>::const_iterator i = curPath.begin(); i != curPath.end(); i++)
	{
		std::cout << +(*i) << ' '; 
	}
	std::cout << std::endl;
}

bool pathOption::operator > (const pathOption& other)
{
	if (getDepth() > other.getDepth())
	{
		return false;
	}
	else if (getDepth() == other.getDepth())
	{
		return (getEstimate() > other.getEstimate());
	}
	else
	{
		return true;
	}
}

bool pathOption::operator < (const pathOption& other)
{
	return !(*this > other || *this == other);
}

bool pathOption::operator == (const pathOption& other)
{
	return (getDepth() == other.getDepth() && curDistance == other.curDistance);
}

void pathOption::addNodeForce(int newNode)
{
	openNodes.erase(newNode);
	curDistance += getLastNode().distanceArray[newNode];
	curPath.push_back(newNode);
}