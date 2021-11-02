#pragma once
#include "../Window/window.h"
#include "../Game Objects/maze.h"
#include "../Rendering system/Text/text.h"

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
	void updateFPSText();
	void wireframeModeOn();
	void wireframeModeOff();

	Window window;

	EventManager eventManager;
	Keyboard keyboard;
	Mouse mouse;

	Maze* maze;

	Font tmpDefaultFont;
	ShaderProgram textShader;

	std::chrono::steady_clock::time_point frameStart;
	std::chrono::steady_clock::time_point frameEnd;

	std::chrono::steady_clock::time_point inputStart;
	std::chrono::steady_clock::time_point inputEnd;
	std::chrono::steady_clock::time_point updateStart;
	std::chrono::steady_clock::time_point updateEnd;
	std::chrono::steady_clock::time_point renderStart;
	std::chrono::steady_clock::time_point renderEnd;

	std::chrono::steady_clock::time_point lastMeasure;
	double fpsMeasureCooldown = 500; //in miliseconds
	bool updateFPSThisFrame = false;

	Text fpsLabel;
	Text fpsValueText;

	Text inputTimeLabel;
	Text inputValueText;

	Text updateTimeLabel;
	Text updateValueText;

	Text renderTimeLabel;
	Text renderValueText;

	bool mainLoopCondition = true;

	bool wireframeMode = false;
};

