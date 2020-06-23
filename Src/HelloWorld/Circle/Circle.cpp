#include "Src/HelloWorld/Circle/Circle.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>

#define MY_PI 3.14159265358979

Circle::Circle(int circleVerticies,float radius,Transform& aTransform): transform(aTransform)
{
    std::vector<float> verticies;
    for (double i = 0; i < 2 * MY_PI; i += 2 * MY_PI / circleVerticies)
    {
        verticies.push_back(cos(i) * radius); //X coordinate
        verticies.push_back(sin(i) * radius); //Y coordinate
        verticies.push_back(0.0);            // Z

        verticies.push_back(0.9);            // R
        verticies.push_back(0.01);           // G
        verticies.push_back(0.01);           // B
    }
    verticiesSize = verticies.size();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), &verticies[0], GL_STATIC_DRAW);

    //position attrib
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    /*
    params:  1 (0) attrib number - layout location
             2 (3) attrib size  [ 3 floats for vec3 x y z]
             3 (GL_FLOAT) type of attrib
             4 (GL_FALSE) if data is normalized
             5 ( 6 * sizeof(float) )  the space between consecutive vertex attributes
             6 ((void *)0) - offset of position data
    */

    //color attrib
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Circle::Draw(Shader *shader)
{
    glUseProgram(shader->shaderProgramID);

    unsigned int transformLoc = glGetUniformLocation(shader->shaderProgramID, "transform");
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform.getTransform()));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, verticiesSize);
    glBindVertexArray(0);
}

Circle::~Circle()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    printf("Circle removed \n");
}
