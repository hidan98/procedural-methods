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
	width = 10;
	depth = 10; 
	height = 2;
	textureMgr->loadTexture("brick", L"../res/bunny.png");

	caveGen = new Cave();
	caveGen->initializeMap(width, depth, height, 100);
	cube = new InstanceCube(renderer->getDevice());
	cube->init(renderer->getDevice(), caveGen->getCellMap(), caveGen->getCount(), width, depth, height);
	shader = new InstanceShader(renderer->getDevice(), hwnd);

	
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.

}


bool App1::frame()
{
	bool result;

	if (regen)
	{
		caveGen->initializeMap(width, depth, height, chance);
		cube->init(renderer->getDevice(), caveGen->getCellMap(), caveGen->getCount(), width, depth, height);
		regen = false;
	}

	if (step)
	{
		caveGen->stepB678_S345678();
		//caveGen->step(death, alive, livelim);
		cube->init(renderer->getDevice(), caveGen->getCellMap(), caveGen->getCount(), width, depth, height);
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

	//worldMatrix = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	// Send geometry data, set shader parameters, render object with shader
	cube->sendData(renderer->getDeviceContext());
	shader->setShderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, cube->getIndexCount(), cube->getInstanceCount(), textureMgr->getTexture("brick"));
	shader->render(renderer->getDeviceContext(), cube->getIndexCount(), cube->getInstanceCount());

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
	ImGui::InputInt("Width", &width);
	ImGui::InputInt("Height", &height);
	ImGui::InputInt("Depth", &depth);
	ImGui::InputInt("Chance", &chance);
	ImGui::InputInt("lonely limit", &death);
	ImGui::InputInt("overPop limit", &alive);
	ImGui::InputInt("Live again lim", &livelim);


	// Render UI
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

