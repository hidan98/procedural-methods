// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "GeometryShader.h"
#include "InstanceCube.h"
#include "InstanceShader.h"
#include"Cave.h"

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

private:
	GeometryShader* geometryShader;
	PointMesh* mesh;

	InstanceCube* cube;
	InstanceShader* shader;
	Cave* caveGen;

	bool regen;
	bool step;

	int chance;
	int death;
	int alive;
};

#endif