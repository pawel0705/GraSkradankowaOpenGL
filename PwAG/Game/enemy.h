#pragma once
#include "src/Game Objects/gameObject.h"
class Enemy
{
public:
	Enemy(GameObject* enemyModel);
	~Enemy();

	void Intelligence(glm::vec3 playerPosition); //some smart enemy behavior

	void drawEnemy(ShaderProgram* shaderProgram);
	void updateEnemy(float deltaTime);


	glm::vec3 getEnemyPosition() const;

private:
	int enemyDirectionLength = 0;
	int enemySpeed = 100;

	float enemyRotation = 0;

	bool chasingPlayer = false; 
	bool whatToDo = false;

	GameObject* enemyModel;
};

