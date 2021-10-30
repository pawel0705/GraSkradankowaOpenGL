#include "pch.h"
#include "application.h"

Application::Application()
	: tmpDefaultFont(std::move(Font("res/fonts/Segan.ttf", 64))), testText(100, 810, "Testowe", tmpDefaultFont)
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

	while(!glfwWindowShouldClose(window.getGLFWWindow()))
	{
		//temporary MVC replacement?

		processInput();
		update();
		render();
	}
}

void Application::processInput()
{
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
}

void Application::update()
{
}

void Application::render()
{
	window.clearToColor(80, 80, 80);

	this->maze->DrawMaze();

	textShader.useShader();
	auto projection = glm::ortho(0.0f, static_cast<float>(Config::g_defaultWidth), 0.0f, static_cast<float>(Config::g_defaultHeight));
	textShader.setMat4("MVP", projection);
	
	testText.render(textShader);

	window.swapBuffers();
}
