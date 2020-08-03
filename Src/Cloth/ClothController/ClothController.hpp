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

    Particle & getActiveParticle();

    void SetActiveParticle(unsigned x, unsigned );

    std::pair<unsigned,unsigned> GetClothSize();
    std::vector<float> GetVertexInfo();

    void AddForceToParticle(glm::vec3 force,unsigned x, unsigned y);
    void SetForceToParticle(glm::vec3 force,unsigned x, unsigned y);
};