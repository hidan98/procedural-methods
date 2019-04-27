#pragma once
#include "D3D.h"
#include <vector>
struct cells;



class Dungeon
{
public:
	Dungeon(Dungeon* Parent);
	~Dungeon();
	void setup(int xPos, int yPos, int width, int height);
	bool intersect(Dungeon* other);
	XMFLOAT3 getCenter() { return centre; }
	

private:
	void cleanUp();
	int width_, depth_, startX, endX, startY, endY;
	XMFLOAT3 centre;
	int generation;
	bool canSplit;
	int minWidth, minDepth;
	int splitKey;
	Dungeon* left;
	Dungeon* right;
	Dungeon* parent;
	std::vector<int> splitHistory;
	friend class DungeonManager;

	

};

