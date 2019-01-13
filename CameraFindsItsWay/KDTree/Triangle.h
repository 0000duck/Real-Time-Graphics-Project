#pragma once

#include "BoundingBox.h"
#include "../glm/glm.hpp"
#include "Ray.h"

class Triangle
{
public:
	BoundingBox getBoundingBox();
	glm::vec3 getCentre();
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
	bool intersect(Ray ray, Triangle* inTriangle, glm::vec3& outIntersectionPoint);
	glm::vec3 a;
	glm::vec3 b;
	glm::vec3 c;
private:

	BoundingBox* bbox;
};