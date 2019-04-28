// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{

}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);
	width = 1;
	depth = 1; 
	height = 1;
	textureMgr->loadTexture("brick", L"../res/bunny.png");

	cube = new InstanceCube(renderer->getDevice());
	/*cube->Init2D(renderer->getDevice(), caveGen->getCellMap(), caveGen->getCount(), width, depth);*/
	shader = new InstanceShader(renderer->getDevice(), hwnd);

	bound = new InstanceCube(renderer->getDevice());
	
	//dungeon->bounds(10, 10);
	
	manager = new DungeonManager;
	manager->setup(100, 100, 2);
	bound->init(renderer->getDevice(), manager->getCave(), manager->getCount());
	close = false;


}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.

	delete caveGen;
	caveGen = nullptr;

	delete cube;
	cube = nullptr;

	delete shader;
	shader = nullptr;
}

void App1::Cavestep()
{
	manager->caveStep();// ->life2D();
	//caveGen->step(death, alive, livelim);
	
	bound->init(renderer->getDevice(), manager->getAllCave(), manager->getCaveSize());
	//caveStep->join();
	close = true;
}

bool App1::frame()
{
	bool result;

	if (close)
	{
		caveStep->join();
		close = false;
	}


	if (step)
	{
		caveStep = new std::thread([&] {Cavestep(); });
		//Cavestep();
		
		step = false;
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

	//worldMatrix = XMMatrixScaling(0.10f, 0.10f, 0.100f);
	// Send geometry data, set shader parameters, render object with shader
	cube->sendData(renderer->getDeviceContext());
	shader->setShderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, cube->getIndexCount(), cube->getInstanceCount(), textureMgr->getTexture("brick"));
	shader->render(renderer->getDeviceContext(), cube->getIndexCount(), cube->getInstanceCount());

	bound->sendData(renderer->getDeviceContext());
	shader->setShderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, bound->getIndexCount(), bound->getInstanceCount(), textureMgr->getTexture("brick"));
	shader->render(renderer->getDeviceContext(), bound->getIndexCount(), bound->getInstanceCount());

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
	if (ImGui::Button("Test"))
	{
		regen = true;
	}
	else
		regen = false;

	if (ImGui::Button("Step"))
	{
		step = true;
	}
	else
		step = false;

	if (ImGui::Button("pseudo Life 2D step"))
	{
		caveGen->pseudoLife2D();
		cube->init(renderer->getDevice(), caveGen->getStack(), caveGen->getCount());
	}
	ImGui::InputInt("Width", &width);
	ImGui::InputInt("Depth", &depth);
	ImGui::InputInt("Chance", &chance);
	ImGui::Text("Failed %i", manager->getFailCount());
	
	if (ImGui::InputInt("Camera change", &camNum))
	{
		XMFLOAT3 temp = manager->getCenter(camNum);
		camera->setPosition(temp.x, 10, temp.z);
	}

	if (ImGui::Button("Regen dungeon"))
	{
		manager->setup(200, 200, 5);
		bound->init(renderer->getDevice(), manager->getAllCave(), manager->getCaveSize());
		cube->init(renderer->getDevice(), manager->getCave(), manager->getCount());
	}



	// Render UI
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}