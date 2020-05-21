#pragma once
#include "Src/Window/Window.hpp"
#include "Src/Shader/Shader.hpp"
#include "Src/Camera/Camera.hpp"

#include <map>

static std::map<int,bool> keyPressedStatus;
class ClothApp
{

    //Window stuff
    Window & windowRef;
    bool cursorEnabled = false;

    Camera camera;
    float cameraSpeed = 1;
    //Shader Stuff
    Shader *shaderProgram = nullptr;

    public:
    void run();
    ClothApp(Window &window);
    ~ClothApp() = default;
    
    unsigned WINDOW_WIDTH = 0;
    unsigned WINDOW_HEIGHT = 0;
};