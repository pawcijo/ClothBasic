#pragma once
#include "Cloth/Model/Cloth.hpp"
#include "glm/glm.hpp"

class ClothController
{
    unsigned XActiveParticle;
    unsigned YActiveParticle;
    Cloth & activeCloth;

public:
    ClothController(Cloth & cloth);
    ClothController() = default;

    Particle & getActiveParticle();

    void SetActiveParticle(unsigned x, unsigned );

    std::pair<unsigned,unsigned> GetClothSize();
    std::vector<float> GetVertexInfo();
    std::vector<glm::vec4>& GetVertexInfo_2();

    Cloth & getCloth();

    void AddForceToParticle(glm::vec3 force,unsigned x, unsigned y);
    void SetForceToParticle(glm::vec3 force,unsigned x, unsigned y);

    void AddForceToParticle_2(glm::vec3 force,unsigned x, unsigned y);
    void SetForceToParticle_2(glm::vec3 force,unsigned x, unsigned y);

    void Draw(Shader *shader, Transform &transform,bool wireMode);
};