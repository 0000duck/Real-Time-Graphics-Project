#pragma once
#include "../glm/glm.hpp"
#include <algorithm>
//#include "glm/gtc/type_ptr.hpp"

class BoundingBox
{
public:
	void expand(BoundingBox otherBox);
	int longestAxis();
	glm::vec3 getPivot() { return pivot; };
	float getXExpansion();
	float getYExpansion();
	float getZExpansion();

	BoundingBox(glm::vec3 a, glm::vec3 b, glm::vec3 c);
	BoundingBox() {};
private:
	glm::vec3 pivot;
	float height; // y
	float width; // x
	float depth; // z
};