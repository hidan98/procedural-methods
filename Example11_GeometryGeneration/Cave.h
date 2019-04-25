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

	void initializeMap(int width, int depth, int height, int chance);
	void initalize2DMap(int width, int depth, int chance);
	cells* getCellMap() { return cellMap; }
	void deleteMap();
	void step(int dethLimit, int aliveLimit, int liveLim);
	void stepB678_S345678();
	void stepB17_18_19_S13_14_16_();
	int getCount(){return count;}
	cells* getStack() { return stack; }
	int getHeight() { return stepHeight; }

	void life2D();
	void pseudoLife2D();

	int getAlive2D(int x, int z);


private:
	int getAlive(int x, int y, int z);

	cells* newCellMap;

	cells* cellMap;
	cells* stack;
	cells* stackCopy;
	cells* newStack;

	int randNum;
	int count;
	int oldCount;

	int width_, height_, depth_, stepHeight;;
	std::vector<cells> temp;
};

