#pragma once

#include "Shader.h"

class Mesh
{
public:
	float position;
	Shader* shader;
private:
	float m_vertices[];
};
