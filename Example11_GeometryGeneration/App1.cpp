// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"
#include <stdlib.h>
#include <time.h>


App1::App1()
{
	srand(time(NULL));
	lock = false;
	unlock = false;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);
	width = 100;
	depth = 100; 

	shader = new InstanceShader(renderer->getDevice(), hwnd);


	//generate 16 random points
	for (int i = 0; i < 16; i++)
	{
		//geneartes between 0 and 100 and then devides by 100 so its between 0 and 1
		int tempX = rand() % (100 - 0 + 1) + 0;
		float x = (float)tempX / 100.f;

		int tempY = rand() % (100 - 0 + 1) + 0;
		float y = (float)tempY / 100.f;
		positions[i] = XMFLOAT2(x, y);
	}
	timeCollect = 0;
	first = false;

	manager = new DungeonManager;
	manager->setup(100, 100, 2, chance);
	bound = new InstanceCube(renderer->getDevice());
	bound->init(renderer->getDevice(), manager->getBounds(), manager->getCount());

	cave = new InstanceCube(renderer->getDevice());
	cave->init(renderer->getDevice(), manager->getAllCave(), manager->getCaveSize());
	close = false;

	path = new InstanceCube(renderer->getDevice());
	path->init(renderer->getDevice(), manager->getAllPath(), manager->getPathSize());
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	

	if (shader)
	{
		delete shader;
		shader = nullptr;
	}	

	if (cave)
	{
		delete cave;
		cave = nullptr;
	}
	if (bound)
	{
		delete bound;
		bound = nullptr;
	}
	if (path) {
		delete path;
		path = nullptr;
	}
}

void App1::lifeStep()
{
	lock = true;
	mutex_lock.lock();
	//generates new level of cave using life rulls and resets buffer 
	manager->lifeStep();
	cave->init(renderer->getDevice(), manager->getAllCave(), manager->getCaveSize());
	mutex_lock.unlock();
	unlock = true;
}
void App1::pseudoLifeStep()
{
	lock = true;
	mutex_lock.lock();
	//generates new level of cave using pseudoLife rulls and resets buffer 
	manager->pseudoLifestep();
	cave->init(renderer->getDevice(), manager->getAllCave(), manager->getCaveSize());
	mutex_lock.unlock();
	unlock = true;
}

//update function
bool App1::frame()
{
	bool result;
	
	//catches the thread and closes it
	if (unlock)
	{
		caveStep->join();
		lock = false;
		unlock = false;
	}

	result = BaseApplication::frame(); 
	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	//render all caves
	cave->sendData(renderer->getDeviceContext());
	shader->setShderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, cave->getIndexCount(), cave->getInstanceCount(), textureMgr->getTexture("brick"), positions, timeCollect);
	shader->render(renderer->getDeviceContext(), cave->getIndexCount(), cave->getInstanceCount());

	//render dungeon bounds
	bound->sendData(renderer->getDeviceContext());
	shader->setShderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, bound->getIndexCount(), bound->getInstanceCount(), textureMgr->getTexture("brick"), positions, timeCollect);
	shader->render(renderer->getDeviceContext(), bound->getIndexCount(), bound->getInstanceCount());

	//render path
	path->sendData(renderer->getDeviceContext());
	shader->setShderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, path->getIndexCount(), path->getInstanceCount(), textureMgr->getTexture("brick"), positions, timeCollect);
	shader->render(renderer->getDeviceContext(), path->getIndexCount(), path->getInstanceCount());

	// Render GUI
	gui();

	// Swap the buffers
	renderer->endScene();

	return true;
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);
	
	//dungeon spesificatins
	ImGui::InputInt("Width", &width);
	ImGui::InputInt("Depth", &depth);
	ImGui::InputInt("Chance", &chance);
	ImGui::InputInt("Splis", &splits);
	
	//regenerates the dungeon / caves and re sets the buffers that will be sent to the shader
	if (ImGui::Button("Regen dungeon"))
	{
		manager->setup(width, depth, splits, chance);
		bound->init(renderer->getDevice(), manager->getBounds(), manager->getCount());
		cave->init(renderer->getDevice(), manager->getAllCave(), manager->getCaveSize());
		path->init(renderer->getDevice(), manager->getAllPath(), manager->getPathSize());
	}

	//create thread to make new generateion of cave
	if (ImGui::Button("Life step"))
	{
		if(!lock)
			caveStep = new std::thread([&] {lifeStep(); });
	}
	if (ImGui::Button("pseudo Life step"))
	{
		caveStep = new std::thread([&] {pseudoLifeStep(); });
	}



	// Render UI
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}