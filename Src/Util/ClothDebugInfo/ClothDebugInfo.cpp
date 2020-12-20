#include "Util/ClothDebugInfo/ClothDebugInfo.hpp"

#include <iomanip>
#include <iostream>
#include <limits>

ClothDebugInfo::ClothDebugInfo(Cloth &cloth_arg,
                               ClothController &clothController_arg)
    : cloth(cloth_arg), clothController(clothController_arg) {}

void ClothDebugInfo::ShowLastRowInfo()
{

  std::cout<<"CPU last row info : \n";
  auto clothSize = cloth.GetClothSize();
  for (int particleRow = 0; particleRow < clothSize.first; particleRow++)
  {
    std::cout
        << std::fixed << std::setw(4) << std::setprecision(4)
        << cloth.GetParticle(particleRow, clothSize.second-1)->GetPosition().y
        << " ";
  }
  std::cout << "\n";
}

void ClothDebugInfo::ShowMatrixY()
{
  std::cout<<"GPU Y MATRIX info : \n";
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
     std::cout << std::endl;

  }
  std::cout << "\n";
}

void ClothDebugInfo::ShowLastRowInfo_2()
{
  std::cout<<"GPU last row info : \n";
   auto clothSize = cloth.GetClothSize();

  for (unsigned particleIndex = clothSize.first; particleIndex > 0; particleIndex--)
  {
      std::cout
          << std::fixed << std::setw(4) << std::setprecision(4)
          << cloth.getPositionData()[cloth.getPositionData().size()-particleIndex].y
          << " ";

  }
  std::cout << "\n";
}

void ClothDebugInfo::MoveLastRow(float pushForce)
{
  auto clothSize = cloth.GetClothSize();
  auto y = clothSize.second;

  for(int i = 0 ;i< clothSize.first;i++)
  clothController.AddForceToParticle(glm::vec3(0, 0, pushForce * clothSize.first) * 0.25f, i, y - 1);
}

void ClothDebugInfo::MoveLastRow_2(float pushForce)
{
  auto clothSize = cloth.GetClothSize();
  auto x = clothSize.first / 2;
  auto y = clothSize.second;

  clothController.AddForceToParticle_2(glm::vec3(0, 0, pushForce * clothSize.first) * 0.25f, x, y - 1);
}





// To add 

/*
	Print infromation about first 4 constrains: 
	std::cout<<"\n";
		for(int i = 0;i<4;i++)
	{
			int firstVerticleId = cloth1.getConstraintsData()[i].x;
			int secondParticleId = cloth1.getConstraintsData()[i].y;

			float P1_x = cloth1.getPositionData()[firstVerticleId].x;
			float P2_x = cloth1.getPositionData()[secondParticleId].x;

			float P1_y = cloth1.getPositionData()[firstVerticleId].y;
			float P2_y = cloth1.getPositionData()[secondParticleId].y;

			float P1_z = cloth1.getPositionData()[firstVerticleId].z;
			float P2_z = cloth1.getPositionData()[secondParticleId].z;


			std::cout << "Contraint [" << i << "]: \n"
				<< "Indexes : " << firstVerticleId << " | " << secondParticleId << "\n"
				<< "Cords :" << P1_x <<" "<< P1_y <<" "<< P1_z << " | " << P2_x <<" "<< P2_y <<" "<< P2_z << "\n";

	}


		std::cout << "\n" << "CPU :" << "\n";
		for (int i = 0; i < 4; i++)
		{
			int firstVerticleId = cloth1.CPUconstraints[i].p1->getIndex();
			int secondParticleId = cloth1.CPUconstraints[i].p2->getIndex();

			float P1_x = cloth1.CPUparticles[firstVerticleId].GetPosition().x;
			float P2_x = cloth1.CPUparticles[secondParticleId].GetPosition().x;

			float P1_y = cloth1.CPUparticles[firstVerticleId].GetPosition().y;
			float P2_y = cloth1.CPUparticles[secondParticleId].GetPosition().y;

			float P1_z = cloth1.CPUparticles[firstVerticleId].GetPosition().z;
			float P2_z = cloth1.CPUparticles[secondParticleId].GetPosition().z;


			std::cout << "Contraint [" << i << "]: \n"
				<< "Indexes : " << firstVerticleId << " | " << secondParticleId << "\n"
				<< "Cords :" << P1_x << " " << P1_y << " " << P1_z << " | " << P2_x << " " << P2_y << " " << P2_z << "\n";

		}
		
*/