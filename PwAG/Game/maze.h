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

enum class MazeBlocks {
	FLOOR = 0,
	ROCK_WALL = 1,
	// todo jakieœ inne 
};

class Maze
{
public:
	glm::vec3 startPosition;
	glm::vec3 endPosition;

	Maze();
	void InitMaze();
	void InitMatrixMVP();
	void InitMazeShaders();
	void InitMazeMaterials();
	void InitMazeTextures();
	void InitObjModels();

	void DrawMaze();

	~Maze();

private:
	int mazeDimension;
	int** mazeIndexData;

	std::vector<GameObject*> walls;
	std::vector<GameObject*> floors;

	ShaderProgram* shaderProgram;

	Shader vertexShader;
	Shader fragmentShader;

	Camera* camera;

	Material* material;

	Texture* wallTexture;

};

