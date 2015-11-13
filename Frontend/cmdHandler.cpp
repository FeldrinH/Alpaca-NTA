#include <iostream>
#include <string>
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