#include "Ray.h"
#include "../glm/glm.hpp"



Ray::Ray()
{
}


Ray::~Ray()
{
}

Ray::Ray(glm::vec3 newOrigin, glm::vec3 newDirection)
{
	origin = newOrigin;
	direction = newDirection;
}

