
#include "Window.hpp"

#include <iostream>

Window::Window(uint16_t Width, uint16_t height, const char *title) : iWidth(Width), iHeight(height)
{

	// Initialise GLFW
	if (!glfwInit())
	{
		printf("glfwInit Fail !!! \n");
	}

	//glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	

	// Open a window and create its OpenGL context
	//window = glfwCreateWindow(Width, height, title, glfwGetPrimaryMonitor(), NULL);
	window = glfwCreateWindow(Width, height, title, NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		printf("Failed to open Window \n");
	}
	glfwMakeContextCurrent(window);

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Turn off V Sync
	glfwSwapInterval(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
}
