#pragma once

#include "BoundingBox.h"
#include "../glm/glm.hpp"

class Triangle
{
public:
	BoundingBox getBoundingBox();
	glm::vec3 getCentre();
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
private:
	glm::vec3 a;
	glm::vec3 b;
	glm::vec3 c;

	BoundingBox* bbox;
};