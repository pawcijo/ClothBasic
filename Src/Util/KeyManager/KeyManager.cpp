#include "KeyManager.hpp"
#include "Src/ClothApp/ClothApp.hpp"

// WORK IN PROGRES

/*
KeyManager::KeyManager(ClothApp * aClothApp):appPtr(aClothApp)
{
    glfwSetKeyCallback(appPtr->windowRef.window, this->key_callback);
}

void KeyManager::key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_F1 && action == GLFW_PRESS)
    {
        keyPressedStatus[key] = keyPressedStatus[key] ? false : true;
        if (keyPressedStatus[key])
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        printf("keyPressedStatus :  %s \n", keyPressedStatus[key] ? "true" : "false");
    }
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
    {
        printf("Camera Position:  x:%f y:%f z:%f \n", globalCameraPosition.x, globalCameraPosition.y, globalCameraPosition.z);
        printf("Camera yaw:%f pitch:%f \n", globalCameraYaw, globalCameraPitch);
    }
}

*/