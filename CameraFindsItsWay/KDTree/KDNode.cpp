#include "KDNode.h"
#include "BoundingBox.h"
#include <algorithm>
#include <iostream>

KDNode* KDNode::build(std::vector<Triangle*>& tris, int depth) const
{
	KDNode* node = new KDNode();
	node->triangles = tris;
	node->left = nullptr;
	node->right = nullptr;
	node->bbox = BoundingBox();

	//std::cout << "size " << tris.size() << std::endl;

	if (tris.size() == 0)
	{
		return node;
	}

	if (tris.size() == 1)
	{
		node->bbox = tris[0]->getBoundingBox();
		node->left = new KDNode();
		node->right = new KDNode();
		node->left->triangles = std::vector<Triangle*>();
		node->right->triangles = std::vector<Triangle*>();

		return node;
	}

	// mega bounding box around all tris
	node->bbox = tris[0]->getBoundingBox();

	for (int i = 1; i < tris.size(); i++)
	{
		node->bbox.expand(tris[i]->getBoundingBox());
	}

	//std::cout << "pivot" << node->bbox.getPivot().x << " " << node->bbox.getPivot().y << " " << node->bbox.getPivot().z << std::endl;
	
	int axis = node->bbox.longestAxis();

	//std::cout << "longest axis: " << axis << std::endl;
	//std::cout << "depth: " << depth << std::endl << std::endl;

	// search for median
	std::vector<float> vals = std::vector<float>();
	for (int i = 0; i < tris.size(); i++)
	{
		if (axis == 0)
		{
			vals.push_back(tris[i]->getCentre().x);
			//std::cout << "Centre: " << tris[i]->getCentre().x << std::endl;
		}
		else if (axis == 1)
		{
			vals.push_back(tris[i]->getCentre().y);
		}
		else if (axis == 2)
		{
			vals.push_back(tris[i]->getCentre().z);
		}
		else
		{
			// something went terribly wrong
		}
	}

	// calculate median
	float median;
	glm::vec3 midpt = glm::vec3(0, 0, 0);
	for (int i = 0; i < tris.size(); i++)
	{
		midpt = midpt + (tris[i]->getCentre() * (1.0f / tris.size()));
	}

	if (std::adjacent_find(vals.begin(), vals.end(), std::not_equal_to<float>()) == vals.end())
	{
		//std::cout << "All elements are equal each other" << std::endl;
		node->left = new KDNode();
		node->right = new KDNode();
		node->left->triangles = std::vector<Triangle*>();
		node->right->triangles = std::vector<Triangle*>();
		return node;
	}

	std::cout << std::endl;

	if (vals.size() % 2 == 0)
	{
		std::nth_element(vals.begin(), vals.begin() + vals.size() / 2, vals.end());
		median = vals[vals.size() / 2];
		std::nth_element(vals.begin(), vals.begin() + vals.size() / 2 - 1, vals.end());
		median = (median + vals[vals.size() / 2 - 1]) / 2;
	}
	else
	{
		std::nth_element(vals.begin(), vals.begin() + vals.size() / 2, vals.end());
		median = vals[vals.size() / 2];
	}

	// split tris


	std::vector<Triangle*> leftTris;
	std::vector<Triangle*> rightTris;

	//std::cout << "Median: " << median << std::endl;
	for (int i = 0; i < tris.size(); i++)
	{
		switch (axis)
		{
		case 0:
			midpt.x >= tris[i]->getCentre().x ? rightTris.push_back(tris[i]) : leftTris.push_back(tris[i]);
			break;
		case 1:
			midpt.y >= tris[i]->getCentre().y ? rightTris.push_back(tris[i]) : leftTris.push_back(tris[i]);
			break;
		case 2:
			midpt.z >= tris[i]->getCentre().z ? rightTris.push_back(tris[i]) : leftTris.push_back(tris[i]);
			break;
		}
	}

	//std::cout << tris.size() << std::endl;

	int matches = 0;
	for (int i = 0; i < leftTris.size(); i++)
	{
		for (int k = 0; k < rightTris.size(); k++)
		{
			if (leftTris[i] == rightTris[k])
			{
				matches++;
			}
		}
	}

	node->left = build(leftTris, depth + 1);
	node->right = build(rightTris, depth + 1);

	//if ((float)matches / leftTris.size() < 0.5 && (float)matches / rightTris.size() < 0.5)
	//{
	//	node->left = build(leftTris, depth + 1);
	//	node->right = build(rightTris, depth + 1);
	//}
	//else
	//{
	//	node->left = new KDNode();
	//	node->right = new KDNode();
	//	node->left->triangles = std::vector<Triangle*>();
	//	node->right->triangles = std::vector<Triangle*>();
	//}

	return node;
}

void KDNode::getAllBoundingBoxes(std::vector<BoundingBox>& bboxes)
{
	bboxes.push_back(this->bbox);

	if (left->triangles.size() > 0)
	{
		left->getAllBoundingBoxes(bboxes);
	}
	if (right->triangles.size() > 0)
	{
		right->getAllBoundingBoxes(bboxes);
	}
}
