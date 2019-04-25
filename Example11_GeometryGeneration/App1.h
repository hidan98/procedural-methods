// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "GeometryShader.h"
#include "InstanceCube.h"
#include "InstanceShader.h"
#include"Cave.h"
#include <thread>
#include "Dungeon.h"
#include "DungeonManager.h"
#include "LinkedList.h"

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

	void Cavestep();

private:
	GeometryShader* geometryShader;
	PointMesh* mesh;

	InstanceCube* cube;
	InstanceCube* bound;
	InstanceShader* shader;
	Cave* caveGen;

	bool regen;
	bool step;

	int chance;
	int death;
	int alive;
	int livelim;

	int width, height, depth;

	std::thread* caveStep;
	bool close;
	Dungeon* dungeon;
	DungeonManager* manager;

	int camNum;
	std::vector<Dungeon>* vec;

};

#endif