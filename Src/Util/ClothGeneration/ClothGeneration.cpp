#include "Src/Util/ClothGeneration/ClothGeneration.hpp"

std::vector<unsigned> genereteIndicies(std::pair<unsigned, unsigned> clothSize)
{
    std::vector<unsigned> clothIndicies;

    for (int y = 1; y < clothSize.second; y++)
    {
        for (int x = 1; x < clothSize.first; x++)
        {
            // First triangle  P3-P1-P2
            unsigned P1 = (y - 1) * clothSize.first + x - 1;
            unsigned P2 = (y - 1) * clothSize.first + x;
            unsigned P3 = y * clothSize.first + x - 1;
            unsigned P4 = y * clothSize.first + x;

            /*
      P1 -  P2
       |  /  |
      P3  - P4
      */

            // Second triangle  P2-P4-P3
            clothIndicies.push_back(P3);
            clothIndicies.push_back(P1);
            clothIndicies.push_back(P2);

            clothIndicies.push_back(P2);
            clothIndicies.push_back(P4);
            clothIndicies.push_back(P3);
        }
    }
    return clothIndicies;
}

std::vector<float> generateClothColor(unsigned particleSize)
{
    std::vector<float> colors;

    for (int x = 0; x < particleSize; x++)
    {
        colors.push_back(x * 0.01f);
        colors.push_back(x * 0.01f);
        colors.push_back(x * 0.01f);
    }
    return colors;
}