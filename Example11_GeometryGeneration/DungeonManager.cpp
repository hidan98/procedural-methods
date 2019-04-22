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

			int startX = 1 + rand() % (200 - width - 1);
			int startY = 1 + rand() % (200 - depth - 1);

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

	//bound = new cells[size];
	cave = new cells[size];
	count = 0;
	for (auto dungeon : *dungeons)
	{
			
		int temp = dungeon.startY;
		for (int k = 0; k < 2; k++)
		{
			for (int i = dungeon.startX; i < dungeon.endX; i++)
			{
				cave[count].active = true;
				cave[count].position = XMFLOAT3(i, 0, temp);
				count++;
			}
			temp = dungeon.endY -1;
		}
		temp = dungeon.startX;
		for (int i = 0; i < 2; i++)
		{
			for (int k = dungeon.startY + 1; k < dungeon.endY - 1; k++)
			{
				cave[count].active = true;
				cave[count].position = XMFLOAT3(temp, 0, k);
				count++;
			}
			temp = dungeon.endX -1;
		}
	}
}

XMFLOAT3 DungeonManager::getCenter(int num)
{
	//XMFLOAT3 = dungeons->at(num);
	return dungeons->at(num).centre;
}