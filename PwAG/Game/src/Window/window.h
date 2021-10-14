#pragma once
#include "windowMode.h"

class Window
{
public:
	explicit Window(uint32_t width = Config::g_defaultWidth, uint32_t height = Config::g_defaultHeight, WindowMode mode = Config::g_defaultWindowMode, std::string title = Config::g_defaultWindowTitle);
	~Window();

#pragma region Inline functions
	void clearToColor(uint8_t red, uint8_t green, uint8_t blue) const
	{
		constexpr float inverseOfMaxValue = 1.0f / 255.0f;
		glClearColor(red * inverseOfMaxValue, green * inverseOfMaxValue, blue * inverseOfMaxValue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void clearToColorf(float red, float green, float blue) const
	{
		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void swapBuffers() const
	{
		glfwSwapBuffers(glfwWindowPtr);
	}
#pragma endregion

private:
	void initialize();

	void initOpenGL() const;
	void createGLFWWindow();

	void createWindowedWindow();
	void createFullscreenWindow();
	void createWindowedFullscreenWindow();

	void centerWindow() const;

	GLFWwindow* glfwWindowPtr{};

	uint32_t width;
	uint32_t height;

	WindowMode mode;

	std::string title;
};