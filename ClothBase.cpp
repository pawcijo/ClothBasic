
#include <iostream>
#include "Src/Window/Window.hpp"
#include "Src/ClothApp/ClothApp.hpp"
#include "Src/Util/Config/ConfigUtils.hpp"
#include <map>

int main()
{

  ConfigUtils::ConfigLoader config;

  unsigned WINDOW_WIDTH = std::get<unsigned>(config.GetValueFromMap("WINDOW_WIDTH"));
  unsigned WINDOW_HEIGHT = std::get<unsigned>(config.GetValueFromMap("WINDOW_HEIGHT"));
  std::string windowName = "ClothBase";
  Window window(WINDOW_WIDTH, WINDOW_HEIGHT, windowName.c_str());
  ClothApp app(window);
  app.run();
}
