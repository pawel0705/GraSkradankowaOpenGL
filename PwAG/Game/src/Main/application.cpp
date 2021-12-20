#include "pch.h"
#include "application.h"
#include "../Program structure/gameState.h"
#include "../Program structure/menuState.h"

Application::Application()
	: tmpDefaultFont(std::move(Font("res/Fonts/Segan.ttf", 18))),
	fpsLabel(0, 880, "FPS:", tmpDefaultFont), fpsValueText(65, 880, "0", tmpDefaultFont),
	inputTimeLabel(0, 860, "Input:", tmpDefaultFont), inputValueText(65, 860, "0", tmpDefaultFont),
	updateTimeLabel(0, 840, "Update:", tmpDefaultFont), updateValueText(65, 840, "0", tmpDefaultFont),
	renderTimeLabel(0, 820, "Render:", tmpDefaultFont), renderValueText(65, 820, "0", tmpDefaultFont)
{
	glfwInit();

	ResourceManager::getInstance().loadFont("default", "res/Fonts/Segan.ttf", 32);
	ResourceManager::getInstance().addShaderProgram("text", "Shaders/text.vert", "Shaders/text.frag");

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
	Shader textVert = Shader::createShaderFromFile("Shaders/text.vert", Shader::Type::eVertex);
	Shader textFrag = Shader::createShaderFromFile("Shaders/text.frag", Shader::Type::eFragment);

	textShader.attachShader(textVert);
	textShader.attachShader(textFrag);
	textShader.linkShaderProgram();

	timer.startTimer("deltaTime");
	timer.startTimer("fps");
	timer.startTimer("previousMeasure");

	// tutaj na razie od razu gra, nim si� zrobi menu g��wne
	// by przej�� do innego stanu np. z menu 
    this->gameReference->m_stateMachine.addNewState(StateReference(new MenuState(this->gameReference))); 
	this->gameReference->window = this->window.getGLFWWindow();

	while (mainLoopCondition)
	{
		this->gameReference->m_stateMachine.changingState();

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
		case EventType::eKeyPressed:
			if(keyboard.keyState[static_cast<int>(Keyboard::Key::eKeyF)])
			{

			}
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

	// na razie przesy��m 'render' bo dzia�a lepiej
	if (this->loopedRender) {
		this->gameReference->m_stateMachine.getCurrentState()->processInput(deltaTime, this->keyboard, this->mouse);
	}
	
	timer.stopTimer("input");

	this->loopedInput = true;
}

void Application::update()
{
	timer.startTimer("update");

	if (this->loopedUpdate) {
		this->gameReference->m_stateMachine.getCurrentState()->update(deltaTime);
	}

	timer.stopTimer("update");

	this->loopedUpdate = true;
}

void Application::render()
{
	fpsCapCooldownLeft -= deltaTime;
	if (fpsCapCooldownLeft <= 0.0f)
	{
		frameDuration = timer.getCurrentDurationInSeconds("fps");
		timer.startTimer("fps");

		fpsCapCooldownLeft = fpsCapCooldown;
		
		timer.startTimer("render");

		window.clearToColor(0, 0, 0);

		if (this->wireframeMode) {
			this->wireframeModeOn(); // wireframe mode on
		}

		if (this->loopedRender) {
			this->gameReference->m_stateMachine.getCurrentState()->render(renderDeltaTime);
		}

		if (this->wireframeMode) {
			this->wireframeModeOff(); // nie chcemy aby text by� renderowany w wireframe mode, wi�c przed jego renderem ustawiamy na off
		}

		// RENDER TEXT
	#ifndef DIST
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
	#endif

		window.swapBuffers();

		timer.stopTimer("render");

		this->loopedRender = true;
	}
}

void Application::updateFPSText()
{
#ifndef DIST
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
#endif
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
}

void Application::calculateRenderDeltaTime()
{
	renderDeltaTime = frameDuration;
}
