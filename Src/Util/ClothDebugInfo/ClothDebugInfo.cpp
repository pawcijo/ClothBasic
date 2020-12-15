#include "Util/ClothDebugInfo/ClothDebugInfo.hpp"

#include <iomanip>
#include <iostream>
#include <limits>

ClothDebugInfo::ClothDebugInfo(Cloth &cloth_arg,
                               ClothController &clothController_arg)
    : cloth(cloth_arg), clothController(clothController_arg) {}

void ClothDebugInfo::ShowLastRowInfo()
{
  auto clothSize = cloth.GetClothSize();
  for (int particleRow = 0; particleRow < clothSize.first; particleRow++)
  {
    std::cout
        << std::fixed << std::setw(4) << std::setprecision(4)
        << cloth.GetParticle(particleRow, clothSize.second)->GetPosition().y
        << " ";
  }
  std::cout << "\n";
}

void ClothDebugInfo::ShowMatrixY()
{
  auto clothSize = cloth.GetClothSize();

  for (int particleRow = 0; particleRow < clothSize.first; particleRow++)
  {
    for (int particleColumn = 0; particleColumn < clothSize.second; particleColumn++)
    {
       unsigned particleNumber = particleRow + (particleColumn * clothSize.first);
      std::cout
          << std::fixed << std::setw(4) << std::setprecision(4)
          << cloth.getPositionData()[particleNumber].y
          << " ";
    }
    if ((particleRow % cloth.GetClothSize().first) == 0)
    {
      std::cout << std::endl;
    }
  }
  std::cout << "\n";
}

void ClothDebugInfo::ShowLastRowInfo_2()
{
  auto clothData = cloth.getPositionData();
  for (int verticleNumber = 0; verticleNumber < clothData.size(); verticleNumber++)
  {
    std::cout
        << std::fixed << std::setw(4) << std::setprecision(4)
        << clothData[verticleNumber].z
        << " ";
    if ((verticleNumber % cloth.GetClothSize().first) == 0)
    {
      std::cout << std::endl;
    }
  }
  std::cout << "\n";
}

void ClothDebugInfo::MoveLastRow(float pushForce)
{
  auto clothSize = cloth.GetClothSize();
  auto x = clothSize.first / 2;
  auto y = clothSize.second;

  clothController.AddForceToParticle(glm::vec3(0, 0, pushForce * clothSize.first) * 0.25f, x, y - 1);
}

void ClothDebugInfo::MoveLastRow_2(float pushForce)
{
  auto clothSize = cloth.GetClothSize();
  auto x = clothSize.first / 2;
  auto y = clothSize.second;

  clothController.AddForceToParticle_2(glm::vec3(0, 0, pushForce * clothSize.first) * 0.25f, x, y - 1);
}