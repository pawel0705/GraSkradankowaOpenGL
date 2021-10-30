#pragma once

#include "src/Rendering system/Shader/shaderProgram.h"

// TODO kamera do edycji => pod gracza z pierwszej osoby
class Camera
{
public:
	Camera();
	Camera(glm::vec3 position = glm::vec3(.0f));

	void updateEulerAngels();

	void setCameraUniforms(ShaderProgram* shaderProgram);

	float getCameraZoom() const;

	virtual ~Camera();

private:
	struct {
		glm::vec3 cameraPosition;
		glm::vec3 cameraFront;
		glm::vec3 cameraUpDown;
		glm::vec3 cameraRightLeft;
		glm::vec3 cameraToWorldUp;
		float cameraPitch;
		float cameraYaw;
	} transformation;

	float speed;
	float zoom;
};

