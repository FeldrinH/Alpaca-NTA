#include "insertion_queue.h"

insertion_queue::insertion_queue()
{
}

insertion_queue::~insertion_queue()
{
}

void insertion_queue::push(pathOption &inPath)
{
	
	if(!pathArray.empty())
	{
		std::list<pathOption>::iterator i = --pathArray.end();
		while (inPath > *i && i != pathArray.begin())
		{
			--i;
		}
		pathArray.insert(++i, inPath);
	}
	else
	{
		pathArray.push_back(inPath);
	}
	
}

pathOption insertion_queue::pull_back()
{
	pathOption out = pathArray.back();
	pathArray.pop_back();
	return out;
}

bool insertion_queue::empty()
{
	return pathArray.empty();
}
