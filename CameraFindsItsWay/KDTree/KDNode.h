#pragma once

#include <vector>
#include <string>
#include "BoundingBox.h"
#include "Triangle.h"
#include "Ray.h"

class KDNode
{
public:
	BoundingBox bbox;
	KDNode* left;
	KDNode* right;
	std::vector<Triangle*> triangles;

	KDNode() {};

	KDNode* build(std::vector<Triangle*>& tris, int depth) const;
	std::string traverse();
	void getAllBoundingBoxes(std::vector<BoundingBox>& bboxes);
	bool hit(KDNode* node, const Ray& ray, float&, float& tmin, glm::vec3& intersectionPoint) const;
};
