#pragma once

#include "Src/Shader/Shader.hpp"
#include "Src/Transform/Transform.hpp"
#include <vector>
class Circle
{
    //Vertex Array buffer
    unsigned VAO; //simply verticies
    // Vertex Buffer object
    unsigned VBO; //simpy object of verticies

    unsigned verticiesSize;
    
public:
    Transform& transform;
    Circle(int verticiesNumber,float angle,Transform& transform);
    ~Circle();
    void Draw(Shader *shaderProgram);
};