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
	cellMap = new bool[width_ * depth_ * height_];
	
	count = 0;
	
	int index;

	for (int x = 0; x < width_; x++)
	{
		for (int y = 0; y < height_; y++)
		{
			for (int z = 0; z < depth_; z++)
			{
				index = (x * height_ * depth_) + (y * depth_) + z;
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
	
}
void Cave::stepB678_S345678()
{
	newCellMap = new bool[width_ * depth_ * height_];
	count = 0;
	int index = 0;

	for (int x = 0; x < width_; x++)
	{
		for (int y = 0; y < height_; y++)
		{
			for (int z = 0; z < depth_; z++)
			{
				index = (x * height_ * depth_) + (y * depth_) + z;

				int aliveNbs = getAlive(x, y, z);

				if (cellMap[index])
				{
					if (aliveNbs < 3)
					{
						newCellMap[index] = false;
						//count++;
					}
					else
					{
						newCellMap[index] = true;
						count++;
					}
				}
				else
				{
					if (aliveNbs > 5)
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
	}

	memcpy(cellMap, newCellMap, sizeof(bool) * width_ * depth_ * height_);
	delete[] newCellMap;
	newCellMap = nullptr;

}

void Cave::step(int lonelyLimit, int overPop, int liveLim)
{
	newCellMap = new bool[width_ * depth_ * height_];
	count = 0;
	int index = 0;
	
	for (int x = 0; x < width_; x++)
	{
		for (int y = 0; y < height_; y++)
		{
			for (int z = 0; z < depth_; z++)
			{
				index = (x * height_ * depth_) + (y * depth_) + z;
				
				int aliveNbs = getAlive(x, y, z);

				if (cellMap[index])
				{
					if (aliveNbs < lonelyLimit)
					{
						newCellMap[index] = false;
					}
					else if (aliveNbs > overPop)
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
					if (aliveNbs == liveLim)
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
	}
	
	
	memcpy(cellMap, newCellMap, sizeof(bool) * width_ * depth_ * height_);
	delete[] newCellMap;
	newCellMap = nullptr;
	
}

int Cave::getAlive(int x, int y, int z)
{
	int alive = 0;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			for (int k = -1; k < 2; k++)
			{
				//int index = (j + z) + height_ * ((k + y) + width_ * (i + x));
				int index = ((x + i) * height_ * depth_) + ((y +j) * depth_) + (z + k);

				//int index = (width_ * y + j) + i + x;
				if (j != 0 && i != 0 && k !=0)
				{
					if (index < 0 || index > width_ * depth_ * height_)
					{
						//alive++;
						
					}
					else if(cellMap[index])
					{
						alive++;
					}
				}

			}
		}
	}
	
	return alive;

}
