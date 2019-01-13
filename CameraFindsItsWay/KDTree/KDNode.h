#pragma once

#include <vector>
#include <string>
#include "BoundingBox.h"
#include "Triangle.h"

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
	bool hit();
};
