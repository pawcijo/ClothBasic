#include "Src/ClothApp/ClothApp.hpp"

ClothApp::ClothApp(Window &window) : windowRef(window)
{
}

void ClothApp::processMouse()
{
}
void ClothApp::processKeys()
{
}
void ClothApp::processInput()
{
    if (glfwGetKey(windowRef.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(windowRef.window, true);
}

void ClothApp::run()
{

    while (!glfwWindowShouldClose(windowRef.window))
    {

        // input
        // -----
        processInput();

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

