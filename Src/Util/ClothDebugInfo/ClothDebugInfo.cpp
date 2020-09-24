#include "Util/ClothDebugInfo/ClothDebugInfo.hpp"

#include <iomanip>
#include <iostream>
#include <limits>

ClothDebugInfo::ClothDebugInfo(Cloth &cloth_arg,
                               ClothController &clothController_arg)
    : cloth(cloth_arg), clothController(clothController_arg) {}

void ClothDebugInfo::ShowLastRowInfo() {
  auto clothSize = cloth.GetClothSize();
  for (int particleRow = 0; particleRow < clothSize.first; particleRow++) {
    std::cout
        << std::fixed << std::setw(4) << std::setprecision(4)
        << cloth.GetParticle(particleRow, clothSize.second)->GetPosition().y
        << " ";
  }
  std::cout << "\n";
}

void ClothDebugInfo::MoveLastRow(float pushForce) {
  auto clothSize = cloth.GetClothSize();
  auto x = clothSize.first /2;
  auto y = clothSize.second;

   clothController.AddForceToParticle(glm::vec3(0, 0, pushForce * clothSize.first) * 0.25f, x, y-1);
}