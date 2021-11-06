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

	fpsCapCooldown = 1.0 / fpsCap;
	fpsCapCooldownLeft = 0.0f;
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

	timer.startTimer("deltaTime");
	timer.startTimer("fps");
	timer.startTimer("previousMeasure");
	while (mainLoopCondition)
	{
		calculateDeltaTime();

		if (timer.getCurrentDurationInSeconds("previousMeasure") >= fpsMeasureCooldown)
		{
			updateFPSThisFrame = true;
		}

		//temporary MVC replacement?
		processInput();
		update();
		render();

		if (updateFPSThisFrame)
		{
			updateFPSText();
		}

	}
}

void Application::processInput()
{
	timer.startTimer("input");

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

	// camera
	if (this->keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyW)]) {
		this->maze->camera->updateInput(timer.getMeasuredDurationInMiliseconds("render"), 0, 0, 0);
	}

	if (this->keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyS)]) {
		this->maze->camera->updateInput(timer.getMeasuredDurationInMiliseconds("render"), 1, 0, 0);
	}

	if (this->keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyA)]) {
		this->maze->camera->updateInput(timer.getMeasuredDurationInMiliseconds("render"), 3, 0, 0);
	}

	if (this->keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyD)]) {
		this->maze->camera->updateInput(timer.getMeasuredDurationInMiliseconds("render"), 2, 0, 0);
	}

	// tymczasowo, trzeba zrobiæ obs³ugê obracania myszk¹
	if (this->keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyZ)]) {
		this->maze->camera->updateInput(timer.getMeasuredDurationInMiliseconds("render"), -1, 10, 0);
	}

	if (this->keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyX)]) {
		this->maze->camera->updateInput(timer.getMeasuredDurationInMiliseconds("render"), -1, -10, 0);
	}

	timer.stopTimer("input");
}

void Application::update()
{
	timer.startTimer("update");

//	this->maze->updateMaze();

	timer.stopTimer("update");
}

void Application::render()
{
	fpsCapCooldownLeft -= deltaTime;
	if (fpsCapCooldownLeft <= 0.0f)
	{
		frameDuration = timer.getCurrentDurationInSeconds("fps");
		fpsCapCooldownLeft = fpsCapCooldown;
		
		timer.startTimer("fps");
		timer.startTimer("render");

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

		timer.stopTimer("render");
	}
}

void Application::updateFPSText()
{
	int32_t fps = static_cast<int32_t>(1.0 / frameDuration);
	fpsValueText.setText(std::move(std::to_string(fps)));

	double inputDuration = timer.getMeasuredDurationInMiliseconds("input");
	std::stringstream streamForInput;
	streamForInput << std::fixed << std::setprecision(4);
	streamForInput << inputDuration;
	inputValueText.setText(streamForInput.str() + "ms");

	double updateDuration = timer.getMeasuredDurationInMiliseconds("update");
	std::stringstream streamForUpdate;
	streamForUpdate << std::fixed << std::setprecision(4);
	streamForUpdate << updateDuration;
	updateValueText.setText(streamForUpdate.str() + "ms");

	double renderDuration = timer.getMeasuredDurationInMiliseconds("render");
	std::stringstream streamForRender;
	streamForRender << std::fixed << std::setprecision(4);
	streamForRender << renderDuration;
	renderValueText.setText(streamForRender.str() + "ms");

	updateFPSThisFrame = false;
	timer.startTimer("previousMeasure");
}

void Application::wireframeModeOn() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Application::wireframeModeOff() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Application::calculateDeltaTime()
{
	deltaTime = timer.getCurrentDurationInSeconds("deltaTime");
	timer.startTimer("deltaTime");

	//std::cout << deltaTime << std::endl;
}
