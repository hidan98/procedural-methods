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
	//dungeons = new std::vector<Dungeon>;
	gen = 1;
}


DungeonManager::~DungeonManager()
{
	/*dungeons->clear();
	delete dungeons;
	dungeons = nullptr;*/
}

void DungeonManager::deleteCave()
{
	if (cave)
	{
		delete[] cave;
		cave = nullptr;
		/*dungeons->clear();
		delete dungeons;
		dungeons = nullptr;*/
		size = 0;
	}
	bool done = false;
	delete root; 
	root = nullptr;
	dungeons.clear();
	final.clear();
	
}

void DungeonManager::caveSetup()
{
	for (auto dun : final)
	{

	}
}
void DungeonManager::setup(int width, int depth, int splits, XMFLOAT3 start)
{
	deleteCave();

	root = new Dungeon(nullptr);
	root->setup(start.x, start.z, width, depth);
	root->generation = 0;

	dungeons.push_back(root);

	bool done = false;
	while (!done)
	{
		for(int i =0; i< dungeons.size(); i++)
		{
			if (dungeons[i]->generation < splits + 1)
			{
				if (dungeons[i]->left == nullptr && dungeons[i]->right == nullptr)
				{
					split(dungeons[i], dungeons[i]->generation);
				}
			}

		}

		done = true;
	}

	setBounds(splits + 1);

}

Dungeon* DungeonManager::getDungeon_(Dungeon* dun)
{
	Dungeon* temp;
	if (dun->left)
	{
		temp = getDungeon_(dun->left);
	}
	else if(dun->right)
	{
		temp = getDungeon_(dun->right);
	}
	else
	{
		temp = dun->parent;
	}
	return temp;
}
void DungeonManager::split(Dungeon* dun, int genNum)
{
	//size = 0;
	int direction = rand() % 2;

	if (dun->width_ < 80)
	{
		if (dun->depth_ >= 80)
		{
			direction = 1;
		}
		else
		{
			Dungeon* temp = dun;
			temp->generation = genNum + 1;
			dungeons.push_back(temp);
			return;
		}
	}
	else if (dun->depth_ < 80)
	{
		if (dun->width_ >= 80)
		{
			direction = 0;
		}
		else
		{
			Dungeon* temp = dun;
			temp->generation = genNum + 1;
			dungeons.push_back(temp);
			return;
		}
	}
	//vertical
	bool done = false;
	if (direction == 0)
	{
		Dungeon* newOne = new Dungeon(dun);
		Dungeon* newTwo = new Dungeon(dun);
		while (!done)
		{			
			int splitAt = rand() % (dun->endX - dun->startX + 1) + dun->startX;
			//splitAt++;
			int width = splitAt - dun->startX + 1;
			if (width >= 40)
			{
				newOne->setup(dun->startX, dun->startY, width, dun->depth_);
				width = dun->width_ - width;
				if (width >= 40)
				{
					newTwo->setup(splitAt + 1, dun->startY, width, dun->depth_);
					done = true;
				}
			}
		}

		newOne->generation = genNum + 1;


		dun->left = newOne;
		dun->right = newTwo;

		dungeons.push_back(newOne);
		dungeons.push_back(newTwo);
		newTwo->generation = genNum + 1;

	}
	else
	{
		Dungeon* newOne = new Dungeon(dun);
		Dungeon* newTwo = new Dungeon(dun);
		
		while (!done)
		{
			int splitAt = rand() % (dun->endY - dun->startY + 1) + dun->startY;
			//splitAt++;
			int depth = splitAt - dun->startY + 1;
			if (depth >= 40)
			{
				newOne->setup(dun->startX, dun->startY, dun->width_, depth);
				depth = dun->depth_ - depth;
				if (depth >= 40)
				{
					newTwo->setup(dun->startX, splitAt + 1, dun->width_, depth);
					done = true;
				}
			}
		}
		

		newOne->generation = genNum + 1;
		

		dun->left = newOne;
		dun->right = newTwo;
		newTwo->generation = genNum + 1;
		dungeons.push_back(newOne);
		dungeons.push_back(newTwo);


	}
}
void DungeonManager::setup(int number)
{	
	/*while (!done)
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
	done = false;*/
}
void DungeonManager::setBounds(int genCheck)
{
	size = 0;
	for (int i = 0; i < dungeons.size(); i++)
	{
		if (dungeons[i]->generation == genCheck)
		{
			final.push_back(dungeons[i]);
			size += (final.back()->width_ * 2) + (final.back()->depth_ * 2) - 4;
		}
	}
	
	cave = new cells[size];
	count = 0;
	for (auto dungeon : final)
	{
		int tempZ = dungeon->startY;
		for (int k = 0; k < 2; k++)
		{
			for (int i = dungeon->startX; i < dungeon->startX + dungeon->width_; i++)
			{
				cave[count].position = XMFLOAT3(i, 0, tempZ);
				count++;
			}
			tempZ = dungeon->endY;
		}
		
		int tempX = dungeon->startX;
		for (int i = 0; i < 2; i++)
		{
			for (int k = dungeon->startY +1; k < dungeon->endY; k++)
			{
				cave[count].position = XMFLOAT3(tempX, 0, k);
				count++;
			}
			tempX = dungeon->endX;
		}
		
	}
}

XMFLOAT3 DungeonManager::getCenter(int num)
{
	//XMFLOAT3 = dungeons->at(num);
	return dungeons.at(num)->centre;//.at(num).centre;
}