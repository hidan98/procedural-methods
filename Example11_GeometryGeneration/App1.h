// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "InstanceCube.h"
#include "InstanceShader.h"
#include"Cave.h"
#include <thread>
#include "Dungeon.h"
#include "DungeonManager.h"
#include <mutex>
#include <atomic>

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void gui();

	void lifeStep();
	void pseudoLifeStep();

private:

	PointMesh* mesh;

	//multiple cube set ups, one for each part 
	InstanceCube* cave;
	InstanceCube* bound;
	InstanceCube* path;
	InstanceShader* shader;

	bool regen;
	bool step;

	int chance;
	int splits;

	int width, depth;

	std::thread* caveStep;
	bool close;
	Dungeon* dungeon;
	DungeonManager* manager;


	XMFLOAT2 positions[16];
	float timeCollect;
	bool first;

	std::mutex mutex_lock;
	std::atomic_bool lock;
	std::atomic_bool unlock;
};



#endif