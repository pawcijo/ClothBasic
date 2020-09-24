#pragma once

#include "Src/Shader/Shader.hpp"
#include "Src/Transform/Transform.hpp"

#include "Src/Util/DrawMode/DrawMode.hpp"

#include <vector>
class Object3D
{
    //Vertex Array buffer
    unsigned VAO; //simply verticies (id)
    // Vertex Buffer object
    unsigned VBO; //simpy object of verticies (id)
    // Element buffer object
    unsigned EBO; // simply object of indicies (id)

    unsigned indiciesSize;
     DrawMode drawmode;

    
public:
    Transform& transform;
    Object3D(std::vector<float>& verticies,std::vector<unsigned>& indicies,Transform& transform,DrawMode drawmode = DrawMode::EDefault);
    ~Object3D();
    void Draw(Shader *shaderProgram/*,std::vector<glm::vec3>& verticies*/);
};