#include "Src/ClothApp/ClothApp.hpp"
#include "Src/HelloWorld/Square/Square.hpp"
#include "Src/HelloWorld/Object3D/Object3D.hpp"
#include "Src/HelloWorld/SubDataObject/SubDataObject.hpp"
#include "Src/HelloWorld/Circle/Circle.hpp"
#include "Src/Shapes/Shapes.hpp"
#include <glm/glm.hpp>
#include <cmath>


void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);

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

    if (key == GLFW_KEY_F3 && action == GLFW_PRESS)
    {
        printf("Camera Position:  x:%f y:%f z:%f \n", globalCameraPosition.x, globalCameraPosition.y, globalCameraPosition.z);
        printf("Camera yaw:%f pitch:%f \n",globalCameraYaw, globalCameraPitch);
    }
}

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos)
{
    mouseToUpdate = true;
    posx = xpos;
    posy = ypos;
}

ClothApp::ClothApp(Window &window) : windowRef(window), camera(Camera())
{
    printf("ClothApp created .\n");
    glfwSetKeyCallback(window.window, key_callback);
    glfwSetCursorPosCallback(window.window, cursor_position_callback);
    shader2D = new Shader("Shaders/Cloth.vs", "Shaders/Cloth.fs");
    shader3D = new Shader("Shaders/Cloth3D.vs", "Shaders/Cloth3D.fs");
    subDataShader3D = new Shader("Shaders/SubDataCloth3D.vs", "Shaders/SubDataCloth3D.fs");
    lastX = windowRef.iHeight/2;
    lastY = windowRef.iWidth/2;
}

void ClothApp::processMouse()
{
      if (mouseCallBack)
  {
    if (mouseToUpdate)
    {
       mouseToUpdate = false;
      if (firstMouse)
      {
        lastX = posx;
        lastY = posy;
        firstMouse = false;
      }

      float xoffset = posx - lastX;
      float yoffset =
          lastY - posy; // reversed since y-coordinates go from bottom to top

      lastX = posx;
      lastY = posy;

    camera.ProcessMouseMovement(xoffset, yoffset);
    }
  }
}

void ClothApp::processKeys()
{

    if (glfwGetKey(windowRef.window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.Position += cameraSpeed * camera.Front;
    }

    if (glfwGetKey(windowRef.window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.Position -= cameraSpeed * camera.Front;
    }
    if (glfwGetKey(windowRef.window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.Position -=
            glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
    }
    if (glfwGetKey(windowRef.window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.Position +=
            glm::normalize(glm::cross(camera.Front, camera.Up)) * cameraSpeed;
    }
    if (glfwGetKey(windowRef.window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        camera.Position += cameraSpeed * camera.Up;
    }

    if (glfwGetKey(windowRef.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        camera.Position -= cameraSpeed * camera.Up;
    }
}

void ClothApp::setViewPerspective(Camera &aCamera)
{

    projection = glm::perspective(
        aCamera.Zoom, (float)windowRef.iWidth / (float)windowRef.iHeight,
        0.1f, 10000.0f);
    view = aCamera.GetViewMatrix();

    shader3D->use();
    shader3D->setMat4("projection", projection);
    shader3D->setMat4("view", view);

    subDataShader3D->use();
    subDataShader3D->setMat4("projection", projection);
    subDataShader3D->setMat4("view", view);
}

void ClothApp::run()
{

      camera.Position = glm::vec3(-100, 10, -5);

    Transform cubeTransform = Transform::origin();
    Transform subDataCubeTransform = Transform::origin();
    Transform circleTransform = Transform::origin();

    Object3D cube(Shapes::Cube::vertices, Shapes::Cube::indices, cubeTransform);
    cube.transform.scaleTransform(10, 10, 10);

    SubDataObject subDataCube(Shapes::Cube::vertices, Shapes::Cube::indices, subDataCubeTransform);
    subDataCube.transform.scaleTransform(10, 10, 10);
    subDataCube.transform.translate(glm::vec3(10, 10, 10));

    Circle circle(170, 0.5, circleTransform);
    circleTransform.translate(glm::vec3(-0.93, 0.89, 0));
    circleTransform.scaleTransform(1, (float)windowRef.iWidth / (float)windowRef.iHeight, 1);
    circleTransform.scaleTransform(0.1, 0.1, 0.1);

    unsigned next_tick = (glfwGetTime() * 1000);
    int loops;
    float interpolation = 1.0;
    const int TICKS_PER_SECOND = 64;
    const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
    const int MAX_FRAMESKIP = 5;

    while (!glfwWindowShouldClose(windowRef.window))
    {

        loops = 0;

        while ((glfwGetTime() * 1000) > next_tick && loops < MAX_FRAMESKIP)
        {
            processKeys();
            processMouse();
            globalCameraPosition = camera.Position;
            globalCameraYaw = camera.Yaw;
            globalCameraPitch = camera.Pitch;
            next_tick += SKIP_TICKS;
            loops++;
        }

        interpolation =
            float((glfwGetTime() * 1000) + SKIP_TICKS - next_tick) /
            float(SKIP_TICKS);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //draw 3D
        glEnable(GL_DEPTH_TEST);
        setViewPerspective(camera);

        subDataCube.Draw(subDataShader3D,Shapes::Cube::vertices,Shapes::Cube::indices);
        cube.Draw(shader3D);
    
        glDisable(GL_DEPTH_TEST);
        //draw 2D
        circle.Draw(shader2D);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(windowRef.window);
        glfwPollEvents();
    }
    glfwTerminate();
}
