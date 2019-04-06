#include "Cave.h"
#include <stdlib.h>
#include <time.h>

Cave::Cave()
{
	srand(time(NULL));

	

}


Cave::~Cave()
{
	delete[] cellMap;
	cellMap = nullptr;
}

void Cave::deleteMap()
{
	if (cellMap)
	{
		delete[] cellMap;
		cellMap = nullptr;
	}
}

void Cave::initializeMap(int width, int depth, int height, int chance)
{
	deleteMap();
	width_ = width;
	depth_ = depth;
	height_ = height;
	cellMap = new bool[width_ * depth_];
	
	count = 0;
	
	int index;

	for (int z = 0; z < depth_; z++)
	{		
		for (int x = 0; x < width_; x++)
		{
			index = (width_ * z) + x;
			randNum = rand() % 100;
			if (randNum < chance)
			{
				cellMap[index] = true;
				count++;
			}
			else
				cellMap[index] = false;
		}
	}
}

void Cave::step(int dethLimit, int aliveLimit)
{
	newCellMap = new bool[width_ * height_];
	count = 0;
	int index = 0;
	for (int i = 0; i < width_; i++)
	{
		for (int j = 0; j < depth_; j++)
		{
			index = (width_ * i) + j;
			int aliveNbs = getAlive(j, i);

			if (cellMap[index])
			{
				if (aliveNbs < dethLimit)
				{
					newCellMap[index] = false;
				}
				else
				{
					newCellMap[index] = true;
					count++;
				}
			}
			else
			{
				if (aliveNbs > aliveLimit)
				{
					newCellMap[index] = true;
					count++;
				}
				else
				{
					newCellMap[index] = false;
				}
			}
			
		}
	}
	
	memcpy(cellMap, newCellMap, sizeof(bool) * width_ * depth_);
	delete[] newCellMap;
	newCellMap = nullptr;
	
}

int Cave::getAlive(int x, int y)
{
	int alive = 0;
	for (int j = -1; j < 2; j++)
	{
		for (int i = -1; i < 2; i++)
		{
			int index = (width_ * y + j) + i + x;
			if (j != 0 && i != 0)
			{
				if (index >= 0 && index <= width_ * depth_)
				{
					if (cellMap[index])
					{
						alive++;
					}
				}
			}

		}
	}
	return alive;

}
