#pragma once
#include <vector>
#include "D3D.h"

struct cells
{
	bool active;
	XMFLOAT3 position;
	XMFLOAT4 colour;
};

class Cave
{
public:
	Cave();
	~Cave();

	void initalize2DMap(int startX, int startY, int width, int depth, int chance);
	//return top layer - no longer required as we get the stack array now
	cells* getCellMap() { return cellMap; }
		
	int getCount(){return count;}
	cells* getStack() { return stack; }
	int getHeight() { return stepHeight; }

	void life2D();
	void pseudoLife2D();

	int getAlive2D(int x, int z);
	int getStartX() { return startX_; }
	int getStartY() { return startY_; }
	int getWidth() { return width_; }
	int getDepth() { return depth_; }

private:
	void deleteMap();

	cells* newCellMap;
	cells* cellMap;
	cells* stack;
	cells* newStack;

	int count;
	int oldCount;

	int startX_, startY_;
	int endX, endY;

	int width_, depth_, stepHeight;

};

