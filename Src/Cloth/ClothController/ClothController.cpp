
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
    for (int i = 0; i < activeCloth.GetParticles().size(); i++)
    {
        vertexInfo.push_back(activeCloth.GetParticles()[i].GetPosition().x);
        vertexInfo.push_back(activeCloth.GetParticles()[i].GetPosition().y);
        vertexInfo.push_back(activeCloth.GetParticles()[i].GetPosition().z);
    }
    return vertexInfo;
}

std::vector<glm::vec4> &ClothController::GetVertexInfo_2()
{
    return activeCloth.getPositionData();
}

Cloth &ClothController::getCloth()
{
    return activeCloth;
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

void ClothController::AddForceToParticle_2(glm::vec3 force, unsigned x, unsigned y)
{
    activeCloth.AddForceToParticle_2(force, x, y);
}
void ClothController::SetForceToParticle_2(glm::vec3 force, unsigned x, unsigned y)
{
    activeCloth.SetForceToParticle_2(force, x, y);
}

void ClothController::Draw(Shader *shader, Transform &transform)
{
    activeCloth.Draw(shader,transform);
}