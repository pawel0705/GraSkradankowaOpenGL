#pragma once
#include "gameObject.h"
#include "smokeBomb.h"
#include "../Rendering system/Camera/camera.h"
#include "../Rendering system/Lighting/point.h"
#include "../Rendering system/Particle system/particleEmitter.h"
#include "../../enemy.h"
#include "../../respawnPoint.h"
#include "../Rendering system/Deferred rendering/deferredRenderer.h"

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
	bool willBeCollisionWithExit();

	void useSmokeBomb();

	virtual ~Maze();

private:
	void deferred_geometryPass();
	void deferred_lightingPass();

	void setLightUniforms(ShaderProgram& shader);
	void updateSmokeBombs(float deltaTime);

	void updateEnemy(float deltaTime);
	void updateRespawnPoint(float deltaTime);

	int mazeDimensionX;
	int mazeDimensionY;
	int** mazeIndexData;

	GameObject* walls;
	GameObject* floors;
	GameObject* ceilings;
	GameObject* torches;
	GameObject* exitDoors;
	GameObject* grass1;
	GameObject* grass2;
	GameObject* grass3;

	std::vector<RespawnPoint*> respawnPickup;
	std::vector<Enemy*> opponents;
	std::vector<Light::Point> pointLights;
	std::vector<ParticleEmitter> torchesParticleEmitters;

	std::vector<SmokeBomb> smokeBombs;
	const float smokeBombCooldown = 0.5f;
	float smokeBombCooldownLeft = 0.0f;

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
	Texture* exitDoorTexture;
	Texture* grass_1Texture;
	Texture* grass_2Texture;
	Texture* grass_3Texture;
	Texture* spawnActiveTexture;
	Texture* spawnInactiveTexture;

	Texture* normalMapWall;
	Texture* normalMapCeiling;
	Texture* normalMapFloor;
	Texture* normalMapDoors;

	Texture* specularMapWall;
	Texture* specularMapCeiling;
	Texture* specularMapFloor;
	Texture* specularMapWood;
	Texture* specularDoors;

	Texture* enemyTexture;

	std::vector<GLfloat> offsetsWalls;
	std::vector<GLfloat> offsetsCeiling;
	std::vector<GLfloat> offsetsFloors;
	std::vector<GLfloat> offsetsTorches;
	std::vector<GLfloat> offsetsExitDoors;
	std::vector<GLfloat> offsetsGrass1;
	std::vector<GLfloat> offsetsGrass2;
	std::vector<GLfloat> offsetsGrass3;

	DeferredRenderer deferred;
};

