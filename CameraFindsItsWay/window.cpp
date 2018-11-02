//#include <glad/glad.h>
//#include <glfw3.h>
//#include "Shader.h"
//#include "QuatCamera.h"
//#include "stb_image.h"
//#include "Model.h"
//
//#include "glm/glm.hpp"
//#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtc/type_ptr.hpp"
//
//#include <iostream>
//#include <iterator>
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow *window);
//void mouse_callback(GLFWwindow* window, double xpos, double ypos);
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//glm::vec3 CatmullRom(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
//float computeSplineLength(glm::vec3 point0, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3);
//glm::quat Squad(glm::quat q0, glm::quat q1, glm::quat q2, glm::quat q4, float t);
//unsigned int loadTexture(char const* path);
//
//const unsigned int SCR_WIDTH = 1280;
//const unsigned int SCR_HEIGHT = 720;
//
//float deltaTime = 0.0f;
//float lastFrame = 0.0f;
//
//QuatCamera camera(glm::vec3(0.0f, 0.0f, 3.0f)); // 1.0f, 0.0f, 3.0f
//float lastX = SCR_WIDTH / 2;
//float lastY = SCR_HEIGHT / 2;
//bool firstMouse = true;
//
//// Camera path stuff
//bool autoCamera = false;
//float totalLength = 0;
//float segmentLength[10];
//
//// plane
//unsigned int planeVAO;
//
//int main()
//{
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "EZG", NULL, NULL);
//	if (window == NULL)
//	{
//		std::cout << "Failed to create GLFW window" << std::endl;
//		glfwTerminate();
//		return -1;
//	}
//
//	glfwMakeContextCurrent(window);
//	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	glfwSetCursorPosCallback(window, mouse_callback);
//	glfwSetScrollCallback(window, scroll_callback);
//
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	{
//		std::cout << "Failed to initialize GLAD" << std::endl;
//		return -1;
//	}
//
//	Shader lampShader("C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/lamp.vertex", "C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/lamp.frag");
//	Shader lightingShader("C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/colors.vertex", "C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/colors.frag");
//	//Shader modelShader("C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/model.vs", "C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/model.fs");
//
//	// plane stuff
//
//	float planeVertices[] = {
//		// positions            // normals         // texcoords
//		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
//		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
//		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
//
//		 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
//		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
//		 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 10.0f
//	};
//
//	unsigned int planeVBO;
//	glGenVertexArrays(1, &planeVAO);
//	glGenBuffers(1, &planeVBO);
//	glBindVertexArray(planeVAO);
//	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//	glBindVertexArray(0);
//
//	unsigned int woodTexture = loadTexture("C:/Users/basti/source/repos/CameraFindsItsWay/CameraFindsItsWay/wood.png");
//
//	float vertices[] = {
//		// positions          // normals           // texture coords
//		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
//		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
//		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
//		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
//		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
//
//		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
//		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
//		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
//		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
//		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
//
//		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
//		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
//		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
//		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
//
//		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
//		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
//		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
//		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
//		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
//
//		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
//		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
//		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
//		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
//		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
//		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
//
//		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
//		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
//		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
//		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
//		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
//		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
//	};
//
//	glm::vec3 pointLightPositions[] = {
//	glm::vec3(0.7f,  0.2f,  2.0f),
//	glm::vec3(2.3f, -3.3f, -4.0f),
//	glm::vec3(-4.0f,  2.0f, -12.0f),
//	glm::vec3(0.0f,  0.0f, -3.0f)
//	};
//
//	glm::vec3 cubePositions[] = {
//		glm::vec3(0.0f,  0.0f,  0.0f),
//		glm::vec3(2.0f,  4.0f, -5.0f),
//		glm::vec3(-1.5f, -2.2f, -2.5f),
//	};
//
//	glm::vec3 pathPoints[] = {
//		glm::vec3(0.0f, 0.0f, 4.0f),
//		glm::vec3(1.0f, 0.0f, 3.0f),
//		glm::vec3(1.0f, 0.0f, -0.5f),
//		glm::vec3(0.55f, -0.3f, -1),
//		glm::vec3(-0.8f, -1.3, -1.2),
//		glm::vec3(-4.0f, -2.0f, -1.65),
//		glm::vec3(-2.8f, -2.2f, -4.5),
//		glm::vec3(-0.95f, -0.7f, -4.1),
//		glm::vec3(0.2f, 1.0f, -4.0f),
//		glm::vec3(1.0f, 2.6f, -3.8f),
//		glm::vec3(2.25f, 4.8f, -4.3f),
//		glm::vec3(2.1f, 4.7f, -5.5f)
//	};
//
//	glm::quat quats[] = {
//		glm::angleAxis(0.0f, glm::vec3(0.0, 1.0, 0.0)),
//		glm::angleAxis(5.0f, glm::vec3(0.0, 1.0, 0.0)),
//		glm::angleAxis(3.0f, glm::vec3(0.0, 1.0, 0.0)),
//		glm::angleAxis(10.0f, glm::vec3(0.0, 1.0, 0.0)),
//		glm::angleAxis(4.0f, glm::vec3(0.0, 1.0, 0.0)),
//		glm::angleAxis(12.0f, glm::vec3(1.0, 0.0, 0.0)),
//		glm::angleAxis(18.0f, glm::vec3(0.0, 1.0, 0.0)),
//		glm::angleAxis(14.0f, glm::vec3(0.0, 0.0, 1.0)),
//		glm::angleAxis(10.0f, glm::vec3(0.0, 1.0, 0.0)),
//		glm::angleAxis(2.0f, glm::vec3(0.0, 1.0, 0.0)),
//		glm::angleAxis(6.0f, glm::vec3(0.0, 1.0, 0.0)),
//		glm::angleAxis(0.0f, glm::vec3(0.0, 1.0, 0.0))
//	};
//
//	for (int i = 1; i < 10; i++)
//	{
//		float l = computeSplineLength(pathPoints[i - 1], pathPoints[i], pathPoints[i + 1], pathPoints[i + 2]);
//		segmentLength[i - 1] = l;
//		totalLength += l;
//	}
//
//	unsigned int VBO, cubeVAO;
//	glGenVertexArrays(1, &cubeVAO);
//	glGenBuffers(1, &VBO);
//
//	glBindVertexArray(cubeVAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//
//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
//	glEnableVertexAttribArray(2);
//
//	unsigned int lightVAO;
//	glGenVertexArrays(1, &lightVAO);
//	glBindVertexArray(lightVAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	unsigned int diffuseMap = loadTexture("container2.png");
//	unsigned int specularMap = loadTexture("container2_specular.png");
//
//	lightingShader.use();
//	lightingShader.setInt("material.diffuse", 0);
//	lightingShader.setInt("material.specular", 1);
//
//	glEnable(GL_DEPTH_TEST);
//
//	float t = 1.0f;
//	float dist = 0.0f;
//	int currentSegment = 0;
//
//	while (!glfwWindowShouldClose(window))
//	{
//		float currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//		processInput(window);
//
//		if (autoCamera)
//		{
//			t = dist / segmentLength[currentSegment] + 1;
//			float tOrientation = 0.5f * deltaTime;
//			if (t >= 2 && currentSegment < 10)
//			{
//				dist = 0;
//				currentSegment++;
//				std::cout << "Current segment: " << currentSegment << std::endl;
//			}
//
//			glm::quat qSquad = Squad(
//				quats[currentSegment], 
//				quats[currentSegment + 1], 
//				quats[currentSegment + 2], 
//				quats[currentSegment + 3], 
//				t - 1);
//			glm::vec3 target = CatmullRom(t, pathPoints[currentSegment], pathPoints[currentSegment+1], pathPoints[currentSegment+2], pathPoints[currentSegment+3]);
//			camera.Move(target - camera.Position, deltaTime);
//			glm::mat4 mSquad = glm::mat4_cast(qSquad);
//			//camera.Rotate(qSquad);
//			dist += camera.MovementSpeed * deltaTime;
//		}
//
//		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//		lightingShader.use();
//		lightingShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
//		lightingShader.setFloat("material.shininess", 32.0f);
//
//		// directional light
//		lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
//		lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
//		lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
//		lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
//
//		// point light 1
//		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
//		lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
//		lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
//		lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
//		lightingShader.setFloat("pointLights[0].constant", 1.0f);
//		lightingShader.setFloat("pointLights[0].linear", 0.09);
//		lightingShader.setFloat("pointLights[0].quadratic", 0.032);
//		// point light 2
//		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
//		lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
//		lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
//		lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
//		lightingShader.setFloat("pointLights[1].constant", 1.0f);
//		lightingShader.setFloat("pointLights[1].linear", 0.09);
//		lightingShader.setFloat("pointLights[1].quadratic", 0.032);
//		// point light 3
//		lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
//		lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
//		lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
//		lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
//		lightingShader.setFloat("pointLights[2].constant", 1.0f);
//		lightingShader.setFloat("pointLights[2].linear", 0.09);
//		lightingShader.setFloat("pointLights[2].quadratic", 0.032);
//		// point light 4
//		lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
//		lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
//		lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
//		lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
//		lightingShader.setFloat("pointLights[3].constant", 1.0f);
//		lightingShader.setFloat("pointLights[3].linear", 0.09);
//		lightingShader.setFloat("pointLights[3].quadratic", 0.032);
//		
//		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//		glm::mat4 view = camera.GetViewMatrix();
//		lightingShader.setMat4("projection", projection);
//		lightingShader.setMat4("view", view);
//
//		glm::mat4 model;
//		lightingShader.setMat4("model", model);
//
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, diffuseMap);
//
//		glActiveTexture(GL_TEXTURE1);
//		glBindTexture(GL_TEXTURE_2D, specularMap);
//
//		glBindVertexArray(cubeVAO);
//		//glDrawArrays(GL_TRIANGLES, 0, 36);
//		for (unsigned int i = 0; i < 3; i++)
//		{
//			// calculate the model matrix for each object and pass it to shader before drawing
//			glm::mat4 model;
//			model = glm::translate(model, cubePositions[i]);
//			float angle = 20.0f * i;
//			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//			lightingShader.setMat4("model", model);
//
//			glDrawArrays(GL_TRIANGLES, 0, 36);
//		}
//
//		lampShader.use();
//		lampShader.setMat4("projection", projection);
//		lampShader.setMat4("view", view);
//
//		glBindVertexArray(lightVAO);
//		for (unsigned int i = 0; i < 4; i++)
//		{
//			model = glm::mat4();
//			model = glm::translate(model, pointLightPositions[i]);
//			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
//			lampShader.setMat4("model", model);
//			glDrawArrays(GL_TRIANGLES, 0, 36);
//		}
//
//		//unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
//		//unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
//		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
//		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
//		//ourShader.setMat4("projection", projection);
//		//glBindVertexArray(VAO);
//		//for (unsigned int i = 0; i < 3; i++)
//		//{
//		//	glm::mat4 model;
//		//	model = glm::translate(model, cubePositions[i]);
//		//	ourShader.setMat4("model", model);
//		//	glDrawArrays(GL_TRIANGLES, 0, 36);
//		//}
//
//
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//	}
//
//	glDeleteVertexArrays(1, &cubeVAO);
//	glDeleteVertexArrays(1, &lightVAO);
//	glDeleteBuffers(1, &VBO);
//
//	glfwTerminate();
//	return 0;
//}
//
//void processInput(GLFWwindow *window)
//{
//	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, true);
//
//	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
//	{
//		autoCamera = true;
//	}
//
//	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
//	{
//		std::cout << "Camera Pos: " << camera.Position.x << " " << camera.Position.y << " " << camera.Position.z << std::endl;
//	}
//
//	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
//	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
//}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//	glViewport(0, 0, width, height);
//}
//
//glm::vec3 CatmullRom(float t, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
//{
//	float t0 = 0.0f, t1 = 1.0f, t2 = 2.0f, t3 = 3.0f;
//
//	glm::vec3 a1 = (t1 - t) / (t1 - t0) * p0 + (t - t0) / (t1 - t0) * p1;
//	glm::vec3 a2 = (t2 - t) / (t2 - t1) * p1 + (t - t1) / (t2 - t1) * p2;
//	glm::vec3 a3 = (t3 - t) / (t3 - t2) * p2 + (t - t2) / (t3 - t2) * p3;
//
//	glm::vec3 b1 = (t2 - t) / (t2 - t0) * a1 + (t - t0) / (t2 - t0) * a2;
//	glm::vec3 b2 = (t3 - t) / (t3 - t1) * a2 + (t - t1) / (t3 - t1) * a3;
//
//	glm::vec3 c = (t2 - t) / (t2 - t1) * b1 + (t - t1) / (t2 - t1) * b2;
//
//	return c;
//}
//
//glm::quat operator*(glm::quat &q, const float &other)
//{
//	q.x *= other;
//	q.y *= other;
//	q.z *= other;
//	q.w *= other;
//
//	return q;
//}
//
//glm::quat Squad(glm::quat q0, glm::quat q1, glm::quat q2, glm::quat q3, float t)
//{
//	q0 = glm::normalize(q0);
//	q1 = glm::normalize(q1);
//	q2 = glm::normalize(q2);
//	q3 = glm::normalize(q3);
//
//	glm::quat a1 = q1 * glm::exp((glm::log(glm::inverse(q1) * q0) + glm::log(glm::inverse(q1) * q2)) * -0.25f);
//	glm::quat a2 = q2 * glm::exp((glm::log(glm::inverse(q2) * q1) + glm::log(glm::inverse(q2) * q3)) * -0.25f);
//
//	glm::quat result;
//	result = glm::mix(glm::mix(q1, q2, t), glm::mix(a1, a2, t), 2 * t * (t - 1));
//
//	glm::vec3 angles = glm::eulerAngles(result);
//
//	return result;
//}
//
//float computeSplineLength(glm::vec3 point0, glm::vec3 point1, glm::vec3 point2, glm::vec3 point3)
//{
//	glm::vec3 points[21];
//	int i = 0;
//	for (float t = 1.0f; i < 21; t += 0.05f)
//	{
//		points[i] = CatmullRom(t, point0, point1, point2, point3);
//		i++;
//	}
//
//	float totalLength = 0.0f;
//
//	for (int i = 0; i < 20; i++)
//	{
//		totalLength += glm::length(points[i + 1] - points[i]);
//	}
//
//	return totalLength;
//}
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	if (!autoCamera)
//	{
//		if (firstMouse)
//		{
//			lastX = xpos;
//			lastY = ypos;
//			firstMouse = false;
//		}
//
//		float xoffset = xpos - lastX;
//		float yoffset = lastY - ypos;
//		lastX = xpos;
//		lastY = ypos;
//
//		camera.ProcessMouseMovement(xoffset, yoffset);
//	}
//}
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	camera.ProcessMouseScroll(yoffset);
//}
//
//unsigned int loadTexture(char const* path)
//{
//	unsigned int textureID;
//	glGenTextures(1, &textureID);
//
//	int width, height, nrComponents;
//	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
//	if (data)
//	{
//		GLenum format;
//		if (nrComponents == 1)
//			format = GL_RED;
//		else if (nrComponents == 3)
//			format = GL_RGB;
//		else if (nrComponents == 4)
//			format = GL_RGBA;
//
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		stbi_image_free(data);
//	}
//	else
//	{
//		std::cout << "Texture failed to load at path: " << path << std::endl;
//	}
//
//	return textureID;
//}
