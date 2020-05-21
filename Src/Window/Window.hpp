#pragma once
//Glew library that helps in querying and loading OpenGL extensions
#include <GL/glew.h>
// GLFW handle the window and the keyboard
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Window
{
public:
  uint16_t iWidth;
  uint16_t iHeight;

  GLFWwindow *window;
  int Initialise();
  Window(uint16_t Width, uint16_t height, const char *title);
};
