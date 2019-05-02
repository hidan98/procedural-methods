#pragma once
#include "D3D.h"
#include <vector>
#include "Cave.h"
struct cells;

enum split
{
	horizontal, vertical
};

class Dungeon
{
public:
	Dungeon(Dungeon* Parent);
	~Dungeon();
	void setup(int xPos, int yPos, int width, int height);
	

private:
	void cleanUp();
	int width_, depth_, startX, endX, startY, endY;
	XMFLOAT3 centre;
	int generation;

	
	//stores a pointer to the cave sittin on it
	Cave* cave;

	//used to check if it should be processed
	bool process;

	//nicer to use than ints to figure out how it was split
	split splitType;


	//used to implement tree
	Dungeon* left;
	Dungeon* right;
	Dungeon* parent;

	friend class DungeonManager;

	

};

