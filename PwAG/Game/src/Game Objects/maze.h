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

	void drawMaze();

	virtual ~Maze();

private:
	int mazeDimension;
	int** mazeIndexData;

	GameObject* walls;
	GameObject* floors;
	GameObject* ceilings;

	ShaderProgram* shaderProgram;

	Shader vertexShader;
	Shader fragmentShader;

	Camera* camera;

	Material* material;

	Texture* wallTexture;
	Texture* floorTexture;
	Texture* ceilingTexture;
};

