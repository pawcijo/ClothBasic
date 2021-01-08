#pragma once
//Glew library that helps in querying and loading OpenGL extensions
#include <GL/glew.h>
// GLFW handle the window and the keyboard
#include <GLFW/glfw3.h>

class ClothApp;


// WORK IN PROGRES
class KeyManager
{
private:
    const ClothApp * appPtr;

public:
    KeyManager(ClothApp * clothApp);
   static void  key_callback(GLFWwindow *window, int, int ,int, int);
};


