#pragma once
#include "../Window/window.h"
#include "../Rendering system/Text/text.h"
#include "../Program structure/stateMachine.h"
#include "../Program structure/gameAssets.h"

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

	void calculateDeltaTime();
	void calculateRenderDeltaTime();

	Window window;

	EventManager eventManager;
	Keyboard keyboard;
	Mouse mouse;

	Font tmpDefaultFont;
	ShaderProgram textShader;

	GameReference gameReference = std::make_shared<GameAssets>(); //state machine here

	double deltaTime = 0.0f;
	double renderDeltaTime = 0.0f;

	uint32_t fpsCap = 240;
	double fpsCapCooldown; //in seconds
	double fpsCapCooldownLeft;
	double frameDuration = 0.0f;
	Timer timer;

	double fpsMeasureCooldown = 0.5; //in seconds
	double fpsMeasureCooldownLeft = 0.0f;
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

	// czy dana metoda chocia� raz do ko�ca przelecia�a (bez sprawdzenia wywala wyj�tek, bo s�owniki np. name['render'] start/stop trzymaj�ce czas s� puste przed ko�cem pierwszej p�tli)
	// jak b�dzie czas to do poprawy aby co ka�dy loop nie sprawdza�o flagi
	bool loopedInput = false;
	bool loopedRender = false;
	bool loopedUpdate = false;
};

