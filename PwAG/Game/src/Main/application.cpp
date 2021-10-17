#include "pch.h"
#include "application.h"

Application::Application()
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
	window.clearToColor(255, 201, 14);

	window.swapBuffers();
}
