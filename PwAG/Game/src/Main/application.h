#pragma once
#include "../Window/window.h"

class Application
{
public:
	Application();
	~Application();

	void run();

	void processInput();
	void update();
	void render();

private:
	Window window;
};

