#include "pch.h"
#include "maze.h"
#include "objReader.h"
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

	this->fragmentShader = Shader::createShaderFromFile("frag.fs", Shader::Type::eFragment);
	this->vertexShader = Shader::createShaderFromFile("vert.vs", Shader::Type::eVertex);

	this->shaderProgram = new ShaderProgram();
	this->shaderProgram->attachShader(this->fragmentShader);
	this->shaderProgram->attachShader(this->vertexShader);
	this->shaderProgram->linkShaderProgram();
}

void Maze::initMazeMaterials() {
	this->material = new Material(glm::vec3(0.95));
}

void Maze::initMazeTextures() {
	this->wallTexture = new Texture("Textures/wall.bmp");
	this->floorTexture = new Texture("Textures/floor.bmp");
	this->ceilingTexture = new Texture("Textures/ceiling.bmp");
}

void Maze::initObjModels() {

	std::vector<DataOBJ> cubeObjects = readObj("Models/cube.obj");
	std::vector<DataOBJ> planeObjects = readObj("Models/plate.obj", glm::vec3(0.0, 1.0, 0.0));

	for (int i = 0; i < this->mazeDimension; i++)
	{
		for (int j = 0; j < this->mazeDimension; j++)
		{
			if (this->mazeIndexData[i][j] == (int)TileType::WALL) {
				this->walls.push_back(new GameObject(material, this->wallTexture, cubeObjects, glm::vec3(i * 2.f, 0.0f, j * 2.f)));
			}
			else if (this->mazeIndexData[i][j] == (int)TileType::PLAYER_START_POS) {
				this->camera = new Camera(glm::vec3(i * 2.f, 0.1f, j * 2.f));
				this->floors.push_back(new GameObject(material, this->floorTexture, planeObjects, glm::vec3(i * 2.f, -1.0f, j * 2.f)));
			}
			else if (this->mazeIndexData[i][j] == (int)TileType::EMPTY_SPACE) {
				this->floors.push_back(new GameObject(material, this->floorTexture, planeObjects, glm::vec3(i * 2.f, -1.0f, j * 2.f)));
				this->ceilings.push_back(new GameObject(material, this->ceilingTexture, planeObjects, glm::vec3(i * 2.f, 1.0f, j * 2.f)));
			}
		}
	}
}

void Maze::drawMaze() {
	this->shaderProgram->useShader();
	this->camera->setCameraUniforms(this->shaderProgram);


	for (auto& element : this->walls) {
		element->draw(this->shaderProgram);
	}

	for (auto& element : this->floors) {
		element->draw(this->shaderProgram);
	}

	for (auto& element : this->ceilings) {
		element->draw(this->shaderProgram);
	}
}

Maze::~Maze() {
	if (this->mazeIndexData != nullptr) {
		for (int i = 0; i < this->mazeDimension; i++) {
			delete[] this->mazeIndexData[i];
		}
		delete[] this->mazeIndexData;
	}

	for (auto p : this->walls)
	{
		delete p;
	}
	this->walls.clear();

	for (auto p : this->floors)
	{
		delete p;
	}
	this->floors.clear();

	delete this->camera;

	delete this->shaderProgram;

	delete this->material;

	delete this->wallTexture;
}