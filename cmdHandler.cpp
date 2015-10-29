#include <iostream>
#include <string>
#include "segmented_queue.h"
#include <chrono>
#include <thread>

void getCmdIn(std::string* inString, const bool* isRunning)
{
	while (*isRunning)
	{
		std::cin >> *inString;
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}