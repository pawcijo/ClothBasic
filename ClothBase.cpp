/*
Tu tego
*/

#include <iostream>
#include <Window.hpp>
#include <ClothApp.hpp>

int main()
{
  unsigned WINDOW_WIDTH = 1280;
  unsigned WINDOW_HEIGHT = 720;
  std::string windowName = "ClothBase";
  Window window(WINDOW_WIDTH, WINDOW_HEIGHT, windowName.c_str());
  ClothApp app(window);

  app.run();
}
