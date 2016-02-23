#pragma once

#include "pathOption.h"
#include <list>
#include <atomic>

class multithread_queue
{
public:
	multithread_queue(int size);
	~multithread_queue();

	void push(pathOption &inPath);
	pathOption pull_back();

	std::atomic_int readPos;
	std::atomic_int writePos;

	bool empty();

private:
	std::vector<pathOption> pathArray;
};

