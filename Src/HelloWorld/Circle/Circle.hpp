#pragma once

#include "Src/Shader/Shader.hpp"

#include <vector>
class Circle
{
    //Vertex Array buffer
    unsigned VAO; //simply verticies
    // Vertex Buffer object
    unsigned VBO; //simpy object of verticies

    unsigned verticiesSize;
public:
    Circle(int verticiesNumber,float angle);
    ~Circle();
    void Draw(Shader *shaderProgram);
};