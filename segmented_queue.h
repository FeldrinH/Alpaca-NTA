#pragma once

#include "pathOption.h"
#include <list>

template <class T> 
class segmented_queue
{
public:
	segmented_queue(int segmentSize)
		:segmentSize(segmentSize)
	{
	}
	~segmented_queue()
	{
	}

	void refreshSegments()
	{
		if (!(segmentArray.empty()))
		{
			for (int i = 0; i < segmentArray.size() - 1; i++)
			{
				if (segmentArray[i].size() > segmentSize)
				{
					//std::cout << "Splicing segment" << std::endl;
					segmentArray[i + 1].splice(segmentArray[i + 1].begin(), segmentArray[i], std::prev(segmentArray[i].end(), segmentArray[i].size() - segmentSize), segmentArray[i].end());
				}
			}
			if (segmentArray.back().size() > segmentSize)
			{
				int i = segmentArray.size() - 1;
				segmentArray.push_back(std::list<T>());
				segmentArray.back().splice(segmentArray.back().begin(), segmentArray[i], std::prev(segmentArray[i].end(), segmentArray[i].size() - segmentSize), segmentArray[i].end());
				//std::cout << "Segments: " << segmentArray.size() << std::endl;
			}
		}
	}

	T front()
	{
		return segmentArray.front().front();
	}

	T back()
	{
		return segmentArray.back().back();
	}

	int segments()
	{
		return segmentArray.size();
	}

	void push(T inPath)
	{
		if (segmentArray.empty())
		{
			segmentArray.push_back(std::list<T>());
			segmentArray[0].push_back(inPath);
			return;
		}

		int i = segmentArray.size()-1;
		while (inPath > segmentArray[i].front() && i > 0)
		{
			--i;
		}
		if (inPath == segmentArray[i].front() || inPath > segmentArray[0].front())
		{
			segmentArray[i].push_front(inPath);
			return;
		}
		else
		{
			std::list<T>::iterator j = std::prev(segmentArray[i].end());
			while (inPath > *j && j != segmentArray[i].begin())
			{
				--j;
			}
			segmentArray[i].insert(++j, inPath);
			return;
		}
	}

	T pull_back()
	{
		T out = segmentArray.back().back();
		segmentArray.back().pop_back();
		if (segmentArray.back().empty())
		{
			segmentArray.pop_back();
			//std::cout << "Segments: " << segmentArray.size() << std::endl;
		}
		return out;
	}

	void pop_back()
	{
		segmentArray.back().pop_back();
		if (segmentArray.back().empty())
		{
			segmentArray.pop_back();
		}
	}

	bool empty()
	{
		return segmentArray.empty();
	}

private:
	std::vector < std::list< T > > segmentArray;
	int segmentSize;
};

