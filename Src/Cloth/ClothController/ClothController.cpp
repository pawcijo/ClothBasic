
#include "Cloth/ClothController/ClothController.hpp"

ClothController::ClothController(Cloth &cloth)
    : activeCloth(cloth), XActiveParticle(0), YActiveParticle(0) {}

void ClothController::AddForceToParticle(glm::vec3 force, unsigned x,
                                         unsigned y)
{

    activeCloth.AddForceToParticle(force, x, y);
}

std::vector<float> ClothController::GetVertexInfo()
{
    std::vector<float> vertexInfo;
    for (int i = 0;i<activeCloth.GetParticles().size(); i++)
    {
     vertexInfo.push_back(activeCloth.GetParticles()[i].GetPosition().x);
     vertexInfo.push_back(activeCloth.GetParticles()[i].GetPosition().y);
     vertexInfo.push_back(activeCloth.GetParticles()[i].GetPosition().z);
    }
    return vertexInfo;
}

std::pair<unsigned, unsigned> ClothController::GetClothSize()
{
    return activeCloth.GetClothSize();
}

void ClothController::SetActiveParticle(unsigned x, unsigned y)
{
    XActiveParticle = x;
    YActiveParticle = y;
}
Particle &ClothController::getActiveParticle()
{
    return activeCloth.GetParticles()[YActiveParticle * activeCloth.GetClothSize().first + XActiveParticle];
}

void ClothController::SetForceToParticle(glm::vec3 force, unsigned x,
                                         unsigned y)
{
    activeCloth.SetForceToParticle(force, x, y);
}
