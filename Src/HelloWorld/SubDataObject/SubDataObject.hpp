#pragma once

#include "Src/Shader/Shader.hpp"
#include "Src/Transform/Transform.hpp"

#include "Src/Util/DrawMode/DrawMode.hpp"

#include <vector>
class SubDataObject
{

    unsigned objectBuffer;
    unsigned vertexArrayObject;
    
    unsigned indiciesSize;
    unsigned objectIndexByteOffset;
    DrawMode drawmode;

public:
    Transform &transform;
    SubDataObject(std::vector<float> verticies,
                  std::vector<float> colors,
                  std::vector<unsigned> indicies,
                  Transform &transform);

    SubDataObject(std::vector<glm::vec4> verticies,
                  std::vector<float> colors,
                  std::vector<unsigned> indicies,
                  Transform &transform);

    ~SubDataObject();
    void Draw(Shader *shaderProgram,
              std::vector<float> verticies,
              std::vector<float> colors,
              std::vector<unsigned> indicies,
              bool wireMode);

    void Draw2(Shader *shader,
               std::vector<glm::vec4> verticies,
               std::vector<float> colors,
               std::vector<unsigned> indicies,
               bool wireMode);
};
