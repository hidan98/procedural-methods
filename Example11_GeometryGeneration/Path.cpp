#include "Path.h"

//class not in use

Path::Path()
{
}


Path::~Path()
{
	if (path)
	{
		delete[] path; 
		path = nullptr;
	}
}
//create a ptah in the y drection - last minute does not work
void Path::createPath(Cave* one, Cave* two)
{
	//set stat and end point of path based on the two input caves
	int start = one->getStartX();
	int end = one->getWidth() - 1;

	int siblingStart = two->getStartX();
	int siblingEnd = two->getDepth() - 1;
	
	
	//if the start and end of each cave overlap in some way we can make a straight path
	if (overlap(one->getStartX(), one->getWidth(), two->getStartX(), two->getWidth()))
	{
		int collisionStart, collisionEnd;
		int pathStart = 0;
		//recheck overlap
		if ((start < siblingStart) && (siblingStart < end))
		{
			collisionStart = siblingStart;
			int len = end - siblingStart;
			collisionEnd = siblingStart + len;
			pathStart = random(collisionStart, collisionEnd);
		}
		else if ((siblingStart < start) && (start < siblingEnd))
		{
			collisionStart = start;
			int len = siblingEnd - start;
			collisionEnd = start + len;
			pathStart = random(collisionStart, collisionEnd);
		}

		//cehcks which cave is on top of the other 
		if (pathStart > 0)
		{
			if (one->getStartY() > two->getStartY())
			{
				//find size of path 
				size = one->getStartY() - (two->getStartY() + two->getDepth() - 1);
				//if the size is less than 0 make it possitve
				if (size < 0)
				{
					size *= -1;
				}
				path = new cells[size];
				int it = 0;
				//loop lenght of path and store
				for (int i = two->getStartY() + two->getDepth() - 1; i < one->getStartY(); i++)
				{

					path[it].position = XMFLOAT3(pathStart, 0, i);
					path[it].active = true;
					it++;
				}
			}//similar as above
			else if (two->getStartY() > one->getStartY())
			{
				size = two->getStartY() - (one->getStartY() + one->getDepth() - 1);
				if (size < 0)
				{
					size *= -1;
				}
				path = new cells[size];
				int it = 0;
				for (int i = one->getStartY() + one->getDepth() - 1; i < two->getStartY(); i++)
				{

					path[it].position = XMFLOAT3(pathStart, 0, i);
					path[it].active = true;
					it++;
				}
			}
		}
		

	}
}
//same as above dirrfent direction
void Path::createXpath(Cave* one, Cave* two)
{
	int start = one->getStartY();
	int end = one->getDepth() - 1;

	int siblingStart = two->getStartY();
	int siblingEnd = two->getDepth() - 1;



	if (overlap(one->getStartY(), one->getDepth(), two->getStartY(), two->getDepth()))
	{
		int collisionStart, collisionEnd;
		int pathStart = 0;
		if ((start < siblingStart) && (siblingStart < end))
		{
			collisionStart = siblingStart;
			int len = end - siblingStart;
			collisionEnd = siblingStart + len;
			pathStart = random(collisionStart, collisionEnd);
		}
		else if ((siblingStart < start) && (start < siblingEnd))
		{
			collisionStart = start;
			int len = siblingEnd - start;
			collisionEnd = start + len;
			pathStart = random(collisionStart, collisionEnd);
		}


		if (pathStart > 0)
		{
			if (one->getStartX() > two->getStartX())
			{
				size = one->getStartX() - (two->getStartX() + two->getWidth() - 1);
				if (size < 0)
				{
					size *= -1;
				}
				path = new cells[size];
				int it = 0;
				for (int i = two->getStartX() + two->getWidth() - 1; i < one->getStartX(); i++)
				{

					path[it].position = XMFLOAT3(pathStart, 0, i);
					path[it].active = true;
					it++;
				}
			}
			else if (two->getStartX() > one->getStartX())
			{
				size = two->getStartY() - (one->getStartY() + one->getWidth() - 1);
				if (size < 0)
				{
					size *= -1;
				}
				path = new cells[size];
				int it = 0;
				for (int i = one->getStartX() + one->getWidth() - 1; i < two->getStartX(); i++)
				{

					path[it].position = XMFLOAT3(pathStart, 0, i);
					path[it].active = true;
				}
			}
		}


	}
}

bool Path::overlap(int start1, int width1, int start2, int width2)
{
	return ((start1 + width1 >= start2) && (start2 + width2 >= start1));
}

int Path::random(int low, int high)
{
	return rand() % (high - low + 1) + low;
}
