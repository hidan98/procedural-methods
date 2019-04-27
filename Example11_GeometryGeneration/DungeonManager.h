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

	void caveSetup();

private:
	void setBounds(int gen);
	void deleteCave();
	Dungeon* getDungeon_(Dungeon* dun);

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

	int gen;

	bool done;
};

