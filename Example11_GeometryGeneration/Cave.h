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
	void step(int dethLimit, int aliveLimit, int liveLim);
	void stepB678_S345678();
	int getCount(){return count;}

private:
	int getAlive(int x, int y, int z);

	bool* oldCellMap;
	bool* newCellMap;

	bool* cellMap;

	int randNum;
	int count;

	int width_, height_, depth_;
};

