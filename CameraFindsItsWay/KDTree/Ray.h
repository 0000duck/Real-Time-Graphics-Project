#pragma once
#include "../glm/glm.hpp"

class Ray
{
public:
	Ray();
	~Ray();

	Ray(glm::vec3 newOrigin, glm::vec3 newDirection);

	// Origin 
	glm::vec3 origin;
	// Direction
	glm::vec3 direction;
};

