#include "Src/Util/Utils/Utils.hpp"
#include <stdio.h>

glm::vec3 calculateAABBCenter(std::vector<float> vertices)
{
	glm::vec3 center;
	glm::vec3 position1{vertices[6], vertices[7], vertices[8]};
	glm::vec3 position2{vertices[27], vertices[28], vertices[29]};

	center = glm::vec3{(position1.x + position2.x) / 2.0,
					   (position1.y + position2.y) / 2.0,
					   (position1.z + position2.z) / 2.0};
	return center;
}

void addForceOnX(std::vector<float> &vertices, float force)
{

    for (int i = 0; i < vertices.size(); i += 3)
    {
        vertices[i] += force;
    }
}

void addForceOnY(std::vector<float> &vertices, float force)
{
    for (int i = 1; i < vertices.size(); i += 3)
    {
        vertices[i] += force;
    }
}

void addForceOnZ(std::vector<float> &vertices, float force)
{
    for (int i = 2; i < vertices.size(); i += 3)
    {
        vertices[i] += force;
    }
}

void moveAABB(std::vector<float> &vertices, glm::vec3 force)
{

    if (force.y == 0 && force.z == 0)
    {
        addForceOnX(vertices, force.x);
    }
    else if (force.x == 0 && force.z == 0)
    {
        addForceOnY(vertices, force.y);
    }
    else
    {
        addForceOnZ(vertices, force.z);
    }
}