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
	cellMap = new cells[width_ * depth_];
	
	count = 0;
	
	int index;

	for (int x = 0; x < width_; x++)
	{
		for (int z = 0; z < depth_; z++)
		{
			index = x * width_ + z;

			randNum = rand() % 100;
			if (randNum < chance)
			{
				cellMap[index].active = true;
				cellMap[index].position = XMFLOAT3(x, 0, z);
				count++;
			}
			else
			{
				cellMap[index].active = false;
			}
		}
	}

	//for (int x = 0; x < width_; x++)
	//{
	//	for (int y = 0; y < height_; y++)
	//	{
	//		for (int z = 0; z < depth_; z++)
	//		{
	//			index = (x * height_ * depth_) + (y * depth_) + z;
	//			randNum = rand() % 100;
	//			if (randNum < chance)
	//			{
	//				cellMap[index].active = true;
	//				cellMap[index].position = XMFLOAT3(x, y, z);
	//				count++;
	//			}
	//			else
	//				cellMap[index].active = false;
	//		}
	//	}
	//}
	
}
void Cave::stepB678_S345678()
{
	newCellMap = new cells[width_ * depth_ ];
	count = 0;
	int index = 0;


	for(int x = 0; x < width_; x++)
	{
		for (int z = 0; z < depth_; z++)
		{
			index = x * width_ + z;//; *depth_) + (y * depth_) + z;

				int aliveNbs = getAlive(x, 0, z);

				if (cellMap[index].active)
				{
					if (aliveNbs < 3)
					{
						newCellMap[index].active = false;
						newCellMap[index].position = XMFLOAT3(x, 0, z);
						//count++;
					}
					else
					{
						newCellMap[index].active = true;
						newCellMap[index].position = XMFLOAT3(x, 0, z);
						count++;
					}
				}
				else
				{
					if (aliveNbs > 5)
					{
						newCellMap[index].active = true;
						newCellMap[index].position = XMFLOAT3(x, 0, z);
						count++;
					}
					else
					{
						newCellMap[index].active = false;
						newCellMap[index].position = XMFLOAT3(x, 0, z);
					}
				}
			//}
		}
	}

	memcpy(cellMap, newCellMap, sizeof(cells) * width_ * depth_);
	delete[] newCellMap;
	newCellMap = nullptr;

}

void Cave::step(int lonelyLimit, int overPop, int liveLim)
{
	newCellMap = new cells[width_ * depth_ * height_];
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

				if (cellMap[index].active)
				{
					if (aliveNbs < lonelyLimit)
					{
						newCellMap[index].active = false;
						cellMap[index].position = XMFLOAT3(x, y, z);
					}
					else if (aliveNbs > overPop)
					{
						newCellMap[index].active = false;
						cellMap[index].position = XMFLOAT3(x, y, z);
					}
					else
					{
						newCellMap[index].active = true;
						cellMap[index].position = XMFLOAT3(x, y, z);
						count++;
					}
				}
				else
				{
					if (aliveNbs == liveLim)
					{
						newCellMap[index].active = true;
						cellMap[index].position = XMFLOAT3(x, y, z);
						count++;
					}
					else
					{
						newCellMap[index].active = false;
						cellMap[index].position = XMFLOAT3(x, y, z);
					}
				}

			}
		}
	}
	
	
	memcpy(cellMap, newCellMap, sizeof(cells) * width_ * depth_ * height_);
	delete[] newCellMap;
	newCellMap = nullptr;
	
}

int Cave::getAlive(int x, int y, int z)
{
	int alive = 0;
	for (int i = -1; i < 2; i++)
	{
		//for (int j = -1; j < 2; j++)
	//{
			for (int k = -1; k < 2; k++)
			{
				int index = x * width_ + z;
				index += depth_ * i;
				index += k;


				
				if ((i != 0) || (k !=0))
				{
					if (index < 0 || index > width_ * depth_)
					{
						//alive--;
						
					}
					else if(cellMap[index].active)
					{
						alive++;
					}
				}

			//}
		}
	}
	
	return alive;

}
