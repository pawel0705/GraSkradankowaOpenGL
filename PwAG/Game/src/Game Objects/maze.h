#pragma once
#include "gameObject.h"
#include "../Rendering system/Camera/camera.h"

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

	virtual ~Maze();

private:
	int mazeDimensionX;
	int mazeDimensionY;
	int** mazeIndexData;

	GameObject* walls;
	GameObject* floors;
	GameObject* ceilings;
	GameObject* torches;

	ShaderProgram* shaderProgram;

	Shader vertexShader;
	Shader fragmentShader;

	Material* material;

	Texture* wallTexture;
	Texture* floorTexture;
	Texture* ceilingTexture;
	Texture* torchTexture;
};

