/*
Tu tego
*/

#include <iostream>
#include "Src/Window/Window.hpp"
#include "Src/ClothApp/ClothApp.hpp"
#include "Src/Util/Config/ConfigUtils.hpp"

int main()
{

  ConfigUtils::LoadConfig();

  unsigned WINDOW_WIDTH = ConfigUtils::GetValueFromMap<unsigned>("WINDOW_WIDTH", ConfigUtils::GlobalConfigMap);
  unsigned WINDOW_HEIGHT = ConfigUtils::GetValueFromMap<unsigned>("WINDOW_HEIGHT", ConfigUtils::GlobalConfigMap);
  std::string windowName = "ClothBase";
  Window window(WINDOW_WIDTH, WINDOW_HEIGHT, windowName.c_str());
  ClothApp app(window);

  app.run();
}
