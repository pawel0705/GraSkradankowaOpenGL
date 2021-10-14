#include "pch.h"
#include "application.h"

Application::Application()
{
	glfwInit();
}

Application::~Application()
{
	glfwTerminate();
}

void Application::run()
{
	bool mainLoopCondition = true;
	while(mainLoopCondition)
	{
		//temporary MVC replacement?

		processInput();
		update();
		render();
	}
}

void Application::processInput()
{
}

void Application::update()
{
}

void Application::render()
{
}
