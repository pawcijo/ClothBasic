#pragma once

#include <glm/glm.hpp>
#include <vector>
glm::vec3 calculateAABBCenter(std::vector<float> vertices);
void moveAABB(std::vector<float> &vertices, glm::vec3 force);
