#pragma once

#include "Src/Shader/Shader.hpp"
#include "Src/Transform/Transform.hpp"

#include "Src/Util/DrawMode/DrawMode.hpp"

#include <vector>
class Circle
{
    //Vertex Array buffer
    unsigned VAO; //simply verticies
    // Vertex Buffer object
    unsigned VBO; //simpy object of verticies

    unsigned verticiesSize;

    DrawMode drawmode;
    
public:
    Transform& transform;
    Circle(int verticiesNumber,float angle,Transform& transform,DrawMode drawmode = DrawMode::EDefault);
    ~Circle();
    void Draw(Shader *shaderProgram);
};