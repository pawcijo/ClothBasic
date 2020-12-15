#pragma once

#include "Src/Shader/Shader.hpp"
#include "Src/Transform/Transform.hpp"

#include "Src/Util/DrawMode/DrawMode.hpp"

#include <vector>
class SubDataObject
{

    unsigned objectBuffer;
    unsigned vertexArrayObject;
    unsigned colorArrayObject;

    unsigned indiciesSize;
    unsigned objectIndexByteOffset;
    DrawMode drawmode;

public:
    Transform &transform;
    SubDataObject(std::vector<float> verticies,
                  std::vector<float> colors,
                  std::vector<unsigned> indicies,
                  Transform &transform,
                  DrawMode drawmode = DrawMode::EDefault);

    SubDataObject(std::vector<glm::vec4> verticies,
                  std::vector<float> colors,
                  std::vector<unsigned> indicies,
                  Transform &transform,
                  DrawMode drawmode = DrawMode::EDefault);

    ~SubDataObject();
    void Draw(Shader *shaderProgram,
              std::vector<float> verticies,
              std::vector<float> colors,
              std::vector<unsigned> indicies);

    void Draw2(Shader *shader,
               std::vector<glm::vec4> verticies,
               std::vector<float> colors,
               std::vector<unsigned> indicies);
};
