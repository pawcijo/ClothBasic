
#include <iostream>
#include "Src/Window/Window.hpp"
#include "Src/ClothApp/ClothApp.hpp"
#include "Src/Util/Config/ConfigUtils.hpp"
#include <map>

int main()
{

  ConfigUtils::ConfigLoader config;

  std::cout<<"Config loaded ..." <<"\n";
  unsigned WINDOW_WIDTH = std::get<unsigned>(config.GetValueFromMap("WINDOW_WIDTH"));
  unsigned WINDOW_HEIGHT = std::get<unsigned>(config.GetValueFromMap("WINDOW_HEIGHT"));
  std::string windowName = "Symulacja tkanin w czasie rzeczywistym";
  Window window(WINDOW_WIDTH, WINDOW_HEIGHT, windowName.c_str());
  std::cout<<"Window  created  ..." <<"\n";
  ClothApp app(window);
  std::cout<<"App  created  ..." <<"\n";
  app.run();
}
