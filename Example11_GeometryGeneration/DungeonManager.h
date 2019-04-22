#pragma once
#include "Dungeon.h"
#include <vector>
#include "D3D.h"
struct cells;

class DungeonManager
{
public:
	DungeonManager();
	~DungeonManager();
	void setup(int number);
	cells* getCave() { return cave; }
	int getCount() { return count; }
	std::vector<Dungeon>* getDungeon() { return dungeons; }
	XMFLOAT3 getCenter(int num);
	int getFailCount() { return failCount; }

private:
	void setBounds();
	void deleteCave();
	std::vector<Dungeon>* dungeons;
	int minWidth, minDepth, maxWidth, maxDepth;
	int count;
	cells* cave;
	int size;
	int failCount;

	bool done;
};

