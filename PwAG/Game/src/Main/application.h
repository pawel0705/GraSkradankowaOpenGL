#pragma once
#include "../Window/window.h"

#pragma region GLFW event callbacks
void framebuffer_size_callback(GLFWwindow* window, const int width, const int height);
void window_close_callback(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
#pragma endregion

class Application
{
	friend void framebuffer_size_callback(GLFWwindow* window, const int width, const int height);
	friend void window_close_callback(GLFWwindow* window);
	friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

public:
	Application();
	~Application();

	void run();

	void processInput();
	void update();
	void render();

private:
	Window window;

	EventManager eventManager;
	Keyboard keyboard;
	Mouse mouse;

	bool mainLoopCondition = true;
};

