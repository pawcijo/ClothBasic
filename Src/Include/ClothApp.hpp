#pragma once
#include <Window.hpp>

class ClothApp
{

    Window & windowRef;
    public:

    void run();

    ClothApp(Window &window);
    ~ClothApp() = default;

    unsigned WINDOW_WIDTH = 0;
    unsigned WINDOW_HEIGHT = 0;

  private:

    void processInput();
    void processMouse();
    void processKeys();

};