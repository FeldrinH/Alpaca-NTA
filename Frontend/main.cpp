#include "main.h"
#include "cmdHandler.h"
#include "..\Alpaca\alpaca.h"
#include "..\Alpaca\pathOption.h"
#include "SDL2_gfxPrimitives.h"
#include <iostream>
#include <string>
#include <thread>
#include <windows.h>

SDL_Renderer* renderer;
std::vector<node> nodeArray;

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow(VERSION_NAME, 20, 20, 640, 320, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);
	SDL_Event mainEvent;
	SetConsoleTitle(VERSION_NAME);

	bool isRunning = true;

	int threadCount = std::thread::hardware_concurrency();

	std::vector<int> drawArray;
	nodeArray.push_back(node(320, 160));
	std::string cmdIn = "";
	int radius = 20;

	std::thread inputHandler(getCmdIn, &cmdIn, &isRunning);

	std::cout << "Thread count: " << threadCount << std::endl;

	while (isRunning)
	{
		while (SDL_PollEvent(&mainEvent) != 0)
		{
			if (mainEvent.type == SDL_QUIT)
			{
				isRunning = false;
				std::cout << "Closing..." << std::endl;
			}
			if (mainEvent.type == SDL_KEYDOWN)
			{
				if (mainEvent.key.keysym.sym == SDLK_r)
				{
					clearNodeArrays();
					drawArray.clear();
					nodeArray.erase(nodeArray.begin() + 1, nodeArray.end());
					pathOption::maxDepth = 1;
				}
			}
			if (mainEvent.type == SDL_MOUSEBUTTONDOWN)
			{
				if (mainEvent.button.button == SDL_BUTTON_LEFT)
				{
					nodeArray.push_back(node(mainEvent.button.x, mainEvent.button.y));
					pathOption::maxDepth = nodeArray.size();
					clearNodeArrays();
					drawArray.clear();
					std::cout << "Node " << nodeArray.size() - 1 << std::endl;
				}
				else if (mainEvent.button.button == SDL_BUTTON_RIGHT)
				{
					nodeArray[0].x = mainEvent.button.x;
					nodeArray[0].y = mainEvent.button.y;
					clearNodeArrays();
					drawArray.clear();
				}
			}
		}

		if (cmdIn != "")
		{
			if (cmdIn == "alpaca")
			{
				Alpaca(&drawArray,cmdIn);
			}
			else if (cmdIn == "alpacaMT")
			{
				AlpacaMT(&drawArray, threadCount);
			}
			else if (cmdIn == "test")
			{
				int nodeCount;
				int sampleCount;
				std::cin >> nodeCount >> sampleCount;
				double minTime = 1000;
				double maxTime = 0;
				double totalTime = 0;
				long long totalLoops = 0;
				alpacaResult curResult(0,0.0,0);
				clearNodeArrays();
				drawArray.clear();
				nodeArray.erase(nodeArray.begin() + 1, nodeArray.end());
				for (int i = 0; i < sampleCount; i++)
				{
					for (int j = 0; j < nodeCount; j++)
					{
						nodeArray.push_back(node(std::rand() % 1270,std::rand() % 720));
					}
					pathOption::maxDepth = nodeArray.size();

					curResult = Alpaca(&drawArray,cmdIn);
					totalLoops += curResult.loops;
					totalTime += curResult.time;
					if (curResult.time > maxTime)
					{
						maxTime = curResult.time;
					}
					if (curResult.time < minTime)
					{
						minTime = curResult.time;
					}

					clearNodeArrays();
					drawArray.clear();
					nodeArray.erase(nodeArray.begin() + 1, nodeArray.end());
					pathOption::maxDepth = 1;
					std::cout << i << std::endl;
				}
				std::cout << "Average time: " << totalTime / sampleCount << std::endl;
				std::cout << "Minimum time: " << minTime << std::endl;
				std::cout << "Maximum time: " << maxTime << std::endl;
				std::cout << "Average loops: " << totalLoops / (long double)sampleCount << std::endl;
			}
			else if (cmdIn == "random")
			{
				int nodeCount;
				std::cin >> nodeCount;
				clearNodeArrays();
				drawArray.clear();
				nodeArray.erase(nodeArray.begin() + 1, nodeArray.end());
				int scrW;
				int scrH;
				SDL_GetWindowSize(window, &scrW, &scrH);
				for (int j = 0; j < nodeCount; j++)
				{
					nodeArray.push_back(node(std::rand() % scrW, std::rand() % scrH));
				}
				pathOption::maxDepth = nodeArray.size();
			}
			else if (cmdIn == "node")
			{
				int x;
				int y;
				std::cin >> x >> y;
				nodeArray.push_back(node(x,y));
				pathOption::maxDepth = nodeArray.size();
				clearNodeArrays();
				drawArray.clear();
				std::cout << "Node " << nodeArray.size() - 1 << std::endl;
			}
			else if (cmdIn == "setThreadCount")
			{
				std::cin >> threadCount;
				std::cout << "Thread count: " << threadCount << std::endl;
			}
			else if (cmdIn == "autoThreadCount")
			{
				int count;
				int maxCount;
				int step;
				std::cin >> count >> maxCount >> step;

				double bestTime = INFINITY;
				double bestCount = 0;

				double curTime;

				for (; count <= maxCount; count += step)
				{
					curTime = AlpacaMT(&drawArray, count).time;
					if (curTime < bestTime)
					{
						bestCount = count;
						bestTime = curTime;
					}
				}
				threadCount = bestCount;
				std::cout << "Optimal thread count: " << threadCount << std::endl;
				std::cout << "Time: " << bestTime << std::endl;
			}
			cmdIn = "";
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderClear(renderer);
		//Draw nodes
		for (int i = 0; i < nodeArray.size(); i++)
		{
			nodeArray[i].draw(i ,radius);
		}
		//Draw node lines according to Alpaca return
		if (!drawArray.empty())
		{
			int prevNode;
			int nextNode;
			int boxShift;
			for (int i = 1; i < drawArray.size(); i++)
			{
				prevNode = drawArray[i-1];
				nextNode = drawArray[i];
				aalineRGBA(renderer, nodeArray[prevNode].x, nodeArray[prevNode].y, nodeArray[nextNode].x, nodeArray[nextNode].y, 100, 100, 100, 255);
				/*if (abs(nodeArray[prevNode].x - nodeArray[nextNode].x) > abs(nodeArray[prevNode].y - nodeArray[nextNode].y))
				{
					boxShift = nodeArray[nextNode].x - sign(nodeArray[nextNode].x - nodeArray[prevNode].x) * abs(nodeArray[prevNode].y - nodeArray[nextNode].y);
					lineRGBA(renderer, nodeArray[prevNode].x, nodeArray[prevNode].y, boxShift, nodeArray[prevNode].y, 100, 100, 100, 255);
					aalineRGBA(renderer, boxShift, nodeArray[prevNode].y, nodeArray[nextNode].x, nodeArray[nextNode].y, 100, 100, 100, 255);
				}
				else
				{
					boxShift = nodeArray[nextNode].y - sign(nodeArray[nextNode].y - nodeArray[prevNode].y) * abs(nodeArray[prevNode].x - nodeArray[nextNode].x);
					lineRGBA(renderer, nodeArray[prevNode].x, nodeArray[prevNode].y, nodeArray[prevNode].x, boxShift, 100, 100, 100, 255);
					aalineRGBA(renderer, nodeArray[prevNode].x, boxShift, nodeArray[nextNode].x, nodeArray[nextNode].y, 100, 100, 100, 255);
				}*/
			}
		}
		SDL_RenderPresent(renderer);
	}

	SDL_Quit();
	exit(0);
	return 0;
}