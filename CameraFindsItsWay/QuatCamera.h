#pragma once

#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

enum class Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float SPEED = 2.0f;
const float SENSITIVITY = 0.01f;
const float ZOOM = 45.0f;

class QuatCamera
{
public:
	glm::vec3 Position;
	glm::quat Orientation;
	float RightAngle;
	float UpAngle;

	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	QuatCamera
	(
		glm::vec3 position = glm::vec3(0.f, 0.f, 0.f),
		glm::vec3 up = glm::vec3(0.f, 1.0f, 0.f)
	);
	QuatCamera(float posX, float posY, float posZ, float upX, float upY, float upZ);

	glm::mat4 GetViewMatrix();
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	void ProcessMouseScroll(float yoffset);
	void Move(glm::vec3 direction, float deltaTime);
	void Rotate(glm::mat4 rot);
	void Rotate(glm::quat q);

private:
	void updateCameraVectors();
};