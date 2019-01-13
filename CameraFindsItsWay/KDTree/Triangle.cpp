#include "Triangle.h"

BoundingBox Triangle::getBoundingBox()
{
	return *bbox;
}

glm::vec3 Triangle::getCentre()
{
	return glm::vec3((a.x + b.x + c.x) / 3, (a.y + b.y + c.y) / 3, (a.z + b.z + c.z) / 3);
}

Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	bbox = new BoundingBox(a, b, c);
	this->a = a;
	this->b = b;
	this->c = c;
}