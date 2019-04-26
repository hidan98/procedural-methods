#include "Dungeon.h"
#include "Cave.h"
#include <math.h>

Dungeon::Dungeon()
{
	minWidth = 20;
	minDepth = 20;
}


Dungeon::~Dungeon()
{
	
}

void Dungeon::setup(int xPos, int yPos, int width, int depth)
{
	startX = xPos;
	endX = xPos + width -1;

	startY = yPos;
	endY = yPos + depth -1;
	width_ = width;
	depth_ = depth;

	if ((depth_ * 2) < minDepth)
	{
		canSplit = false;

	}
	if ((width_ * 2) < minWidth)
	{
		canSplit = false;
	}

	centre = XMFLOAT3(floor((startX + endX) / 2), 0, floor((startY + endY) / 2));
}
//will need to double check this
bool Dungeon::intersect(Dungeon* other)
{
	if ((startX <= other->endX) && (endX >= other->startX) && (startY <= other->endY) && (endY >= other->startY))
	{
		return true;
	}
	return false;
}




//
//void Dungeon::generate(int width, int depth)
//{
//	//for
//}
//
//void Dungeon::bounds(int x, int z)
//{
//	count = 0;
//	x += 2;
//	int size = (x * 2) + (z * 2);
//	
//	bound = new cells[size];
//	int temp = 0;
//	for (int k = 0; k < 2; k++)
//	{
//		for (int i = 0; i < x; i++)
//		{
//			bound[count].active = true;
//			bound[count].position = XMFLOAT3(i, 0, temp);
//			count++;
//		}
//		temp = z +1;
//	}
//	temp = 0;
//	for (int i = 0; i < 2; i++)
//	{
//		for (int k = 0; k < z; k++)
//		{
//			bound[count].active = true;
//			bound[count].position = XMFLOAT3(temp, 0, k+1);
//			count++;
//		}
//		temp = x-1;
//	}
//	
//	
//}
