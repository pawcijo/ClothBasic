#pragma once
#include "Cloth/Model/Cloth.hpp"
#include "Cloth/ClothController/ClothController.hpp"

class ClothDebugInfo
{
public:
    ClothDebugInfo(Cloth &cloth_arg, ClothController &clothController_arg);
    void ShowLastRowInfo();
    void ShowLastRowInfo_2();
    void MoveLastRow(float pushForce);
    void MoveLastRow_2(float pushForce);
    void ShowMatrixY();

private:
    Cloth &cloth;
    ClothController &clothController;
};