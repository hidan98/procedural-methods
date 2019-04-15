#pragma once
#include <vector>
#include "D3D.h"

struct cells
{
	bool active;
	XMFLOAT3 position;
	XMFLOAT4 colour;
	int num;
};

class Cave
{
public:
	Cave();
	~Cave();

	void initializeMap(int width, int depth, int height, int chance);
	cells* getCellMap() { return cellMap; }
	void deleteMap();
	void step(int dethLimit, int aliveLimit, int liveLim);
	void stepB678_S345678();
	void stepB17_18_19_S13_14_16_();
	int getCount(){return count;}



private:
	int getAlive(int x, int y, int z);

	cells* newCellMap;

	cells* cellMap;

	int randNum;
	int count;

	int width_, height_, depth_;
};

