#pragma once
#include "Src/Window/Window.hpp"
#include "Src/Shader/Shader.hpp"
#include "Src/Camera/Camera.hpp"

#include <map>
#include <vector>

static std::map<int,bool> keyPressedStatus;
static bool mouseToUpdate = false;
static double posx;
static double posy;

static glm::vec3 globalCameraPosition;
static float globalCameraYaw;
static float globalCameraPitch;

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

class ClothApp
{

    //Window stuff
    Window & windowRef;
    bool cursorEnabled = false;

    // Camera camera;
    float cameraSpeed = 1;
    //Shader Stuff
    Shader *shader2D = nullptr;
    Shader *shader3D = nullptr;
    Shader *subDataShader3D = nullptr;

    //Camera
    Camera camera;
    glm::mat4 view;
    glm::mat4 projection;
    bool mouseCallBack = true;
    bool firstMouse = true;
    double lastY;
    double lastX;


    //example vertices
    std::vector<float> exampleToUpdate;

    public:

    void run();
    void processKeys();
    void processMouse();
    void setViewPerspective(Camera &aCamera);
    ClothApp(Window &window);
    ~ClothApp() = default;
    
};