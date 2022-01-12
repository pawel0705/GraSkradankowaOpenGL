#include "pch.h"
#include "camera.h"

Camera::Camera() {

}

Camera::Camera(glm::vec3 position) {
	this->transformation.cameraPosition = position;
	this->futureCameraPosition = position;

	this->transformation.cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	this->transformation.cameraToWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	this->transformation.cameraPitch = 0;
	this->transformation.cameraYaw = -90;

	this->speed = 2.5f;
	this->zoom = 45.0f;
	this->sensitivity = .05f;

	this->updateCameraEulerAng();
}

void Camera::updateCameraEulerAng() {
	// rotacje
	this->transformation.cameraFront = glm::normalize(glm::vec3(
		cos(glm::radians(this->transformation.cameraYaw)) * cos(glm::radians(this->transformation.cameraPitch)),
		sin(glm::radians(this->transformation.cameraPitch)),
		sin(glm::radians(this->transformation.cameraYaw)) * cos(glm::radians(this->transformation.cameraPitch))));

	// left
	this->transformation.cameraRightLeft = glm::normalize(glm::cross(this->transformation.cameraFront, this->transformation.cameraToWorldUp));

	// up
	this->transformation.cameraUpDown = glm::normalize(glm::cross(this->transformation.cameraRightLeft, this->transformation.cameraFront));
}

void Camera::setCameraUniforms(ShaderProgram* shaderProgram) {
	shaderProgram->setMat4("ViewMatrix", glm::lookAt(this->transformation.cameraPosition, this->transformation.cameraPosition + this->transformation.cameraFront, this->transformation.cameraUpDown));

	shaderProgram->setVec3f("cameraPos", this->transformation.cameraPosition);
}

void Camera::updateInput(const float& deltaTime, const int direction, const float& x, const float& y)
{
	this->UpdateMouse(deltaTime, x, y);
	this->UpdateKeyboard(deltaTime, direction);
}

void Camera::setCameraPosition(glm::vec3 position)
{
	this->transformation.cameraPosition = position;
	this->futureCameraPosition = position;
}


void Camera::updateCameraPosition()
{
	this->transformation.cameraPosition = this->futureCameraPosition;
}

void Camera::revertCameraPosition()
{
	this->futureCameraPosition = this->transformation.cameraPosition;
}

void Camera::UpdateKeyboard(const float& deltaTime, const int direction)
{
	
	if (direction == 0) // up 
	{
		this->futureCameraPosition += this->transformation.cameraFront * speed * deltaTime;
	}
	else if (direction == 1) // down
	{
		this->futureCameraPosition -= this->transformation.cameraFront * speed * deltaTime;
	}
	else if (direction == 2) // left
	{
		this->futureCameraPosition += this->transformation.cameraRightLeft * speed * deltaTime;
	}
	else if (direction == 3 ) // right
	{
		this->futureCameraPosition -= this->transformation.cameraRightLeft * speed * deltaTime;
	}
}

void Camera::UpdateMouse(const float& deltaTime, const float& x, const float& y)
{
	//bez deltaTime, bo inaczej jest dwa razy brany pod uwagê
	this->transformation.cameraYaw += x * this->sensitivity;
	this->transformation.cameraPitch += y * this->sensitivity;

	if (this->transformation.cameraYaw > 360 || this->transformation.cameraYaw < -360) {
		this->transformation.cameraYaw = 0;
	}

	if (this->transformation.cameraPitch > 89.9f) {
		this->transformation.cameraPitch = 89.9f;
	}
	else if (this->transformation.cameraPitch < -89.9f) {
		this->transformation.cameraPitch = 89.9f;
	}
}


float Camera::getCameraZoom() const {
	return this->zoom;
}

glm::vec3 Camera::getCameraPosition() const
{
	return this->transformation.cameraPosition;
}

glm::vec3 Camera::getFutureCameraPosition() const
{
	return this->futureCameraPosition;
}

Camera::~Camera() {

}
