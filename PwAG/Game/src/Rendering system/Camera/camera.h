#pragma once

// TODO kamera do edycji => pod gracza z pierwszej osoby
class Camera
{
public:
	Camera();
	Camera(glm::vec3 position = glm::vec3(.0f));

	void updateCameraEulerAng();
	void setCameraUniforms(ShaderProgram* shaderProgram);
	void updateInput(const float& deltaTime, const int direction, const float& x, const float& y);
	void setCameraPosition(glm::vec3 position);
	
	void updateCameraPosition();
	void revertCameraPosition();

	float getCameraZoom() const;
	glm::vec3 getCameraPosition() const;
	glm::vec3 getFutureCameraPosition() const;

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

	glm::vec3 futureCameraPosition; // przechowuje koordynaty kamery, które bêd¹ podmienione je¿eli nie zajdzie na nich kolizja ze œcian¹

	float speed;
	float zoom;
	float sensitivity;

	void UpdateKeyboard(const float& deltaTime, const int direction);
	void UpdateMouse(const float& deltaTime, const float& x, const float& y);
};

