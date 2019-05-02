#pragma once
#include "Dungeon.h"
#include <vector>
#include "D3D.h"
#include <stack>
#include "Cave.h"
#include "Path.h"
struct cells;

class DungeonManager
{
public:
	DungeonManager();
	~DungeonManager();
	void setup(int width, int depth, int splits, int chance, XMFLOAT3 start = XMFLOAT3(0,0,0));
	void caveSetup(int chance);

	void lifeStep();
	void pseudoLifestep();

	cells* getBounds() { return bounds; }
	int getCount() { return count; }

	cells* getAllCave() { return allCaves; }
	int getCaveSize() { return caveSize; }	

	cells* getAllPath() { return allPath; }
	int getPathSize() { return pathSize; }

private:
	void setBounds(int gen);
	void deleteCave();
	void createPath();
	int random(int low, int high);
	void split(Dungeon* dun, int genNum, int genGoal);

	//vectors to store pointers for dungeons
	std::vector<Dungeon*> dungeons;
	std::vector<Dungeon*> final;

	int count;
	//stores all data to render bounds
	cells* bounds;
	int size;
	int failCount;

	//inital dungeon/start of tree
	Dungeon* root;

	std::vector<Cave*> caves;
	cells* allCaves;
	int caveSize;
	int gen;

	std::vector<Path*> paths;
	cells* allPath;
	int pathSize;
	

};

