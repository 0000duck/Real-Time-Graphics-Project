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

bool Triangle::intersect(Ray ray, Triangle * inTriangle, glm::vec3 & outIntersectionPoint)
{
	const float EPSILON = 0.0000001;
	glm::vec3 vertex0 = inTriangle->a;
	glm::vec3 vertex1 = inTriangle->b;
	glm::vec3 vertex2 = inTriangle->c;

	glm::vec3 edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;

	h = glm::cross(ray.direction, edge2);
	a = glm::dot(edge1, h);
	if (a > -EPSILON && a < EPSILON)
	{
		return false; // parallel to triangle
	}

	f = 1.0 / a;
	s = ray.origin - vertex0;
	u = f * glm::dot(s, h);
	if (u < 0.0 || u > 1.0)
	{
		return false;
	}
	q = glm::cross(s, edge1);
	v = f * glm::dot(ray.direction, q);
	if (v < 0.0 || u + v > 1.0)
	{
		return false;
	}

	float t = f * glm::dot(edge2, q);
	if (t > EPSILON)
	{
		outIntersectionPoint = ray.origin + ray.direction * t;
		return true;
	}
}
