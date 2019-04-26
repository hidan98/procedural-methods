#pragma once
#include "Dungeon.h"
#include <vector>
#include "D3D.h"
#include <stack>
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
	std::vector<Dungeon>* getDungeon() { return dungeons; }
	XMFLOAT3 getCenter(int num);
	int getFailCount() { return failCount; }

private:
	void setBounds();
	void deleteCave();

	void split(const Dungeon* dun);
	std::vector<Dungeon>* dungeons;
	std::vector<Dungeon>* dungeons1;
	int minWidth, minDepth, maxWidth, maxDepth;
	int count;
	cells* cave;
	int size;
	int failCount;

	Dungeon* root;

	std::stack<Dungeon> stack;
	std::stack<Dungeon> currentGen;
	std::stack<Dungeon> nextGen;
	int gen;

	bool done;
};

