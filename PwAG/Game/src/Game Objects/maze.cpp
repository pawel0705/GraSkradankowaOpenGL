#include "pch.h"
#include "maze.h"
#include "../Rendering system/Model/objReader.h"
#include "tileType.h"


Maze::Maze() {
	this->initMaze();
}

void Maze::initMaze() {
	std::string mazeDataFileName = "Maps/map_1.txt";

	std::ifstream mazefile;
	mazefile.open(mazeDataFileName);

	if (!mazefile.is_open())
	{
		std::cout << "Error while reading maze data file" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string singleline;

	std::getline(mazefile, singleline);
	this->mazeDimensionX = atoi(singleline.c_str());
	std::getline(mazefile, singleline);
	this->mazeDimensionY = atoi(singleline.c_str());
	this->mazeIndexData = new int* [this->mazeDimensionX];

	for (int i = 0; i < this->mazeDimensionX; i++)
	{
		this->mazeIndexData[i] = new int[this->mazeDimensionY];
	}


	for (int j = 0; !mazefile.eof(); j++)
	{
		std::getline(mazefile, singleline);

		std::stringstream maze_line(singleline);
		for (int i = 0; i < this->mazeDimensionY; i++)
		{
			maze_line >> this->mazeIndexData[i][j];
		}
	}

	mazefile.close();

	this->initMazeMaterials();
	this->initMazeTextures();
	this->initObjModels();
	this->initMazeShaders();
	this->initMatrixMVP();
}

void Maze::initMatrixMVP() {
	glm::mat4 projectionMatrix = glm::perspective(
		glm::radians(this->camera->getCameraZoom()),
		(float)Config::g_defaultWidth / (float)Config::g_defaultHeight,
		0.1f,
		100.0f
	);

	this->shaderProgram->useShader();
	this->shaderProgram->setMat4("ProjectionMatrix", projectionMatrix);
	this->camera->setCameraUniforms(this->shaderProgram);
}

void Maze::initMazeShaders() {

	this->fragmentShader = Shader::createShaderFromFile("Shaders/map.frag", Shader::Type::eFragment);
	this->vertexShader = Shader::createShaderFromFile("Shaders/map.vert", Shader::Type::eVertex);

	this->shaderProgram = new ShaderProgram();
	this->shaderProgram->attachShader(this->fragmentShader);
	this->shaderProgram->attachShader(this->vertexShader);
	this->shaderProgram->linkShaderProgram();
}

void Maze::initMazeMaterials() {
	this->material = new Material(glm::vec3(0.25));
}

void Maze::initMazeTextures() {
	this->wallTexture = new Texture("res/Textures/wall.bmp", TextureType::BMP);
	this->floorTexture = new Texture("res/Textures/floor.bmp", TextureType::BMP);
	this->ceilingTexture = new Texture("res/Textures/ceiling.bmp", TextureType::BMP);
	this->torchTexture = new Texture("res/Textures/wood.png", TextureType::PNG);
}

void Maze::initObjModels() {
	std::vector<DataOBJ> cubeObjects = readObj("res/Models/wall.obj");
	std::vector<DataOBJ> planeObjects = readObj("res/Models/plate.obj");
	std::vector<DataOBJ> planeUpObjects = readObj("res/Models/plateUp.obj");
	std::vector<DataOBJ> torchObjects = readObj("res/Models/torch.obj");

	std::vector<GLfloat> offsetsWalls;
	std::vector<GLfloat> offsetsCeiling;
	std::vector<GLfloat> offsetsFloors;
	std::vector<GLfloat> offsetsTorches;

	int wallInstances = 0;
	int ceilingInstances = 0;
	int floorInstances = 0;
	int torchInstances = 0;

	for (int i = 0; i < this->mazeDimensionX; i++)
	{
		for (int j = 0; j < this->mazeDimensionY; j++)
		{
			if (this->mazeIndexData[i][j] == (int)TileType::WALL) {
				offsetsWalls.emplace_back(i * 2.f);
				offsetsWalls.emplace_back(0.0f);
				offsetsWalls.emplace_back(j * 2.f);

				wallInstances++;
			}
			else if (this->mazeIndexData[i][j] == (int)TileType::PLAYER_START_POS) {
				this->camera = new Camera(glm::vec3(i * 2.f, 0.0f, j * 2.f));

				offsetsFloors.emplace_back(i * 2.f);
				offsetsFloors.emplace_back(-2.0f);
				offsetsFloors.emplace_back(j * 2.f);

				offsetsCeiling.emplace_back(i * 2.f);
				offsetsCeiling.emplace_back(1.0f);
				offsetsCeiling.emplace_back(j * 2.f);

				floorInstances++;
				ceilingInstances++;
			}
			else if (this->mazeIndexData[i][j] == (int)TileType::EMPTY_SPACE) {
				offsetsFloors.emplace_back(i * 2.f);
				offsetsFloors.emplace_back(-2.0f);
				offsetsFloors.emplace_back(j * 2.f);

				offsetsCeiling.emplace_back(i * 2.f);
				offsetsCeiling.emplace_back(1.0f);
				offsetsCeiling.emplace_back(j * 2.f);

				floorInstances++;
				ceilingInstances++;
			}
		}
	}
	TransformationOBJ transformation = TransformationOBJ();
	this->walls = new GameObject(material, this->wallTexture, cubeObjects, transformation, offsetsWalls, wallInstances);
	this->floors = new GameObject(material, this->floorTexture, planeObjects, transformation, offsetsFloors, floorInstances);
	this->ceilings = new GameObject(material, this->ceilingTexture, planeUpObjects, transformation, offsetsCeiling, ceilingInstances);


	// randomize torhes
	for (int i = 0; i < floorInstances; i++) {
		int randomValue = rand() % 8;

		// some chance to spawn torch
		if (randomValue == 0) {
			int j = 3 * i;
			float x = offsetsFloors[j];
			float y = offsetsFloors[j + 2];

			int from = -8;
			int to = 8;

			float offsetX = (from + rand() % (to - from + 1)) / 10.0f;
			offsetsTorches.emplace_back(x + offsetX);
			offsetsTorches.emplace_back(-2.0f);

			float offsetY = (from + rand() % (to - from + 1)) / 10.0f;
			offsetsTorches.emplace_back(y + offsetY);

			torchInstances++;
		}
	}


	this->torches = new GameObject(material, this->torchTexture, torchObjects, transformation, offsetsTorches, torchInstances);
}

void Maze::drawMaze() {
	this->shaderProgram->useShader();
	this->camera->updateEulerAngels();
	this->camera->setCameraUniforms(this->shaderProgram);

	this->walls->draw(this->shaderProgram);
	this->floors->draw(this->shaderProgram);
	this->ceilings->draw(this->shaderProgram);
	this->torches->draw(this->shaderProgram);
}

void Maze::updateMaze()
{

}

Maze::~Maze() {
	if (this->mazeIndexData != nullptr) {
		for (int i = 0; i < this->mazeDimensionX; i++) {
			delete[] this->mazeIndexData[i];
		}
		delete[] this->mazeIndexData;
	}

	delete this->walls;

	delete this->ceilings;

	delete this->ceilings;

	delete this->torches;

	delete this->camera;

	delete this->shaderProgram;

	delete this->material;

	delete this->wallTexture;

	delete this->torchTexture;
}