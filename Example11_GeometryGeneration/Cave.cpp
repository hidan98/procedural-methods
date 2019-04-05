#include "Cave.h"
#include <stdlib.h>
#include <time.h>

Cave::Cave()
{
	srand(time(NULL));

	cellMap = new bool[100* 100];

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

void Cave::initializeMap(int width, int depth, int chance)
{
	//deleteMap();
	count = 0;
	
	int index;
	for (int x = 0; x < width; x++)
	{		
		for (int z = 0; z < depth; z++)
		{
			index = (width * x) + z;
			int randNum = rand() % 100;
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
	newCellMap = new bool[100 * 100];
	count = 0;
	int index = 0;
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			index = (100 * i) + j;
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
	
	memcpy(cellMap, newCellMap, sizeof(bool) * 100 * 100);
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
			int index = (100 * y + j) + i + x;
			if (j != 0 && i != 0)
			{
				if (index >= 0 && index <= 100 * 100)
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
