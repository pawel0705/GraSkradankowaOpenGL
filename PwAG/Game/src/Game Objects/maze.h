#pragma once
#include "gameObject.h"
#include "../Rendering system/Camera/camera.h"
#include "../Rendering system/Lighting/point.h"
#include "../Rendering system/Particle system/particleEmitter.h"
#include "../../enemy.h"
#include "../../respawnPoint.h"

class Maze
{
public:
	glm::vec3 startPosition;
	glm::vec3 endPosition;

	Maze();

	void initMaze();
	void initMatrixMVP();
	void initMazeShaders();
	void initMazeMaterials();
	void initMazeTextures();
	void initObjModels();

	Camera* camera;

	void drawMaze(float deltaTime);
	void updateMaze(float deltaTime);

	bool willBeCollisionWithWall(float deltaTime);

	virtual ~Maze();

private:
	void setLightUniforms(ShaderProgram*& shader);

	void updateEnemy(float deltaTime);

	int mazeDimensionX;
	int mazeDimensionY;
	int** mazeIndexData;

	GameObject* walls;
	GameObject* floors;
	GameObject* ceilings;
	GameObject* torches;
	GameObject* grass1;
	GameObject* grass2;
	GameObject* grass3;

	std::vector<RespawnPoint*> respawnPickup;
	std::vector<Enemy*> opponents;
	std::vector<Light::Point> pointLights;
	std::vector<ParticleEmitter> torchesParticleEmitters;

	ShaderProgram* shaderProgram;
	ShaderProgram* shaderGrassProgram;
	ShaderProgram* shaderPickupProgram;

	ShaderProgram shaderParticles;

	Shader vertexShader;
	Shader fragmentShader;

	Shader vertexShaderGrass;
	Shader fragmentShaderGrass;

	Shader vertexShaderPickup;
	Shader fragmentShaderPickup;

	Material* material;

	Texture* wallTexture;
	Texture* floorTexture;
	Texture* ceilingTexture;
	Texture* torchTexture;
	Texture* grass_1Texture;
	Texture* grass_2Texture;
	Texture* grass_3Texture;
	Texture* spawnActiveTexture;
	Texture* spawnInactiveTexture;

	Texture* normalMapWall;
	Texture* normalMapCeiling;
	Texture* normalMapFloor;

	Texture* enemyTexture;

	std::vector<GLfloat> offsetsWalls;
	std::vector<GLfloat> offsetsCeiling;
	std::vector<GLfloat> offsetsFloors;
	std::vector<GLfloat> offsetsTorches;
	std::vector<GLfloat> offsetsGrass1;
	std::vector<GLfloat> offsetsGrass2;
	std::vector<GLfloat> offsetsGrass3;
};

