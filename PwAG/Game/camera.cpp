#include "pch.h"
#include "camera.h"



Camera::Camera() {

}

Camera::Camera(glm::vec3 position) {
	this->transformation.cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	this->speed = 1.0f;
	this->zoom = 45.0f;
	this->transformation.cameraPitch = -70;
	this->transformation.cameraYaw = -90;
	this->transformation.cameraPosition = position;
	this->transformation.cameraToWorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	this->UpdateEulerAngels();
}

void Camera::UpdateEulerAngels() {

	// recalculate camera's front vector
	this->transformation.cameraFront = glm::normalize(glm::vec3(
		cos(glm::radians(this->transformation.cameraYaw)) * cos(glm::radians(this->transformation.cameraPitch)),
		sin(glm::radians(this->transformation.cameraPitch)),
		sin(glm::radians(this->transformation.cameraYaw)) * cos(glm::radians(this->transformation.cameraPitch))));
	
	// recalculate camera's positions vectors
	this->transformation.cameraRightLeft = glm::normalize(glm::cross(this->transformation.cameraFront, this->transformation.cameraToWorldUp));
	this->transformation.cameraUpDown = glm::normalize(glm::cross(this->transformation.cameraRightLeft, this->transformation.cameraFront));
}

void Camera::SetCameraUniforms(ShaderProgram* shaderProgram) {
	shaderProgram->setMat4("ViewMatrix", glm::lookAt(this->transformation.cameraPosition, this->transformation.cameraPosition + this->transformation.cameraFront, this->transformation.cameraUpDown));
}

float Camera::GetCameraZoom() const {
	return this->zoom;
}

Camera::~Camera() {

}