#pragma once
#include <vector>
class Cave
{
public:
	Cave();
	~Cave();

	void initializeMap(int width, int depth, int height, int chance);
	bool* getCellMap() { return cellMap; }
	void deleteMap();
	void step(int dethLimit, int aliveLimit);
	int getCount(){return count;}

private:
	int getAlive(int x, int y);

	bool* oldCellMap;
	bool* newCellMap;

	bool* cellMap;

	int randNum;
	int count;

	int width_, height_, depth_;
};

