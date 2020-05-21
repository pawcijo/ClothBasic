#include "Src/ClothApp/ClothApp.hpp"

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
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
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
}

ClothApp::ClothApp(Window &window) : windowRef(window), camera(Camera())
{
    printf("ClothApp created .\n");
    glfwSetKeyCallback(window.window, key_callback);
    glfwSetCursorPosCallback(window.window, cursor_position_callback);
    shaderProgram = new Shader("Shaders/Cloth.vs", "Shaders/Cloth.fs");
}

void ClothApp::run()
{

    while (!glfwWindowShouldClose(windowRef.window))
    {

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(windowRef.window);
        glfwPollEvents();
    }
    glfwTerminate();
}
