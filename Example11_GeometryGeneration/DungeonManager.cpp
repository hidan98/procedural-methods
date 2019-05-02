#include "DungeonManager.h"
#include <stdlib.h>
#include <time.h>
#include "Cave.h"


DungeonManager::DungeonManager()
{
	srand(time(NULL));

	//dungeons = new std::vector<Dungeon>;
	gen = 1;
}


DungeonManager::~DungeonManager()
{
	deleteCave();
}

void DungeonManager::deleteCave()
{
	if (bounds)
	{
		delete[] bounds;
		bounds = nullptr;
		size = 0;
	}

	if (allCaves)
	{
		delete[] allCaves;
		allCaves = nullptr;
		for (auto cave : caves)
		{
			delete cave;
		}
		caves.clear();
		caveSize = 0;

	}
	if (allPath)
	{
		delete[] allPath;
		allPath = nullptr;
	}
	for (auto path : paths)
	{
		delete path;
		path = nullptr;
	}
	paths.clear();
	pathSize = 0;
	bool done = false;
	delete root; 
	root = nullptr;
	dungeons.clear();

	
	final.clear();
	size = 0;
}


void DungeonManager::setup(int width, int depth, int splits, int chance, XMFLOAT3 start)
{
	//if anything exhists delete it
	deleteCave();

	//set up root of tree / orignal size of dungeon
	root = new Dungeon(nullptr);
	root->setup(start.x, start.z, width, depth);
	root->generation = 0;
	//store the pointer to the root
	dungeons.push_back(root);

	bool done = false;
	//loop until we have reached the corecct generation
	while (!done)
	{
		for (int i = 0; i < dungeons.size(); i++)
		{
			//checks the generation is less than the one spesified
			if (dungeons[i]->generation < splits)
			{
				//checks that its children are set to nullpty otherwisw something gine wronge 
				if (dungeons[i]->left == nullptr && dungeons[i]->right == nullptr && dungeons[i]->process)
				{
					//split the dungeon
					split(dungeons[i], dungeons[i]->generation, splits);
				}
			}

		}

		done = true;
	}

	setBounds(splits);
	caveSetup(chance);
	//createPath();
}

//not in use
void DungeonManager::createPath()
{
	//loop through all the final serviving dungeons
	for (auto dun : final)
	{
		//gets the parant of one of the dungeons
		Dungeon* parent = dun->parent;
		
		//depending on the way the dungeon was split will determine how the path is made
		if (dun->splitType == vertical)
		{
			//if both child componants of th parant are valid
			if (parent->left->cave && parent->right->cave)
			{
				//create new path
				paths.push_back(new Path);
				paths.back()->createXpath(parent->left->cave, parent->right->cave);
				pathSize += paths.back()->getSize();
			}
		} // same as above
		else if (dun->splitType == horizontal)
		{
			if (parent->left->cave && parent->right->cave)
			{
				paths.push_back(new Path);
				paths.back()->createPath(parent->left->cave, parent->right->cave);
				pathSize += paths.back()->getSize();
			}
			
		}
		
	}
	//stores all the new paths in an array 
	allPath = new cells[pathSize];
	int it = 0;
	for (auto path : paths)
	{		
		for (int i = 0; i < path->getSize(); i++)
		{
			allPath[it] = path->getPath()[i];
			it++;
		}
	}

}

//setup caves in the space provided by the dungeon
void DungeonManager::caveSetup(int chance)
{
	//loop through all the final generation of dungeons
	for (auto dun : final)
	{
		bool done = false;
		int startX;
		int width;
		//loop until we have a successful placment
		while (!done)
		{
			//random x pos start
			startX = random(dun->startX, dun->endX);
			//random width
			width = random(30, dun->width_ - 10);
			//can get the last position in terms of width from above
			int end = startX + width - 1;
			// if the end position is less than the end position of the dungeon it is in the dungeon 
			if (end < dun->endX)
			{
				done = true;
			}
		}
		int startY;
		int depth;
		done = false;
		//similar as above but in terms of the y start position
		while (!done)
		{
			startY = random(dun->startY, dun->endY);
			depth = random(30, dun->depth_ - 10);
			int end = startY + depth - 1;
			if (end < dun->endY)
			{
				done = true;
			}
		}
		//create new cave
		Cave* temp = new Cave;		
		//assign the dungeon a pointer to the cave
		dun->cave = temp;
		//setup the starting layer of the cave
		temp->initalize2DMap(startX, startY, width, depth, chance);
		caves.push_back(temp);
		caveSize += caves.back()->getCount();
	}

	//after all caves are set up store them all in one array so we can draw them
	allCaves = new cells[caveSize];
	int it = 0;
	for (auto cave : caves)
	{
		for (int i = 0; i < cave->getCount(); i++)
		{
			allCaves[it] = cave->getStack()[i];
			it++;
		}
	}

}

//calls the life step function in all caves stored
void DungeonManager::lifeStep()
{
	caveSize = 0;
	//if anything is stored delete it
	if (allCaves)
	{
		delete[] allCaves;
		allCaves = nullptr;
	}
	
	//loop through all stored caves and generate new layer. store the number cells in cave as well
	for (auto cave : caves)
	{
		cave->life2D();
		caveSize += cave->getCount();
	}
	int it = 0;
	allCaves = new cells[caveSize];
	//store all the new cells in an array so we can render
	for (auto cave : caves)
	{
		for (int i = 0; i < cave->getCount(); i++)
		{
			allCaves[it] = cave->getStack()[i];
			it++;
		}
	}

}

//calls the pseudoLife step function in all caves stored
//code same as above but calls "pseudoLife2D" instead 
void DungeonManager::pseudoLifestep()
{
	caveSize = 0;
	if (allCaves)
	{
		delete[] allCaves;
		allCaves = nullptr;
	}
	

	for (auto cave : caves)
	{
		cave->pseudoLife2D();
		caveSize += cave->getCount();
	}
	int it = 0;
	allCaves = new cells[caveSize];
	for (auto cave : caves)
	{
		for (int i = 0; i < cave->getCount(); i++)
		{
			allCaves[it] = cave->getStack()[i];
			it++;
		}
	}
}

//controls the bsp
void DungeonManager::split(Dungeon* dun, int genNum, int genGoal)
{
	//size = 0;
	//check that the dungeon is suitable to process
	if (dun->process)
	{
		//gets random direction
		int direction = rand() % 2;

		//checks if the dungeon is smaller than the minimum split size 
		if (dun->width_ < 80)
		{
			//if it is smaller but can split in oposite direction manualy set split direction
			if (dun->depth_ >= 80)
			{
				direction = 1;
			}
			else
			{
				//if the dungeon is to small
				//set process to falls so it does not go through loop again
				dun->process = false;
				//since it cant be split it must become the final iteration of the dungeon
				final.push_back(dun);
				//calculate size of bounds so we can render it 
				size += (final.back()->width_ * 2) + (final.back()->depth_ * 2) - 4;
				return;
			}
		}//similar as above but checks oposite direction first
		else if (dun->depth_ < 80)
		{
			if (dun->width_ >= 80)
			{
				direction = 0;
			}
			else
			{				
				final.push_back(dun);
				size += (final.back()->width_ * 2) + (final.back()->depth_ * 2) - 4;
				dun->process = false;
				return;
			}
		}

		//if random direction num is 0 - vertical
		bool done = false;
		if (direction == 0)
		{
			//create two new dungeons
			Dungeon* newOne = new Dungeon(dun);
			Dungeon* newTwo = new Dungeon(dun);
			//loop until we have valid dungeons
			while (!done)
			{
				//random split on x axis
				int splitAt = rand() % (dun->endX - dun->startX + 1) + dun->startX;
				//calculate the width of the split
				int width = splitAt - dun->startX + 1;
				//if the split is greater or equal to 40 it is of the correct size
				if (width >= 40)
				{
					//set up the first dungeon
					newOne->setup(dun->startX, dun->startY, width, dun->depth_);
					newOne->splitType = vertical;
					//find the width of the second new dungeon
					width = dun->width_ - width;
					//if the split is greater or equal to 40 it is of the correct size
					if (width >= 40)
					{
						newTwo->setup(splitAt + 1, dun->startY, width, dun->depth_);
						newTwo->splitType = vertical;
						done = true;
					}
				}
			}
			//set the generation of the dungons
			newOne->generation = genNum + 1;
			newTwo->generation = genNum + 1;
			//set the input dungeons children to be the two new generated dungeons
			dun->left = newOne;
			dun->right = newTwo;
			//store pointer to dungeon in vector
			dungeons.push_back(newOne);
			dungeons.push_back(newTwo);

		}
		else //if random direction num is 1 - horizontal
		{

			//code is same as above but will split on the y axis 
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
					newOne->splitType = horizontal;
					depth = dun->depth_ - depth;
					if (depth >= 40)
					{
						newTwo->setup(dun->startX, splitAt + 1, dun->width_, depth);
						newTwo->splitType = horizontal;
						done = true;
					}
				}
			}


			newOne->generation = genNum + 1;
			newTwo->generation = genNum + 1;

			dun->left = newOne;
			dun->right = newTwo;
			
			dungeons.push_back(newOne);
			dungeons.push_back(newTwo);


		}
	}
	
}
//sets up the dungeon bounds so we can draw them
void DungeonManager::setBounds(int genCheck)
{
	//loops through all the dungeons stored
	for (int i = 0; i < dungeons.size(); i++)
	{
		//grab all the final generation dungeons and store in seperate vector
		if (dungeons[i]->generation == genCheck)
		{
			final.push_back(dungeons[i]);
			//calculate size of cells required to draw
			size += (final.back()->width_ * 2) + (final.back()->depth_ * 2) - 4;
		}
	}
	
	//store new cells of dungeons
	bounds = new cells[size];
	count = 0;
	//loop through final vector
	for (auto dungeon : final)
	{
		//prosess the x axis first
		int tempZ = dungeon->startY;
		//loop twice (top and bottom)
		for (int k = 0; k < 2; k++)
		{
			//set the start positon of the loop to be the start positon i terms of x and loop until we get to the end
			for (int i = dungeon->startX; i < dungeon->endX; i++)
			{
				//we can use i to be the x everything is on y = 0; and z is set to temp z that gets re set
				bounds[count].position = XMFLOAT3(i, 0, tempZ);
				//count is the positon in the array
				count++;
			}
			//adjust z value
			tempZ = dungeon->endY;
		}
		
		int tempX = dungeon->startX;
		//similar as above
		for (int i = 0; i < 2; i++)
		{
			//slight difrence in y start pos so we dont draw a voxal on top of each other
			for (int k = dungeon->startY +1; k < dungeon->endY; k++)
			{
				bounds[count].position = XMFLOAT3(tempX, 0, k);
				count++;
			}
			tempX = dungeon->endX;
		}
		
	}
}

//random number between two points / should be used everywhere but not fully implemented everywhere
int DungeonManager::random(int low, int high)
{
	return rand() % (high - low + 1) + low;
}