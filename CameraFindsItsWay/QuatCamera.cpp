#include "QuatCamera.h"
#include <iostream>

QuatCamera::QuatCamera(glm::vec3 position, glm::vec3 up)
	:
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVITY),
	Zoom(ZOOM)
{
	Position = position;
	Orientation = glm::quat(0, 0, 0, -1);
	RightAngle = 0.f;
	UpAngle = 0.f;
	updateCameraVectors();
}

QuatCamera::QuatCamera(float posX, float posY, float posZ, float upX, float upY, float upZ)
	: MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
{
	Position = glm::vec3(posX, posY, posZ);
	Orientation = glm::quat(0, 0, 0, -1);
	RightAngle = 0.f;
	UpAngle = 0.f;
	updateCameraVectors();
}

glm::mat4 QuatCamera::GetViewMatrix()
{
	glm::quat reverseOrient = glm::conjugate(Orientation);
	glm::mat4 rot = glm::mat4_cast(reverseOrient);
	glm::mat4 translation = glm::translate(glm::mat4(1.0), -Position);

	return rot * translation;
}

void QuatCamera::Move(glm::vec3 direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	glm::normalize(direction);
	Position += direction * velocity;
}

void QuatCamera::Rotate(glm::mat4 rot)
{

}

void QuatCamera::Rotate(glm::quat q)
{
	Orientation = q;
}

glm::vec3 QuatCamera::GetFront()
{
	glm::quat qF = Orientation * glm::quat(0, 0, 0, -1) * glm::conjugate(Orientation);
	glm::vec3 Front = { qF.x, qF.y, qF.z };
	return Front;
}

void QuatCamera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;

	glm::quat qF = Orientation * glm::quat(0, 0, 0, -1) * glm::conjugate(Orientation);
	glm::vec3 Front = { qF.x, qF.y, qF.z };
	glm::vec3 Right = glm::normalize(glm::cross(Front, glm::vec3(0, 1, 0)));

	if (direction == Camera_Movement::FORWARD)
		Position += Front * velocity;

	if (direction == Camera_Movement::BACKWARD)
		Position -= Front * velocity;

	if (direction == Camera_Movement::LEFT)
		Position -= Right * velocity;

	if (direction == Camera_Movement::RIGHT)
		Position += Right * velocity;
}

void QuatCamera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	RightAngle += xoffset;
	UpAngle += yoffset;

	updateCameraVectors();
}

void QuatCamera::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.f && Zoom <= 45.f)
		Zoom -= yoffset;

	if (Zoom <= 1.f)
		Zoom = 1.f;

	if (Zoom >= 45.f)
		Zoom = 45.f;
}

void QuatCamera::updateCameraVectors()
{
	glm::quat aroundY = glm::angleAxis(glm::radians(-RightAngle), glm::vec3(0, 1, 0));

	glm::quat aroundX = glm::angleAxis(glm::radians(UpAngle), glm::vec3(1, 0, 0));

	Orientation = aroundY * aroundX;
}