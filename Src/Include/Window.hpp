#pragma once

// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
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
