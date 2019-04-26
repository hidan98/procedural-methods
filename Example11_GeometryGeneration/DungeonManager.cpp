#include "DungeonManager.h"
#include <stdlib.h>
#include <time.h>
#include "Cave.h"


DungeonManager::DungeonManager()
{
	srand(time(NULL));
	minWidth = 20;
	minDepth = 20;
	maxWidth = 100; 
	maxDepth = 100;
	done = false;
	dungeons = new std::vector<Dungeon>;
	gen = 1;
}


DungeonManager::~DungeonManager()
{
	dungeons->clear();
	delete dungeons;
	dungeons = nullptr;
}

void DungeonManager::deleteCave()
{
	if (cave)
	{
		delete[] cave;
		cave = nullptr;
		dungeons->clear();
		delete dungeons;
		dungeons = nullptr;
		size = 0;
	}
	
}
void DungeonManager::setup(int width, int depth, int splits, XMFLOAT3 start)
{
	deleteCave();
	dungeons = new std::vector<Dungeon>;
	root = new Dungeon;
	root->setup(start.x, start.z, width, depth);
	root->generation = 1;
	currentGen.push(*root);
	//stack.push(*root);
	

	for (int i = 0; i < splits; i++)
	{
		while (!currentGen.empty())
		{
			split(&currentGen.top());
			currentGen.pop();
		}
		currentGen = nextGen;
		while (!nextGen.empty())
		{
			nextGen.pop();
		}
	}
	while (!currentGen.empty())
	{
		dungeons->push_back(currentGen.top());
		currentGen.pop();
	}
	//dungeons->push_back(*root);
	//size = (width * 2) + (depth * 2);*/
	setBounds();
/*
		int i = 0;
		while (i<3)
		{
			if (stack.top().canSplit)
			{
				split(&stack.top());
				stack.pop();
				i++;
			}
			else
			{
				stack.pop();
			}
		}
	
		
		
	
	setBounds();*/
	/*split(root);
	split(newOne);
	split(newTwo);*/
	delete root;
	root = nullptr;
}
void DungeonManager::split(const Dungeon* dun)
{
	//size = 0;
	int direction = rand() % 2;
	//vertical
	if (direction == 0)
	{
		Dungeon newOne;
		int splitAt = rand() % dun->endX + dun->startX;
		//splitAt++;
		int width = splitAt - dun->startX + 1;
		newOne.setup(dun->startX, dun->startY, width, dun->depth_);
		size += (width * 2) + (dun->depth_ * 2) - 4;
		nextGen.push(newOne);

		Dungeon newTwo;
		width = dun->width_ - width;
		newTwo.setup(splitAt + 1, dun->startY, width, dun->depth_);
		nextGen.push(newTwo);
		size += (width * 2) + (dun->depth_ * 2) - 4;
	}
	else
	{
		Dungeon newOne;
		int splitAt = rand() % dun->endY + dun->startY;
		//splitAt++;
		int depth = splitAt - dun->startY + 1;
		newOne.setup(dun->startX, dun->startY, dun->width_, depth);
		size += (dun->width_ * 2) + (depth * 2) - 4;
		nextGen.push(newOne);

		Dungeon newTwo;
		depth = dun->depth_ - depth;
		newTwo.setup(dun->startX, splitAt + 1, dun->width_, depth);
		nextGen.push(newTwo);
		size += (dun->width_ * 2) + (depth * 2) - 4;
	}
}
void DungeonManager::setup(int number)
{	
	while (!done)
	{
		deleteCave();
		dungeons = new std::vector<Dungeon>;
		int it = 0;
		failCount = 0;
		while (it < number)
		{
			if (failCount > 100000)
			{
				deleteCave();
				dungeons = new std::vector<Dungeon>;
				it = 0;
				failCount = 0;
			}
			int width = minWidth + rand() % (maxWidth - minWidth + 1);
			int depth = minDepth + rand() % (maxDepth - minDepth + 1);

			int startX = 1 + rand() % (400 - width - 1);
			int startY = 1 + rand() % (400 - depth - 1);

			Dungeon temp;
			temp.setup(startX, startY, width, depth);


			bool check = false;
			for (auto dungeon : *dungeons)
			{
				if (dungeon.intersect(&temp))
				{

					failCount++;
					check = true;
					break;
				}
			}
			if (!check)
			{
				size += (width * 2) + ((depth - 2) * 2);
				dungeons->push_back(temp);
				it++;
			}
		}
		done = true;
	}
	

	
	setBounds();
	done = false;
}
void DungeonManager::setBounds()
{

	cave = new cells[size];
	count = 0;
	for (auto dungeon : *dungeons)
	{
		int tempZ = dungeon.startY;
		for (int k = 0; k < 2; k++)
		{
			for (int i = dungeon.startX; i < dungeon.startX + dungeon.width_; i++)
			{
				cave[count].position = XMFLOAT3(i, 0, tempZ);
				count++;
			}
			tempZ = dungeon.endY;
		}
		
		int tempX = dungeon.startX;
		for (int i = 0; i < 2; i++)
		{
			for (int k = dungeon.startY +1; k < dungeon.endY; k++)
			{
				cave[count].position = XMFLOAT3(tempX, 0, k);
				count++;
			}
			tempX = dungeon.endX;
		}
		
	}
}

XMFLOAT3 DungeonManager::getCenter(int num)
{
	//XMFLOAT3 = dungeons->at(num);
	return dungeons->at(num).centre;
}