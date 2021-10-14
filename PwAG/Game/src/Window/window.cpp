#include "pch.h"
#include "window.h"

Window::Window(uint32_t width, uint32_t height, WindowMode mode, std::string title)
	: width(width), height(height), mode(mode), title(std::move(title))
{
	initialize();
}

Window::~Window()
{
	if(glfwWindowPtr)
	{
		glfwDestroyWindow(glfwWindowPtr);
		glfwWindowPtr = nullptr;
	}
}

void Window::initialize()
{
	initOpenGL();
	createGLFWWindow();
}

void Window::initOpenGL() const
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Window::createGLFWWindow()
{
	switch (mode)
	{
	case WindowMode::eWindowed:
		createWindowedWindow();
		break;
	case WindowMode::eFullscreen:
		createFullscreenWindow();
		break;
	case WindowMode::eWindowFullscreen:
		createWindowedFullscreenWindow();
		break;
	}
}

void Window::createWindowedWindow()
{
	glfwWindowPtr = glfwCreateWindow(width, height, Config::g_defaultWindowTitle, nullptr, nullptr);
	centerWindow();
}

void Window::createFullscreenWindow()
{
	const auto monitor = glfwGetPrimaryMonitor();
	glfwWindowPtr = glfwCreateWindow(width, height, Config::g_defaultWindowTitle, monitor, nullptr);
}

void Window::createWindowedFullscreenWindow()
{
	const auto monitor = glfwGetPrimaryMonitor();
	const auto videoMode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);

	width = videoMode->width;
	height = videoMode->height;
}

void Window::centerWindow() const
{
	if (const auto monitor = glfwGetPrimaryMonitor())
	{
		if (const auto videoMode = glfwGetVideoMode(monitor)) 
		{
			int monitorX, monitorY;
			glfwGetMonitorPos(monitor, &monitorX, &monitorY);

			int windowWidth, windowHeight;
			glfwGetWindowSize(glfwWindowPtr, &windowWidth, &windowHeight);

			glfwSetWindowPos(glfwWindowPtr, monitorX + (videoMode->width - windowWidth) / 2, monitorY + (videoMode->height - windowHeight) / 2);
		}
	}

}
