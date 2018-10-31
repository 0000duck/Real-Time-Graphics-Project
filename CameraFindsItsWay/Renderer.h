#pragma once

#include "Shader.h"
#include <glad/glad.h>
#include <glfw3.h>



class Renderer
{
public:
	Renderer(float vertices[]);
	float position;
	Shader* shader;
	void Draw();
private:
	float* m_vertices;
	unsigned int VAO;
	void initRenderData();
};
