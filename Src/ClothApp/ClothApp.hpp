#pragma once
#include "Src/Window/Window.hpp"
#include "Src/Shader/Shader.hpp"
#include "Src/Camera/Camera.hpp"

#include <map>
#include <vector>

#include "Cloth/Model/Cloth.hpp"
#include "Cloth/ClothController/ClothController.hpp"
#include "Util/ClothDebugInfo/ClothDebugInfo.hpp"
#include "Src/Util/DrawMode/DrawMode.hpp"
#include "Util/Config/ConfigUtils.hpp"

static std::map<int,bool> keyPressedStatus;
static bool mouseToUpdate = false;
static bool processMouseMovement = true;
static double posx;
static double posy;

static glm::vec3 globalCameraPosition;
static float globalCameraYaw;
static float globalCameraPitch;

const float TIME_STEPSIZE2 = 0.5*0.5;

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);

class ClothApp
{

    //Config 
    ConfigUtils::ConfigLoader config;

    bool cursorEnabled = false;

    // Camera camera;
    float cameraSpeed = 1;
    //Shader Stuff
    Shader *shader2D = nullptr;
    Shader *shader3D = nullptr;
    Shader *subDataShader3D = nullptr;
    Shader *subDataShader3D_2 = nullptr;
    Shader *clothResolveShader = nullptr;
    Shader *clothUpdateShader = nullptr;
    Shader *clothCollisionShader = nullptr;
    

    //Camera
    Camera camera;
    glm::mat4 view;
    glm::mat4 projection;
    bool mouseCallBack = true;
    bool firstMouse = true;
    double lastY;
    double lastX;

    //Window stuff
    Window & windowRef;
  
    unsigned query;
    unsigned query2;

    //example vertices
    std::vector<float> exampleToUpdate;
    //
    std::vector<float> plane;

    //Cloth
    ClothController clothController;
    
    unsigned clothParticleWidth;
    unsigned clothParticleHight;

    //ImGui Params
    int * clothConstraintsResolvePerUpdate = new int(25);
    int * clothStructuralConstraintsRepetition = new int(4);
    int * clothShearAndBendingConstraintsRepetition = new int(2);
    float * springConstant = new float(0.5);
    double  * msPerFrame = new double(0);
    float * gravityForce = new float(-0.4f);
    float *aaBBXPosition = new float(0);
    float *aaBBYPosition = new float(0);
    float *aaBBZPosition = new float(0);
    bool * gpuClothUpdateOn = new bool(true);
    bool * gpuClothResolveConstraintOn = new bool(true);
    bool * gpuClothCollisionOn = new bool(false);
    bool * GpuWireFrameMode = new bool(false);
    bool * CpuWireFrameMode = new bool(false);
    bool * drawAABB = new bool(false);
    bool * CPU_SIMULATION_ON = new bool(false);
    bool * GPU_SIMULATION_ON = new bool(true);
    bool * realTimeCorrection = new bool(false);
    bool * cpuConstantTimeStep = new bool(false);
    
    
    public:
    //Cloth
    ClothDebugInfo clothDebugInfo;
    Cloth cloth1;
    float pushingForce;

    void run();
    void Update();
    void PhysixUpdate(float elapsedTime);
    void processKeys();
    void processMouse();
    void setViewPerspective(Camera &aCamera);
    void ImGuiStuff();
    ClothApp(Window &window);
    ~ClothApp() = default;
    
};
