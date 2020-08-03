#include "Util/ClothDebugInfo/ClothDebugInfo.hpp"

#include <iostream>
#include <limits>
#include <iomanip>

ClothDebugInfo::ClothDebugInfo(Cloth &cloth_arg, ClothController &clothController_arg)
    : cloth(cloth_arg), clothController(clothController_arg)
{
}

void ClothDebugInfo::ShowLastRowInfo()
{
    auto clothSize = cloth.GetClothSize();
    for (int particleRow = 0; particleRow < clothSize.first; particleRow++)
    {
        std::cout << std::fixed << std::setw(4) << std::setprecision(4) << cloth.GetParticle(particleRow, clothSize.second)->GetPosition().y << " ";
    }
    std::cout << "\n";
}

void ClothDebugInfo::MoveLastRow()
{
    auto clothSize = cloth.GetClothSize();
    auto x = clothSize.first * 1.0f / 2.0f;
    auto y = clothSize.second;
    std::cout << "Last row moved :)  [particle : x:" << x << " y:" << y << "]\n";
    clothController.AddForceToParticle(
        glm::vec3(0, 0, 150) *
            0.025f,
        x, y);
}