#include "Node.h"
#include <iostream>
#include "..\Frontend\SDL2_gfxPrimitives.h"
#include "..\Frontend\main.h"

node::node(int x, int y)
	:x(x), y(y)
{
}

void node::preCalcDistance(int index, std::multiset<uint16_t>* minPathArray)
{
	distanceArray.push_back(-2015);
	for (int i = 1; i < nodeArray.size(); i++)
	{
		if (i == index)
		{
			distanceArray.push_back(-1000);
			//std::cout << "Self" << std::endl;
		}
		else if (nodeArray[i].distanceArray.size() > index)
		{
			distanceArray.push_back(nodeArray[i].distanceArray[index]);
			//std::cout << "Calculated: " << distanceArray.back() << std::endl;
		}
		else
		{
			distanceArray.push_back(round(sqrt(pow(x - nodeArray[i].x, 2) + pow(y - nodeArray[i].y, 2))));
			minPathArray->insert(distanceArray.back());
			//std::cout << "Calculating: " << distanceArray.back() << std::endl;
		}
	}
}

void node::preCalcStart()
{
	distanceArray.push_back(-2015);
	for (int i = 1; i < nodeArray.size(); i++)
	{
		distanceArray.push_back(round(sqrt(pow(x - nodeArray[i].x, 2) + pow(y - nodeArray[i].y, 2))));
	}
}

void node::draw(int index, int radius)
{
	if (index == 0)
	{
		filledCircleRGBA(renderer, x, y, radius, 100, 255, 100, 255);
	}
	else
	{
		filledCircleRGBA(renderer, x, y, radius, 200, 200, 200, 255);
	}
	aacircleRGBA(renderer, x, y, radius, 150, 150, 150, 255);
}

void clearNodeArrays()
{
	for (int i = 0; i < nodeArray.size(); i++)
	{
		nodeArray[i].distanceArray.clear();
	}
}