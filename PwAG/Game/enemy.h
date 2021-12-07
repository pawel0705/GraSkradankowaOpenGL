#pragma once
#include "src/Game Objects/gameObject.h"
class Enemy
{
public:
	Enemy(GameObject* enemyModel);
	~Enemy();

	void drawEnemy(ShaderProgram* shaderProgram);
	void updateEnemy(float deltaTime);

	void setEnemyPosition(glm::vec3 position);

	glm::vec3 getEnemyPosition() const;
	glm::vec3 getEnemyPositionWithoutOffset() const;

	void resetEnemyPosition();

private:
	float enemyRotation = 0;

	GameObject* enemyModel;

	glm::vec3 enemyStartPosition;
};

