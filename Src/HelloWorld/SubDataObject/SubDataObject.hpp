#pragma once

#include "Src/Shader/Shader.hpp"
#include "Src/Transform/Transform.hpp"

#include <vector>
class SubDataObject
{

    unsigned  objectBuffer;
    unsigned  vertexArrayObject;
    unsigned  colorArrayObject;

    unsigned  indiciesSize;
    unsigned  objectIndexByteOffset;

    
public:
    Transform& transform;
    SubDataObject(std::vector<float>& verticies,std::vector<float>& colors,std::vector<unsigned>& indicies,Transform& transform);
  
    ~SubDataObject();
    void Draw(Shader *shaderProgram,
    std::vector<float>& verticies,
    std::vector<float>& colors,
    std::vector<unsigned>& indicies);
};
