#include "BoundingBox.h"
#include <iostream>


void BoundingBox::expand(BoundingBox otherBox)
{
	glm::vec3 newPivot;
	float newWidth;
	float newHeight;
	float newDepth;

	// first of all find the new pos of the pivot
	if (pivot.x > otherBox.getPivot().x)
	{
		newPivot.x = otherBox.getPivot().x;
	}
	else
	{
		newPivot.x = pivot.x;
	}

	if (pivot.y > otherBox.getPivot().y)
	{
		newPivot.y = otherBox.getPivot().y;
	}
	else
	{
		newPivot.y = pivot.y;
	}

	if (pivot.z > otherBox.getPivot().z)
	{
		newPivot.z = otherBox.getPivot().z;
	}
	else
	{
		newPivot.z = pivot.z;
	}

	// new expansions
	if (getXExpansion() >= otherBox.getXExpansion())
	{
		newWidth = getXExpansion() - newPivot.x;
	}
	else
	{
		newWidth = otherBox.getXExpansion() - newPivot.x;
	}
	if (getYExpansion() >= otherBox.getYExpansion())
	{
		newHeight = getYExpansion() - newPivot.y;
	}
	else
	{
		newHeight = otherBox.getYExpansion() - newPivot.y;
	}
	if (getZExpansion() >= otherBox.getZExpansion())
	{
		newDepth = getZExpansion() - newPivot.z;
	}
	else
	{
		newDepth = otherBox.getZExpansion() - newPivot.z;
	}


	this->pivot = newPivot;
	this->width = newWidth;
	this->height = newHeight;
	this->depth = newDepth;
}

int BoundingBox::longestAxis()
{
	if (width >= height)
	{
		if (width >= depth)
		{
			return 0;
		}
		else
		{
			return 2;
		}
	}
	else if (height >= depth)
	{
		return 1;
	}
	else
	{
		return 2;
	}
}

float BoundingBox::getXExpansion()
{
	return pivot.x + width;
}

float BoundingBox::getYExpansion()
{
	return pivot.y + height;
}

float BoundingBox::getZExpansion()
{
	return pivot.z + depth;
}

BoundingBox::BoundingBox(glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	float orderedX[3] = { a.x, b.x, c.x };
	float orderedY[3] = { a.y, b.y, c.y };
	float orderedZ[3] = { a.z, b.z, c.z };

	std::sort(orderedX, orderedX + 3);
	std::sort(orderedY, orderedY + 3);
	std::sort(orderedZ, orderedZ + 3);

	pivot.x = orderedX[0];
	pivot.y = orderedY[0];
	pivot.z = orderedZ[0];

	this->width = orderedX[2] - orderedX[0];
	this->height = orderedY[2] - orderedY[0];
	this->depth = orderedZ[2] - orderedZ[0];
}
