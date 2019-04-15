#include "Cave.h"
#include <stdlib.h>
#include <time.h>

Cave::Cave()
{
	srand(time(NULL));	
	cellMap = nullptr;
	

}


Cave::~Cave()
{
	if (cellMap)
	{
		delete[] cellMap;
		cellMap = nullptr;
	}
	
	if (newCellMap)
	{
		delete[] newCellMap;
		newCellMap = nullptr;
	}
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
	cellMap = new cells[width_ * depth_ * height_];
	
	count = 0;
	
	int index;

	/*for (int x = 0; x < width_; x++)
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
	}*/
	int temp = 0;
	for (int z = 0; z < depth_; z++)
	{
		for (int y = 0; y < height_; y++)
		{			
			for (int x = 0; x < width_; x++)
			{
				//index = (x * height_ * depth_) + (y * depth_) + z;
				index = (z * height_ * width_) + (y*width_) + x;
				randNum = rand() % 100;
				if (randNum < chance)
				{
					cellMap[index].active = true;
					cellMap[index].position = XMFLOAT3(x, y, z);
					
					count++;
					if (index == 0)
					{
						cellMap[index].colour = XMFLOAT4(1, 0, 0, 1);
					}
					else
					{
						cellMap[index].colour = XMFLOAT4(0, 1, 0, 1);
					}
				}
				else
				{
					cellMap[index].active = false;
					cellMap[index].num = temp;
				}
				temp++;
			}
		}
	}
	
}

void Cave::initalize2DMap(int width, int depth, int chance)
{
	deleteMap();

	width_ = width;
	depth_ = depth;
	height_ = 1;
	stepHeight = 1;
	cellMap = new cells[width_ * depth_ ];
	stack = new cells[width_ * depth_];

	count = 0;

	int index;


	int temp = 0;
	for (int z = 0; z < depth_; z++)
	{		
		for (int x = 0; x < width_; x++)
		{
			index = (z*width_) + x;
			randNum = rand() % 100;
			if (randNum < chance)
			{
				cellMap[index].active = true;
				cellMap[index].position = XMFLOAT3(x, stepHeight, z);

				count++;				
			}
			else
			{
				cellMap[index].active = false;
				cellMap[index].num = temp;
			}
			temp++;
		}
		
	}

	memcpy(stack, cellMap, sizeof(cells) * width_ * depth_ * stepHeight);
}

void Cave::stepB17_18_19_S13_14_16_()
{
	newCellMap = new cells[width_* depth_ * height_];
	count = 0;
	int index = 0;


	for (int z = 0; z < depth_; z++)
	{
		for (int y = 0; y < height_; y++)
		{
			for (int x = 0; x < width_; x++)
			{
				index = (z * height_ * width_) + (y*width_) + x;

				int aliveNbs = getAlive(x, y, z);

				
				if (cellMap[index].active)
				{
					if ((aliveNbs > 12))
					{
						newCellMap[index].active = false;
						newCellMap[index].position = XMFLOAT3(x, y, z);
						//count++;
					}
					else
					{
						newCellMap[index].active = true;
						newCellMap[index].position = XMFLOAT3(x, y, z);
						count++;
					}
				}
				else
				{
					if ((aliveNbs == 14) || (aliveNbs == 17) || (aliveNbs == 18) || (aliveNbs == 19))
					{
						newCellMap[index].active = true;
						newCellMap[index].position = XMFLOAT3(x, y, z);
						count++;
					}
					else
					{
						newCellMap[index].active = false;
						newCellMap[index].position = XMFLOAT3(x, y, z);
					}
				}
			}
		}
	}

	memcpy(cellMap, newCellMap, sizeof(cells) * width_ * depth_ * height_);
	delete[] newCellMap;
	newCellMap = nullptr;
}
void Cave::stepB678_S345678()
{
	newCellMap = new cells[width_ * depth_ * height_];
	count = 0;
	int index = 0;


	for (int z = 0; z < depth_; z++)
	{
		for (int y = 0; y < height_; y++)
		{
			for (int x = 0; x < width_; x++)
			{
				index = (z * height_ * width_) + (y*width_) + x;

				int aliveNbs = getAlive(x, y, z);

				if (cellMap[index].active)
				{
					if (aliveNbs < 3)
					{
						newCellMap[index].active = false;
						newCellMap[index].position = XMFLOAT3(x, y, z);
						//count++;
					}
					else
					{
						newCellMap[index].active = true;
						newCellMap[index].position = XMFLOAT3(x, y, z);
						count++;
					}
				}
				else
				{
				if (aliveNbs > 5)
				{
					newCellMap[index].active = true;
					newCellMap[index].position = XMFLOAT3(x, y, z);
					count++;
				}
				else
				{
					newCellMap[index].active = false;
					newCellMap[index].position = XMFLOAT3(x, y, z);
				}
				}
			}
		}
	}

	memcpy(cellMap, newCellMap, sizeof(cells) * width_ * depth_ * height_);
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

				int aliveNbs = getAlive(x, y , z);

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


	memcpy(cellMap, newCellMap, sizeof(cells) * width_ * depth_ * stepHeight);
	delete[] newCellMap;
	newCellMap = nullptr;

	

}

void Cave::life2D()
{
	stepHeight++;
	newCellMap = new cells[width_ * depth_];
	//count = 0;
	int index = 0;

	for (int z = 0; z < depth_; z++)
	{
		
		for (int x = 0; x < width_; x++)
		{
			index = (z * width_) + x;

			int aliveNbs = getAlive2D(x, z);

			if (cellMap[index].active)
			{
				if ((aliveNbs == 2) || (aliveNbs == 3))
				{
					newCellMap[index].active = true;
					newCellMap[index].position = XMFLOAT3(x, stepHeight, z);
					count++;
				}
				
				else
				{
					newCellMap[index].active = false;
					newCellMap[index].position = XMFLOAT3(x, stepHeight, z);
					//count++;
				}
			}
			else
			{
				if (aliveNbs == 2)
				{
					newCellMap[index].active = true;
					newCellMap[index].position = XMFLOAT3(x, stepHeight, z);
					count++;
				}
				else
				{
					newCellMap[index].active = false;
					newCellMap[index].position = XMFLOAT3(x, stepHeight, z);
				}
			}
		}
	}

	memcpy(cellMap, newCellMap, sizeof(cells) * width_ * depth_ );
	delete[] newCellMap;
	newCellMap = nullptr;

	
	newStack = new cells[width_* depth_ * stepHeight];
	

	memcpy(newStack, stack, sizeof(cells) * width_ * depth_ * (stepHeight -1 ));
	delete[] stack;
	stack = nullptr;


	

	int temp = 0;

	for (int z = 0; z < depth_; z++)
	{
		for (int x = 0; x < width_; x++)
		{
			index = (z * width_) + x;
			int newIndex;
			
			newIndex = (index) + (width_) * (depth_) * (stepHeight -1);			 
			
			newStack[newIndex] = cellMap[index];
		}

	}
	/*for (int z = 0; z < depth_; z++)
	{
		for (int y = stepHeight - 1; y < stepHeight; y++)
		{
			for (int x = 0; x < width_; x++)
			{
				index = (z * stepHeight * width_) + (y*width_) + x;
				newStack[index] = cellMap[temp];
				temp++;
			}
		}
	}*/
	stack = new cells[width_ * depth_ * stepHeight];
	memcpy(stack, newStack, sizeof(cells) * width_ * depth_ * stepHeight);
	delete[] newStack;
	newStack = nullptr;
}

int Cave::getAlive(int x, int y, int z)
{
	int alive = 0;
	int index;
	for (int i = -1; i < 2; i++)
	{
		for (int j = -1; j < 2; j++)
		{
			for (int k = -1; k < 2; k++)
			{
				int tempX = x + k;
				int tempY = y + j;
				int tempZ = z + i;
				index = (tempZ * height_ * width_) + (tempY*width_) + tempX;

				if ((tempX >= 0 && tempX <width_) && (tempY >= 0 && tempY < height_) && (tempZ >= 0 && tempZ <  depth_))
				{

					if ((i != 0) || (k != 0) || (j != 0))
					{
						if (index < 0 || index > width_ * depth_ * height_ -1)
						{
							//alive--;

						}
						else if (cellMap[index].active)
						{
							
							alive++;
						}
					}
				}
				
				

			}
		}
	}
	
	return alive;

}

int Cave::getAlive2D(int x, int z)
{
	int alive = 0;
	int index;
	for (int i = -1; i < 2; i++)
	{
		for (int k = -1; k < 2; k++)
		{
			int tempX = x + k;
			int tempZ = z + i;
			index = (tempZ*width_) + tempX;

			if ((tempX >= 0 && tempX <width_) && (tempZ >= 0 && tempZ <  depth_))
			{

				if ((i != 0) || (k != 0))
				{
					if (index < 0 || index > width_ * depth_ - 1)
					{
						//alive--;

					}
					else if (cellMap[index].active)
					{

						alive++;
					}
				}
			}
		}
		
	}

	return alive;
}
