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

void ClothDebugInfo::MoveLastRow() {
  auto clothSize = cloth.GetClothSize();
  auto x = clothSize.first / 2.0f;
  auto y = clothSize.second;
  std::cout << "Last row moved :)  [particle : x:" << x << " y:" << y << "]\n";

  for (int i = 0; i < x; i++) {
    clothController.AddForceToParticle(glm::vec3(0, 0, 300) * 0.25f, i, y-i);
  }
}