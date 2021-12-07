#include "pch.h"
#include "respawnPoint.h"

RespawnPoint::RespawnPoint(GameObject* respawnPointModel, bool isActive)
{
	this->respawnPointModel = respawnPointModel;
	this->isActive = isActive;
}

RespawnPoint::~RespawnPoint()
{
	delete this->respawnPointModel;
}

void RespawnPoint::drawRespawnPoint(ShaderProgram* shaderProgram)
{
	this->respawnPointModel->draw(shaderProgram);
}

void RespawnPoint::updateRespawnPoint(float deltaTime)
{
	if (this->isActive == true) {
		std::vector<GLfloat> offset = this->respawnPointModel->getOffsets();

		glm::vec3 pos = this->respawnPointModel->getPosition();

		if (offset.size() >= 3) {
			this->respawnPointModel->setOrigin(glm::vec3(offset[0] + pos.x, offset[1] + pos.y, offset[2] + pos.z));
			this->respawnPointModel->setRotation(glm::vec3(0, respawnPointRotation += deltaTime * 25, 0.f));
		}
	}
}

void RespawnPoint::setRespawnActivation(bool isActive)
{
	this->isActive = isActive;
}


