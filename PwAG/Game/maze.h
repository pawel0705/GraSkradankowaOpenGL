#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdio.h>
#include <vector>
#include "gameObject.h"
#include "camera.h"
#include "src/Rendering system/Shader/shaderProgram.h"

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

	std::vector<GameObject*> walls;
	std::vector<GameObject*> floors;
	std::vector<GameObject*> ceilings;

	ShaderProgram* shaderProgram;

	Shader vertexShader;
	Shader fragmentShader;

	Camera* camera;

	Material* material;

	Texture* wallTexture;
	Texture* floorTexture;
	Texture* ceilingTexture;
};

