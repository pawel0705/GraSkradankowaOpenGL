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
	this->mazeDimension = atoi(singleline.c_str());
	this->mazeIndexData = new int* [this->mazeDimension];

	for (int i = 0; i < this->mazeDimension; i++)
	{
		this->mazeIndexData[i] = new int[this->mazeDimension];
	}


	for (int j = 0; !mazefile.eof(); j++)
	{
		std::getline(mazefile, singleline);

		std::stringstream maze_line(singleline);
		for (int i = 0; i < this->mazeDimension; i++)
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

	this->fragmentShader = Shader::createShaderFromFile("Shaders/frag.fs", Shader::Type::eFragment);
	this->vertexShader = Shader::createShaderFromFile("Shaders/vert.vs", Shader::Type::eVertex);

	this->shaderProgram = new ShaderProgram();
	this->shaderProgram->attachShader(this->fragmentShader);
	this->shaderProgram->attachShader(this->vertexShader);
	this->shaderProgram->linkShaderProgram();
}

void Maze::initMazeMaterials() {
	this->material = new Material(glm::vec3(0.95));
}

void Maze::initMazeTextures() {
	this->wallTexture = new Texture("res/Textures/wall.bmp");
	this->floorTexture = new Texture("res/Textures/floor.bmp");
	this->ceilingTexture = new Texture("res/Textures/ceiling.bmp");
}

void Maze::initObjModels() {

	std::vector<DataOBJ> cubeObjects = readObj("res/Models/cube.obj");
	std::vector<DataOBJ> planeObjects = readObj("res/Models/plate.obj", glm::vec3(0.0, 1.0, 0.0));

	std::vector<GLfloat> offsetsWalls;
	std::vector<GLfloat> offsetsCeiling;
	std::vector<GLfloat> offsetsFloors;

	int wallInstances = 0;
	int ceilingInstances = 0;
	int floorInstances = 0;

	for (int i = 0; i < this->mazeDimension; i++)
	{
		for (int j = 0; j < this->mazeDimension; j++)
		{
			if (this->mazeIndexData[i][j] == (int)TileType::WALL) {
				offsetsWalls.emplace_back(i * 2.f);
				offsetsWalls.emplace_back(0.0f);
				offsetsWalls.emplace_back(j * 2.f);

				wallInstances++;
			}
			else if (this->mazeIndexData[i][j] == (int)TileType::PLAYER_START_POS) {
				this->camera = new Camera(glm::vec3(i * 2.f, 1.0f, j * 2.f));
				offsetsFloors.emplace_back(i * 2.f);
				offsetsFloors.emplace_back(-1.0f);
				offsetsFloors.emplace_back(j * 2.f);

				offsetsCeiling.emplace_back(i * 2.f);
				offsetsCeiling.emplace_back(1.0f);
				offsetsCeiling.emplace_back(j * 2.f);

				floorInstances++;
				ceilingInstances++;
			}
			else if (this->mazeIndexData[i][j] == (int)TileType::EMPTY_SPACE) {
				offsetsFloors.emplace_back(i * 2.f);
				offsetsFloors.emplace_back(-1.0f);
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
	this->ceilings = new GameObject(material, this->ceilingTexture, planeObjects, transformation, offsetsCeiling, ceilingInstances);
}

void Maze::drawMaze() {
	this->shaderProgram->useShader();
	this->camera->setCameraUniforms(this->shaderProgram);

	this->walls->draw(this->shaderProgram);
	this->floors->draw(this->shaderProgram);
	this->ceilings->draw(this->shaderProgram);
}

Maze::~Maze() {
	if (this->mazeIndexData != nullptr) {
		for (int i = 0; i < this->mazeDimension; i++) {
			delete[] this->mazeIndexData[i];
		}
		delete[] this->mazeIndexData;
	}

	delete this->walls;

	delete this->ceilings;

	delete this->ceilings;

	delete this->camera;

	delete this->shaderProgram;

	delete this->material;

	delete this->wallTexture;
}