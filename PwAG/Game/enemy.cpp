#include "pch.h"
#include "enemy.h"

Enemy::Enemy(GameObject* enemyModel)
{
	this->enemyModel = enemyModel;
}

Enemy::~Enemy()
{
	delete this->enemyModel;
}

void Enemy::Intelligence(glm::vec3 playerPosition)
{
}

void Enemy::drawEnemy(ShaderProgram* shaderProgram)
{
	this->enemyModel->draw(shaderProgram);
}

void Enemy::updateEnemy(float deltaTime)
{
	std::vector<GLfloat> offsetEnemy = this->enemyModel->getOffsets();

	if (offsetEnemy.size() >= 3) {
			this->enemyModel->setOrigin(glm::vec3(offsetEnemy[0] + 1, offsetEnemy[1] + 1, offsetEnemy[2] + 1));
			this->enemyModel->setRotation(glm::vec3(0, enemyRotation += deltaTime *25, 0.f));
	}


}

glm::vec3 Enemy::getEnemyPosition() const
{
	std::vector<GLfloat> offsetEnemy = this->enemyModel->getOffsets();

	if (offsetEnemy.size() >= 3) {
		return glm::vec3(offsetEnemy[0], offsetEnemy[1], offsetEnemy[2]);
	}

	return glm::vec3(-1, -1, -1);
}
