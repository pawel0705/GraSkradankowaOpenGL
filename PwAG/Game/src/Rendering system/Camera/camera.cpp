#include "pch.h"
#include "camera.h"

Camera::Camera() {

}

Camera::Camera(glm::vec3 position) {
	this->transformation.cameraPosition = position;

	this->transformation.cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	this->transformation.cameraToWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	this->transformation.cameraPitch = 0;
	this->transformation.cameraYaw = -90;

	this->speed = 2.5f;
	this->zoom = 45.0f;
	this->sensitivity = .05f;

	this->updateEulerAngels();
}

void Camera::updateEulerAngels() {

	// recalculate camera's front vector
	this->transformation.cameraFront = glm::normalize(glm::vec3(
		cos(glm::radians(this->transformation.cameraYaw)) * cos(glm::radians(this->transformation.cameraPitch)),
		sin(glm::radians(this->transformation.cameraPitch)),
		sin(glm::radians(this->transformation.cameraYaw)) * cos(glm::radians(this->transformation.cameraPitch))));

	// recalculate camera's positions vectors
	this->transformation.cameraRightLeft = glm::normalize(glm::cross(this->transformation.cameraFront, this->transformation.cameraToWorldUp));
	this->transformation.cameraUpDown = glm::normalize(glm::cross(this->transformation.cameraRightLeft, this->transformation.cameraFront));
}

void Camera::setCameraUniforms(ShaderProgram* shaderProgram) {
	shaderProgram->setMat4("ViewMatrix", glm::lookAt(this->transformation.cameraPosition, this->transformation.cameraPosition + this->transformation.cameraFront, this->transformation.cameraUpDown));

	shaderProgram->setVec3f("cameraPos", this->transformation.cameraPosition);
	//shaderProgram->setVec3f("lightPosition", this->transformation.cameraPosition);
}

void Camera::updateInput(const float& deltaTime, const int direction, const float& x, const float& y)
{
	this->UpdateMouse(deltaTime, x, y);
	this->UpdateKeyboard(deltaTime, direction);
}

void Camera::UpdateKeyboard(const float& deltaTime, const int direction)
{
	
	if (direction == 0) // up 
	{
		this->transformation.cameraPosition += this->transformation.cameraFront * speed * deltaTime;
	}
	else if (direction == 1) // down
	{
		this->transformation.cameraPosition -= this->transformation.cameraFront * speed * deltaTime;
	}
	else if (direction == 2) // left
	{
		this->transformation.cameraPosition += this->transformation.cameraRightLeft * speed * deltaTime;
	}
	else if (direction == 3) // right
	{
		this->transformation.cameraPosition -= this->transformation.cameraRightLeft * speed * deltaTime;
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

Camera::~Camera() {

}
