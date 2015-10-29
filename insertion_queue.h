#pragma once

#include "pathOption.h"
#include <list>

class insertion_queue
{
public:
	insertion_queue();
	~insertion_queue();

	void push(pathOption inPath);
	pathOption pull_back();

	bool empty();

private:
	std::list<pathOption> pathArray;
};

