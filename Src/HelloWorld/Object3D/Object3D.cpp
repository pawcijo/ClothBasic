#include "Src/HelloWorld/Object3D/Object3D.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

Object3D::Object3D(std::vector<float>& verticies,std::vector<unsigned>& indicies)
{

    indiciesSize =  indicies.size() * sizeof(unsigned);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticies.size() * sizeof(float), &verticies[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned), &indicies[0], GL_STATIC_DRAW);

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

void Object3D::Draw(Shader *shader/*std::vector<glm::vec3>& verticies*/)
{
    glUseProgram(shader->shaderProgramID);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indiciesSize, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Object3D::~Object3D()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    printf("Object removed");
}