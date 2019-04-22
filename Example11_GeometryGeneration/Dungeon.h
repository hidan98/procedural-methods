#pragma once
#include "D3D.h"
struct cells;



class Dungeon
{
public:
	Dungeon();
	~Dungeon();
	void setup(int xPos, int yPos, int width, int height);
	bool intersect(Dungeon* other);
	XMFLOAT3 getCenter() { return centre; }
	

private:
	
	int width_, depth_, startX, endX, startY, endY;
	XMFLOAT3 centre;
	friend class DungeonManager;
	

};

