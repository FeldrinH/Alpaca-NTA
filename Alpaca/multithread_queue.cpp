#include "multithread_queue.h"

multithread_queue::multithread_queue(int size)
{
}

multithread_queue::~multithread_queue()
{
}

void multithread_queue::push(pathOption &inPath)
{
	writePos++;
	pathArray[writePos] = inPath;
	readPos++;
}

pathOption multithread_queue::pull_back()
{
	readPos--;
	pathOption out = pathArray[writePos];
	writePos--;
	return out;
}

bool multithread_queue::empty()
{
	return (writePos < 0) && (readPos < 0);
}