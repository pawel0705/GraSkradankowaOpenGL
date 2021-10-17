#pragma once

#include "src/Rendering system/Shader/shaderProgram.h"

// info kamera na razie mo¿e byæ beznadziejna, ale chcia³bym mieæ podgl¹d, póŸniej bêdê chcia³ j¹ poprawiæ

class Camera
{
public:
	Camera();
	Camera(glm::vec3 position = glm::vec3(.0f));

	void UpdateEulerAngels();

	void SetCameraUniforms(ShaderProgram* shaderProgram);

	float GetCameraZoom() const;

	~Camera();

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

