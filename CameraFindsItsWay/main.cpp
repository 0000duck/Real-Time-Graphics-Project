#include <glad/glad.h>
#include <glfw3.h>
#include "Shader.h"
#include "QuatCamera.h"
#include "stb_image.h"
#include "Model.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <iterator>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
glm::vec3 CatmullRom(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
float computeSplineLength(glm::vec3 point0, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
glm::quat Squad(glm::quat q0, glm::quat q1, glm::quat q2, glm::quat q4, float t);
unsigned int loadTexture(char const* path);
void renderScene(const Shader &shader);
void renderCube();
void renderQuad();
void calculateTangents(float vertexArray[], int stride, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4, glm::vec2  uv1, glm::vec2 uv2, glm::vec2 uv3, glm::vec2 uv4, glm::vec3 nm);
void renderQuad(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3, glm::vec2 uv4, glm::vec3 nm);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

QuatCamera camera(glm::vec3(1.0f, 0.0f, 3.0f)); // 1.0f, 0.0f, 3.0f
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;
bool firstMouse = true;

// Camera path stuff
bool autoCamera = false;
float totalLength = 0;
float segmentLength[10];

glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
glm::mat4 lightRotationMat;
float lightRotation;

// normal map
glm::vec3 bumpFactor = glm::vec3(1.0f);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "EZG", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	//Shader lampShader("C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/lamp.vertex", "C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/lamp.frag");
	//Shader lightingShader("C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/colors.vertex", "C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/colors.frag");
	//Shader modelShader("C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/model.vs", "C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/model.fs");

	Shader shadowMapShader("shadowMapping.vs", "shadowMapping.fs");
	Shader debugShader("debugQuad.vs", "debugQuad.fs");
	Shader shader("Shadow_Mapping_Shader.vs", "Shadow_Mapping_Shader.fs");
	Shader normalShader("normalShader.vs", "normalShader.fs");

	Shader modelShader("model.vs", "model.fs");

	//unsigned int cubeTexture = loadTexture("C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/brickwall.jpg");
	//unsigned int normalMap = loadTexture("C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/brickwall_normal.jpg");

	unsigned int diffuseMap = loadTexture("C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/Wood_Floor_007_COLOR.jpg");
	unsigned int normalMap = loadTexture("C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/normal_4.png");

	glm::vec3 pathPoints[] = {
		glm::vec3(0.0f, 0.0f, 4.0f),
		glm::vec3(1.0f, 0.0f, 3.0f),
		glm::vec3(1.0f, 0.0f, -0.5f),
		glm::vec3(0.55f, -0.3f, -1),
		glm::vec3(-0.8f, -1.3, -1.2),
		glm::vec3(-4.0f, -2.0f, -1.65),
		glm::vec3(-2.8f, -2.2f, -4.5),
		glm::vec3(-0.95f, -0.7f, -4.1),
		glm::vec3(0.2f, 1.0f, -4.0f),
		glm::vec3(1.0f, 2.6f, -3.8f),
		glm::vec3(2.25f, 4.8f, -4.3f),
		glm::vec3(2.1f, 4.7f, -5.5f)
	};

	glm::quat quats[] = {
		glm::angleAxis(0.0f, glm::vec3(0.0, 1.0, 0.0)),
		glm::angleAxis(5.0f, glm::vec3(0.0, 1.0, 0.0)),
		glm::angleAxis(3.0f, glm::vec3(0.0, 1.0, 0.0)),
		glm::angleAxis(10.0f, glm::vec3(0.0, 1.0, 0.0)),
		glm::angleAxis(4.0f, glm::vec3(0.0, 1.0, 0.0)),
		glm::angleAxis(12.0f, glm::vec3(1.0, 0.0, 0.0)),
		glm::angleAxis(18.0f, glm::vec3(0.0, 1.0, 0.0)),
		glm::angleAxis(14.0f, glm::vec3(0.0, 0.0, 1.0)),
		glm::angleAxis(10.0f, glm::vec3(0.0, 1.0, 0.0)),
		glm::angleAxis(2.0f, glm::vec3(0.0, 1.0, 0.0)),
		glm::angleAxis(6.0f, glm::vec3(0.0, 1.0, 0.0)),
		glm::angleAxis(0.0f, glm::vec3(0.0, 1.0, 0.0))
	};

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	normalShader.use();
	normalShader.setInt("diffuseMap", 0);
	normalShader.setInt("normalMap", 1);
	normalShader.setInt("shadowMap", 2);
	debugShader.use();
	debugShader.setInt("depthMap0", 0);
	
	for (int i = 1; i < 10; i++)
	{
		float l = computeSplineLength(pathPoints[i - 1], pathPoints[i], pathPoints[i + 1], pathPoints[i + 2]);
		segmentLength[i - 1] = l;
		totalLength += l;
	}

	float t = 1.0f;
	float dist = 0.0f;
	int currentSegment = 0;

	//Model carModel("testarossa/testarossa3.obj");

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (autoCamera)
		{
			t = dist / segmentLength[currentSegment] + 1;
			float tOrientation = 0.5f * deltaTime;
			if (t >= 2 && currentSegment < 10)
			{
				dist = 0;
				currentSegment++;
				std::cout << "Current segment: " << currentSegment << std::endl;
			}

			glm::quat qSquad = Squad(
				quats[currentSegment],
				quats[currentSegment + 1],
				quats[currentSegment + 2],
				quats[currentSegment + 3],
				t - 1);
			glm::vec3 target = CatmullRom(t, pathPoints[currentSegment], pathPoints[currentSegment + 1], pathPoints[currentSegment + 2], pathPoints[currentSegment + 3]);
			camera.Move(target - camera.Position, deltaTime);
			glm::mat4 mSquad = glm::mat4_cast(qSquad);
			//camera.Rotate(qSquad);
			dist += camera.MovementSpeed * deltaTime;
		}

		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::vec3 lightPosRotated = glm::vec4(lightPos, 0) * lightRotationMat;
		lightView = glm::lookAt(lightPosRotated, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;

		shadowMapShader.use();
		shadowMapShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		renderScene(shadowMapShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		normalShader.use();

		normalShader.setVec3("bumpFactor", bumpFactor);

		//normalShader.setInt("diffuseMap", 0);
		//normalShader.setInt("normalMap", 1);
		//normalShader.setInt("shadowMap", 2);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		normalShader.setMat4("projection", projection);
		normalShader.setMat4("view", view);
		
		normalShader.setVec3("viewPos", camera.Position);
		normalShader.setVec3("lightPos", lightPosRotated);
		normalShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, normalMap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		renderScene(normalShader);


		//modelShader.use();
		//modelShader.setMat4("projection", projection);
		//modelShader.setMat4("view", view);

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		//modelShader.setMat4("model", model);
		//carModel.Draw(modelShader);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//glDeleteVertexArrays(1, &planeVAO);
	//glDeleteBuffers(1, &planeVBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	{
		autoCamera = true;
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
	{
		std::cout << "Camera Pos: " << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << std::endl;
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		glm::mat4 rotationMat;
		lightRotation = lightRotation + deltaTime * 25.0f;
		rotationMat = glm::rotate(rotationMat, glm::radians(lightRotation), glm::vec3(0.0f, 1.0f, 0.0f));
		lightRotationMat = rotationMat;
		//lightPos = glm::vec4(lightPos.x, lightPos.y, lightPos.z, 1.0) * rotationMat;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

glm::vec3 CatmullRom(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	float t0 = 0.0f, t1 = 1.0f, t2 = 2.0f, t3 = 3.0f;

	glm::vec3 a1 = (t1 - t) / (t1 - t0) * p0 + (t - t0) / (t1 - t0) * p1;
	glm::vec3 a2 = (t2 - t) / (t2 - t1) * p1 + (t - t1) / (t2 - t1) * p2;
	glm::vec3 a3 = (t3 - t) / (t3 - t2) * p2 + (t - t2) / (t3 - t2) * p3;

	glm::vec3 b1 = (t2 - t) / (t2 - t0) * a1 + (t - t0) / (t2 - t0) * a2;
	glm::vec3 b2 = (t3 - t) / (t3 - t1) * a2 + (t - t1) / (t3 - t1) * a3;

	glm::vec3 c = (t2 - t) / (t2 - t1) * b1 + (t - t1) / (t2 - t1) * b2;

	return c;
}

glm::quat operator*(glm::quat &q, const float &other)
{
	q.x *= other;
	q.y *= other;
	q.z *= other;
	q.w *= other;

	return q;
}

glm::quat Squad(glm::quat q0, glm::quat q1, glm::quat q2, glm::quat q3, float t)
{
	q0 = glm::normalize(q0);
	q1 = glm::normalize(q1);
	q2 = glm::normalize(q2);
	q3 = glm::normalize(q3);

	glm::quat a1 = q1 * glm::exp((glm::log(glm::inverse(q1) * q0) + glm::log(glm::inverse(q1) * q2)) * -0.25f);
	glm::quat a2 = q2 * glm::exp((glm::log(glm::inverse(q2) * q1) + glm::log(glm::inverse(q2) * q3)) * -0.25f);

	glm::quat result;
	result = glm::mix(glm::mix(q1, q2, t), glm::mix(a1, a2, t), 2 * t * (t - 1));

	glm::vec3 angles = glm::eulerAngles(result);

	return result;
}

float computeSplineLength(glm::vec3 point0, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3)
{
	glm::vec3 points[21];
	int i = 0;
	for (float t = 1.0f; i < 21; t += 0.05f)
	{
		points[i] = CatmullRom(t, point0, point1, point2, point3);
		i++;
	}

	float totalLength = 0.0f;

	for (int i = 0; i < 20; i++)
	{
		totalLength += glm::length(points[i + 1] - points[i]);
	}

	return totalLength;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (!autoCamera)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;
		lastX = xpos;
		lastY = ypos;

		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
	}

	return textureID;
}


//// positions            // normals         // texcoords
//25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
//-25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
//-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
//
//25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
//-25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
//25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 10.0f

void renderScene(const Shader &shader)
{
	// floor
	glm::mat4 model;
	shader.setMat4("model", model);
	//glBindVertexArray(planeVAO);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	renderQuad(glm::vec3(-25.0f, -0.5, -25.0f), glm::vec3(-25.0f, -0.5, 25.0f), glm::vec3(25.0f, -0.5f, 25.0f), glm::vec3(25.0f, -0.5, -25.0f), glm::vec2(0.0f, 25.0f), glm::vec2(0.0f, 0.0f), glm::vec2(25.0f, 0.0f), glm::vec2(25.0f, 25.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// cubes
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(2.0f, 2.0f, -5.0));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);
	renderCube();
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-1.5f, 2.2f, -2.5f));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.5));
	shader.setMat4("model", model);
	renderCube();

	glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  4.0f, -5.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	};

	//for (unsigned int i = 0; i < 3; i++)
	//{
	//	model = glm::translate(model, cubePositions[i]);
	//	float angle = 20.0f * i;
	//	model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
	//	shader.setMat4("model", model);
	//	renderCube();
	//}
}

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
		float vertices[504];

		calculateTangents(vertices, 0, glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f));
		calculateTangents(vertices, 84, glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		calculateTangents(vertices, 168, glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
		calculateTangents(vertices, 252, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		calculateTangents(vertices, 336, glm::vec3(1.0f, -1.0f, -1.0f), glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		calculateTangents(vertices, 420, glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void calculateTangents(float vertexArray[], int stride, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4, glm::vec2  uv1, glm::vec2 uv2, glm::vec2 uv3, glm::vec2 uv4, glm::vec3 nm)
{
	// calculate tangent/bitangent vectors of both triangles
	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;
	// triangle 1
	// ----------
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent1 = glm::normalize(tangent1);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent1 = glm::normalize(bitangent1);

	// triangle 2
	// ----------
	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent2 = glm::normalize(tangent2);


	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent2 = glm::normalize(bitangent2);

	vertexArray[stride] = pos1.x;
	vertexArray[stride + 1] = pos1.y;
	vertexArray[stride + 2] = pos1.z;
	vertexArray[stride + 3] = nm.x;
	vertexArray[stride + 4] = nm.y;
	vertexArray[stride + 5] = nm.z;
	vertexArray[stride + 6] = uv1.x;
	vertexArray[stride + 7] = uv1.y;
	vertexArray[stride + 8] = tangent1.x;
	vertexArray[stride + 9] = tangent1.y;
	vertexArray[stride + 10] = tangent1.z;
	vertexArray[stride + 11] = bitangent1.x;
	vertexArray[stride + 12] = bitangent1.y;
	vertexArray[stride + 13] = bitangent1.z;

	vertexArray[stride + 14] = pos2.x;
	vertexArray[stride + 15] = pos2.y;
	vertexArray[stride + 16] = pos2.z;
	vertexArray[stride + 17] = nm.x;
	vertexArray[stride + 18] = nm.y;
	vertexArray[stride + 19] = nm.z;
	vertexArray[stride + 20] = uv2.x;
	vertexArray[stride + 21] = uv2.y;
	vertexArray[stride + 22] = tangent1.x;
	vertexArray[stride + 23] = tangent1.y;
	vertexArray[stride + 24] = tangent1.z;
	vertexArray[stride + 25] = bitangent1.x;
	vertexArray[stride + 26] = bitangent1.y;
	vertexArray[stride + 27] = bitangent1.z;

	vertexArray[stride + 28] = pos3.x;
	vertexArray[stride + 29] = pos3.y;
	vertexArray[stride + 30] = pos3.z;
	vertexArray[stride + 31] = nm.x;
	vertexArray[stride + 32] = nm.y;
	vertexArray[stride + 33] = nm.z;
	vertexArray[stride + 34] = uv3.x;
	vertexArray[stride + 35] = uv3.y;
	vertexArray[stride + 36] = tangent1.x;
	vertexArray[stride + 37] = tangent1.y;
	vertexArray[stride + 38] = tangent1.z;
	vertexArray[stride + 39] = bitangent1.x;
	vertexArray[stride + 40] = bitangent1.y;
	vertexArray[stride + 41] = bitangent1.z;

	vertexArray[stride + 42] = pos1.x;
	vertexArray[stride + 43] = pos1.y;
	vertexArray[stride + 44] = pos1.z;
	vertexArray[stride + 45] = nm.x;
	vertexArray[stride + 46] = nm.y;
	vertexArray[stride + 47] = nm.z;
	vertexArray[stride + 48] = uv1.x;
	vertexArray[stride + 49] = uv1.y;
	vertexArray[stride + 50] = tangent2.x;
	vertexArray[stride + 51] = tangent2.y;
	vertexArray[stride + 52] = tangent2.z;
	vertexArray[stride + 53] = bitangent2.x;
	vertexArray[stride + 54] = bitangent2.y;
	vertexArray[stride + 55] = bitangent2.z;

	vertexArray[stride + 56] = pos3.x;
	vertexArray[stride + 57] = pos3.y;
	vertexArray[stride + 58] = pos3.z;
	vertexArray[stride + 59] = nm.x;
	vertexArray[stride + 60] = nm.y;
	vertexArray[stride + 61] = nm.z;
	vertexArray[stride + 62] = uv3.x;
	vertexArray[stride + 63] = uv3.y;
	vertexArray[stride + 64] = tangent2.x;
	vertexArray[stride + 65] = tangent2.y;
	vertexArray[stride + 66] = tangent2.z;
	vertexArray[stride + 67] = bitangent2.x;
	vertexArray[stride + 68] = bitangent2.y;
	vertexArray[stride + 69] = bitangent2.z;

	vertexArray[stride + 70] = pos4.x;
	vertexArray[stride + 71] = pos4.y;
	vertexArray[stride + 72] = pos4.z;
	vertexArray[stride + 73] = nm.x;
	vertexArray[stride + 74] = nm.y;
	vertexArray[stride + 75] = nm.z;
	vertexArray[stride + 76] = uv4.x;
	vertexArray[stride + 77] = uv4.y;
	vertexArray[stride + 78] = tangent2.x;
	vertexArray[stride + 79] = tangent2.y;
	vertexArray[stride + 80] = tangent2.z;
	vertexArray[stride + 81] = bitangent2.x;
	vertexArray[stride + 82] = bitangent2.y;
	vertexArray[stride + 83] = bitangent2.z;
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
//void renderQuad()
//{
//	if (quadVAO == 0)
//	{
//		float quadVertices[] = {
//			// positions        // texture Coords
//			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
//			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
//			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
//			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
//		};
//		// setup plane VAO
//		glGenVertexArrays(1, &quadVAO);
//		glGenBuffers(1, &quadVBO);
//		glBindVertexArray(quadVAO);
//		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
//		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
//		glEnableVertexAttribArray(0);
//		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
//		glEnableVertexAttribArray(1);
//		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
//	}
//	glBindVertexArray(quadVAO);
//	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//	glBindVertexArray(0);
//}

unsigned int planeVAO = 0;
unsigned int planeVBO = 0;

void renderQuad(glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec3 pos4, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3, glm::vec2 uv4, glm::vec3 nm)
{
	if (planeVAO == 0)
	{
		float vertices[84];
		calculateTangents(vertices, 0, pos1, pos2, pos3, pos4, uv1, uv2, uv3, uv4, nm);
		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &planeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(planeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	}
	// render plane
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void renderQuad()
{
	if (quadVAO == 0)
	{
		glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
		glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
		glm::vec3 pos3(1.0f, -1.0f, 0.0f);
		glm::vec3 pos4(1.0f, 1.0f, 0.0f);

		// texture coords
		glm::vec2 uv1(0.0f, 1.0f);
		glm::vec2 uv2(0.0f, 0.0f);
		glm::vec2 uv3(1.0f, 0.0f);
		glm::vec2 uv4(1.0f, 1.0f);

		// normal vector
		glm::vec3 nm(0.0f, 0.0f, 1.0f);

		// calculate tangent/bitangent vectors of both triangles
		glm::vec3 tangent1, bitangent1;
		glm::vec3 tangent2, bitangent2;
		

		// triangle 1

		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent1 = glm::normalize(tangent1);

		bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent1 = glm::normalize(bitangent1);

		// triangle 2

		edge1 = pos3 - pos1;
		edge2 = pos4 - pos1;
		deltaUV1 = uv3 - uv1;
		deltaUV2 = uv4 - uv1;

		f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		tangent2 = glm::normalize(tangent2);


		bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		bitangent2 = glm::normalize(bitangent2);

		float quadVertices[] = {
			// positions            // normal         // texcoords  // tangent                          // bitangent
			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos2.x, pos2.y, pos2.z, nm.x, nm.y, nm.z, uv2.x, uv2.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

			pos1.x, pos1.y, pos1.z, nm.x, nm.y, nm.z, uv1.x, uv1.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos3.x, pos3.y, pos3.z, nm.x, nm.y, nm.z, uv3.x, uv3.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
			pos4.x, pos4.y, pos4.z, nm.x, nm.y, nm.z, uv4.x, uv4.y, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
	}

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
