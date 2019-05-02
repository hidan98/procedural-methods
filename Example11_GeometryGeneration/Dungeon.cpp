#include "Dungeon.h"
#include "Cave.h"
#include <math.h>

Dungeon::Dungeon(Dungeon* Parent) : parent(Parent)
{

	left = nullptr;
	right = nullptr;
	process = true;
}


Dungeon::~Dungeon()
{
	cleanUp();
	
}
//checks if it has any children and deletes them first
void Dungeon::cleanUp()
{
	if (left)
	{
		delete left;
		left = nullptr;
	}
	if (right)
	{
		delete right;
		right = nullptr;
	}
}

//used to setup dungeon
void Dungeon::setup(int xPos, int yPos, int width, int depth)
{
	//start of dungeon x
	startX = xPos;
	endX = xPos + width -1;

	//start of dungeon z
	startY = yPos;
	endY = yPos + depth -1;
	width_ = width;
	depth_ = depth;
	
	centre = XMFLOAT3(floor((startX + endX) / 2), 0, floor((startY + endY) / 2));
}
