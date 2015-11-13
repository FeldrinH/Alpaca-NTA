#pragma once

#include "pathOption.h"

struct shorterFirst
{
	bool operator() (pathOption a, pathOption b)
	{
		return (a.getEstimate() > b.getEstimate());
	}
}shorterSort;

struct longerFirst
{
	bool operator() (pathOption a, pathOption b)
	{
		return (a.getEstimate() < b.getEstimate());
	}
}longerSort;