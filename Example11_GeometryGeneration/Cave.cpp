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
	deleteMap();
}

void Cave::deleteMap()
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
	if (stack)
	{
		delete[] stack;
		stack = nullptr;
	}
	if (newStack)
	{
		delete[] newStack;
		newStack = nullptr;
	}
}



//set up start layer
void Cave::initalize2DMap(int startX, int startY, int width, int depth, int chance)
{
	//if any data is stored delete it all
	deleteMap();
	//start and end values / already been checked for validity
	width_ = width;
	depth_ = depth;
	stepHeight = 1;
	startX_ = startX;
	startY_ = startY;

	endX = startX_ + width_ - 1;
	endY = startY + depth_ - 1;

	//set up array to store top layer with maximum amount needed to store
	cellMap = new cells[width_ * depth_];

	count = 0;

	int index;

	int temp = 0;
	int zPos = startY_;
	//nested loop to interate a 1d array like it is a 2d array 
	for (int z = 0; z < depth_; z++)
	{			
		int xPos = startX_;
		for (int x = 0; x < width_; x++)
		{			
			//index is the positon in the array we are at
			index = (z*width_) + x;
			//randomise number
			int randNum = rand() % 100;
			//if the number generated is less than the input chance it becomes alive
			if (randNum < chance)
			{
				//set alive and store positon on 2d gird
				cellMap[index].active = true;
				cellMap[index].position = XMFLOAT3(xPos, stepHeight, zPos);
				//increase the amount of allive cells 
				count++;				
			}
			else
			{
				//if random number greater than chance it is dead
				cellMap[index].active = false;
			}
			temp++;
			xPos++;
		}
		zPos++;
	}
	//create array for all alive cells
	stack = new cells[count];

	int iterator = 0;
	//loop through top layer 
	for (int z = 0; z < depth_; z++)
	{
		for (int x = 0; x < width_; x++)
		{
			index = (z * width_) + x;
			//if cell is alove store in stack - used to draw all cave cells
			if (cellMap[index].active)
			{
				stack[iterator] = cellMap[index];
				iterator++;
			}
		}
	}
}

//basic game of life rules for 2d - had adaptied 3d version but removes as it did not work
void Cave::life2D()
{
	//store the size of all the alive values
	oldCount = count;
	//increase the height 
	stepHeight++;
	//create a new array for the top layer
	newCellMap = new cells[width_ * depth_];

	int index = 0;
	int zPos = startY_;
	//loop through the old top layer
	for (int z = 0; z < depth_; z++)
	{
		int xPos = startX_;
		for (int x = 0; x < width_; x++)
		{
			//positon in old layer array
			index = (z * width_) + x;
			//checks how many neighbors the cell has
			int aliveNbs = getAlive2D(x, z);

			//if the cell we are checking is alive
			if (cellMap[index].active)
			{
				//if the cell has 2 or 3 alive neighbors the cell stays alive and we store this in the new top layer array
				if ((aliveNbs == 2) || (aliveNbs == 3))
				{
					newCellMap[index].active = true;
					newCellMap[index].position = XMFLOAT3(xPos, stepHeight, zPos);
					count++;
				}
				//any other amount of neighbors it dies, stores this in the new top layer array
				else
				{
					newCellMap[index].active = false;
					newCellMap[index].position = XMFLOAT3(xPos, stepHeight, zPos);
					//count++;
				}
			}
			else // if the cell is dead
			{
				//if the cell has 2 alive neighbors the cell becomes alive and we store this in the new top layer array
				if (aliveNbs == 2)
				{
					newCellMap[index].active = true;
					newCellMap[index].position = XMFLOAT3(xPos, stepHeight, zPos);
					count++;
				}
				else // any other amount it stays dead, we store this in the new layer array
				{
					newCellMap[index].active = false;
					newCellMap[index].position = XMFLOAT3(xPos, stepHeight, zPos);
				}
			}
			xPos++;
		}
		zPos++;
	}


	//not very effcient
	//since we have a new layer we no longer need the old one so we delete it
	if (cellMap)
	{
		delete[] cellMap;
		cellMap = nullptr;
	}
	//we re set up the array we just deleted and put the new array data in it
	cellMap = new cells[width_ * depth_];
	memcpy(cellMap, newCellMap, sizeof(cells) * width_ * depth_ );
	//since the data has been copied we no longer need this array
	if (newCellMap)
	{
		delete[] newCellMap;
		newCellMap = nullptr;
	}
	
	//we create a new stack array this will hold every alive cell from all generations 
	newStack = new cells[count];
	//we copy over all the previous alive cells into the new stack
	memcpy(newStack, stack, sizeof(cells) * oldCount);

	//we delete the old stack array 
	if (stack)
	{
		delete[] stack;
		stack = nullptr;
	}	

	int iterator = oldCount;
	//loop through the top layer array
	for (int z = 0; z < depth_; z++)
	{
		for (int x = 0; x < width_; x++)
		{
			index = (z * width_) + x;
			//places all the active cells in the top layer into the new stack
			if (cellMap[index].active)
			{
				newStack[iterator] = cellMap[index];
				iterator++;
			}
		}
	}

	//set up the original stack array now with the new size and copy all the data over
	stack = new cells[count];
	memcpy(stack, newStack, sizeof(cells) * count);
	//no longer needed

	if (newStack)
	{
		delete[] newStack;
		newStack = nullptr;
	}
	
}

//function basicly same as above, slightly diffrent allive/death rules
void Cave::pseudoLife2D()
{

	oldCount = count;
	stepHeight++;
	newCellMap = new cells[width_ * depth_];
	//count = 0;
	int index = 0;

	int zPos = startY_;
	for (int z = 0; z < depth_; z++)
	{
		int xPos = startX_;
		for (int x = 0; x < width_; x++)
		{
			index = (z * width_) + x;

			int aliveNbs = getAlive2D(x, z);

			if (cellMap[index].active)
			{
				if ((aliveNbs == 2) || (aliveNbs == 3) || (aliveNbs == 8))
				{
					newCellMap[index].active = true;
					newCellMap[index].position = XMFLOAT3(xPos, stepHeight, zPos);
					count++;
				}

				else
				{
					newCellMap[index].active = false;
					newCellMap[index].position = XMFLOAT3(xPos, stepHeight, zPos);
					//count++;
				}
			}
			else
			{
				if ((aliveNbs == 3) || (aliveNbs == 7))
				{
					newCellMap[index].active = true;
					newCellMap[index].position = XMFLOAT3(xPos, stepHeight, zPos);
					count++;
				}
				else
				{
					newCellMap[index].active = false;
					newCellMap[index].position = XMFLOAT3(xPos, stepHeight, zPos);
				}
			}
			xPos++;
		}
		zPos++;
	}
	if (cellMap)
	{
		delete[] cellMap;
		cellMap = nullptr;
	}
	
	cellMap = new cells[width_ * depth_];
	memcpy(cellMap, newCellMap, sizeof(cells) * width_ * depth_);
	if (newCellMap)
	{
		delete[] newCellMap;
		newCellMap = nullptr;
	}
	

	newStack = new cells[count];

	memcpy(newStack, stack, sizeof(cells) * oldCount);
	if (stack)
	{
		delete[] stack;
		stack = nullptr;
	}

	int iterator = oldCount;
	for (int z = 0; z < depth_; z++)
	{
		for (int x = 0; x < width_; x++)
		{
			index = (z * width_) + x;
			if (cellMap[index].active)
			{
				newStack[iterator] = cellMap[index];
				iterator++;
			}
		}
	}


	stack = new cells[count];
	memcpy(stack, newStack, sizeof(cells) * count);

	if (newStack)
	{
		delete[] newStack;
		newStack = nullptr;
	}
	
}

//gets the amount of allive neighbors using Moore neighbourhood
int Cave::getAlive2D(int x, int z)
{
	int alive = 0;
	int index;
	for (int i = -1; i < 2; i++)
	{
		for (int k = -1; k < 2; k++)
		{
			//positon to seach is the positon input into the function + offset from loop
			int tempX = x + k;
			int tempZ = z + i;
			index = (tempZ*width_) + tempX;

			//check that the positon is actualy in the array
			if ((tempX >= 0 && tempX <width_) && (tempZ >= 0 && tempZ <  depth_))
			{
				//dont want to check original cell
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
