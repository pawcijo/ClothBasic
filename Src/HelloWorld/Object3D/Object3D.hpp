#pragma once

#include "Src/Shader/Shader.hpp"

#include <vector>
class Object3D
{
    //Vertex Array buffer
    unsigned VAO; //simply verticies
    // Vertex Buffer object
    unsigned VBO; //simpy object of verticies
    // Element buffer object
    unsigned EBO; // simply object of indicies

    unsigned indiciesSize;

public:
    Object3D(std::vector<float>& verticies,std::vector<unsigned>& indicies);
    ~Object3D();
    void Draw(Shader *shaderProgram/*,std::vector<glm::vec3>& verticies*/);
};