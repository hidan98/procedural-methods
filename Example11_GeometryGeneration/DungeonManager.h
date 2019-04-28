#pragma once
#include "Dungeon.h"
#include <vector>
#include "D3D.h"
#include <stack>
#include "Cave.h"
struct cells;

class DungeonManager
{
public:
	DungeonManager();
	~DungeonManager();
	void setup(int number);
	void setup(int width, int depth, int splits, XMFLOAT3 start = XMFLOAT3(0,0,0));
	cells* getCave() { return cave; }
	int getCount() { return count; }
	XMFLOAT3 getCenter(int num);
	int getFailCount() { return failCount; }

	cells* getAllCave() { return allCaves; }
	int getCaveSize() { return caveSize; }

	void caveSetup();
	void caveStep();

private:
	void setBounds(int gen);
	void deleteCave();
	Dungeon* getDungeon_(Dungeon* dun);
	int random(int low, int high);

	void split(Dungeon* dun, int genNum);
	std::vector<Dungeon*> dungeons;
	std::vector<Dungeon*> final;
	int minWidth, minDepth, maxWidth, maxDepth;
	int count;
	cells* cave;
	int size;
	int failCount;

	Dungeon* root;

	std::vector<Cave*> caves;
	cells* allCaves;
	int gen;

	int caveSize;

};

