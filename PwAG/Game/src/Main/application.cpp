#include "pch.h"
#include "application.h"

Application::Application()
	: tmpDefaultFont(std::move(Font("res/Fonts/Segan.ttf", 18))),
	fpsLabel(0, 880, "FPS:", tmpDefaultFont), fpsValueText(65, 880, "0", tmpDefaultFont),
	inputTimeLabel(0, 860, "Input:", tmpDefaultFont), inputValueText(65, 860, "0", tmpDefaultFont),
	updateTimeLabel(0, 840, "Update:", tmpDefaultFont), updateValueText(65, 840, "0", tmpDefaultFont),
	renderTimeLabel(0, 820, "Render:", tmpDefaultFont), renderValueText(65, 820, "0", tmpDefaultFont)
{
	glfwInit();

	eventManager.registerKeyboard(keyboard);
	eventManager.registerMouse(mouse);

	window.attachEventManager(eventManager);
}

Application::~Application()
{
	glfwTerminate();
}

void Application::run()
{
	this->maze = new Maze();

	Shader textVert = Shader::createShaderFromFile("Shaders/text.vert", Shader::Type::eVertex);
	Shader textFrag = Shader::createShaderFromFile("Shaders/text.frag", Shader::Type::eFragment);

	textShader.attachShader(textVert);
	textShader.attachShader(textFrag);
	textShader.linkShaderProgram();

	lastMeasure = std::chrono::steady_clock::now();
	while (!glfwWindowShouldClose(window.getGLFWWindow()))
	{
		frameStart = std::chrono::steady_clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - lastMeasure).count() >= fpsMeasureCooldown)
		{
			updateFPSThisFrame = true;
		}

		//temporary MVC replacement?
		processInput();
		update();
		render();

		frameEnd = std::chrono::steady_clock::now();

		if (updateFPSThisFrame)
		{
			updateFPSText();
		}
	}
}

void Application::processInput()
{
	inputStart = std::chrono::steady_clock::now();

	eventManager.checkForEvents();
	while (!eventManager.isEventQueueEmpty())
	{
		switch (eventManager.getLatestEventType())
		{
		case EventType::eWindowClosed:
			this->mainLoopCondition = false;
			break;
		}
	}

	// wireframe mode on/off
	if (this->keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyQ)]) {
		this->wireframeMode = true;
		this->wireframeModeOn();
	}
	else if (this->keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyE)]) {
		this->wireframeMode = false;
		this->wireframeModeOff();
	}

	inputEnd = std::chrono::steady_clock::now();
}

void Application::update()
{
	updateStart = std::chrono::steady_clock::now();

	updateEnd = std::chrono::steady_clock::now();
}

void Application::render()
{
	renderStart = std::chrono::steady_clock::now();

	window.clearToColor(80, 80, 80);

	// TODO na razie od razu gra, póŸniej jakaœ maszyna stanów do menu itp

	if (this->wireframeMode) {
		this->wireframeModeOn(); // wireframe mode on
	}

	// RENDER GAME OBJECTS
	this->maze->drawMaze();

	if (this->wireframeMode) {
		this->wireframeModeOff(); // nie chcemy aby text by³ renderowany w wireframe mode, wiêc przed jego renderem ustawiamy na off
	}

	// RENDER TEXT
	textShader.useShader();
	auto projection = glm::ortho(0.0f, static_cast<float>(Config::g_defaultWidth), 0.0f, static_cast<float>(Config::g_defaultHeight));
	textShader.setMat4("MVP", projection);

	fpsLabel.render(textShader);
	fpsValueText.render(textShader);

	inputTimeLabel.render(textShader);
	inputValueText.render(textShader);
	updateTimeLabel.render(textShader);
	updateValueText.render(textShader);
	renderTimeLabel.render(textShader);
	renderValueText.render(textShader);

	window.swapBuffers();

	renderEnd = std::chrono::steady_clock::now();
}

void Application::updateFPSText()
{
	auto frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart).count();
	int32_t fps = static_cast<int32_t>((1.0 / frameDuration) * 1000000.0);
	fpsValueText.setText(std::to_string(fps));

	double inputDuration = std::chrono::duration_cast<std::chrono::microseconds>(inputEnd - inputStart).count() / 1000.0;
	std::stringstream streamForInput;
	streamForInput << std::fixed << std::setprecision(4);
	streamForInput << inputDuration;
	inputValueText.setText(streamForInput.str() + "ms");

	double updateDuration = std::chrono::duration_cast<std::chrono::microseconds>(updateEnd - updateStart).count() / 1000.0;
	std::stringstream streamForUpdate;
	streamForUpdate << std::fixed << std::setprecision(4);
	streamForUpdate << updateDuration;
	updateValueText.setText(streamForUpdate.str() + "ms");

	double renderDuration = std::chrono::duration_cast<std::chrono::microseconds>(renderEnd - renderStart).count() / 1000.0;
	std::stringstream streamForRender;
	streamForRender << std::fixed << std::setprecision(4);
	streamForRender << renderDuration;
	renderValueText.setText(streamForRender.str() + "ms");

	lastMeasure = std::chrono::steady_clock::now();
	updateFPSThisFrame = false;
}

void Application::wireframeModeOn() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Application::wireframeModeOff() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}