#pragma once
#include "Cloth/Model/Cloth.hpp"
#include "Cloth/ClothController/ClothController.hpp"

class ClothDebugInfo
{
public:
    ClothDebugInfo(Cloth &cloth_arg, ClothController &clothController_arg);
    void ShowLastRowInfo();
    void MoveLastRow();

private:
    Cloth &cloth;
    ClothController &clothController;
};