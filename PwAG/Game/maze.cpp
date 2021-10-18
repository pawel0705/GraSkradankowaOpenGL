#include "pch.h"
#include "maze.h"
#include "objReader.h"


Maze::Maze() {
	this->InitMaze();
}

void Maze::InitMaze() {
	std::string mazeDataFileName = "maze1.txt";

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

	this->InitMazeMaterials();
	this->InitMazeTextures();
	this->InitObjModels();
	this->InitMazeShaders();
	this->InitMatrixMVP();
}

void Maze::InitMatrixMVP() {
	glm::mat4 projectionMatrix = glm::perspective(
		glm::radians(this->camera->GetCameraZoom()),
		(float)Config::g_defaultWidth / (float)Config::g_defaultHeight,
		0.1f,
		100.0f
	);

	this->shaderProgram->useShader();
	this->shaderProgram->setMat4("ProjectionMatrix", projectionMatrix);
	this->camera->SetCameraUniforms(this->shaderProgram);
}

void Maze::InitMazeShaders() {

	this->fragmentShader = Shader::createShaderFromFile("frag.fs", Shader::Type::eFragment);
	this->vertexShader = Shader::createShaderFromFile("vert.vs", Shader::Type::eVertex);

	this->shaderProgram = new ShaderProgram();
	this->shaderProgram->attachShader(this->fragmentShader);
	this->shaderProgram->attachShader(this->vertexShader);
	this->shaderProgram->linkShaderProgram();
}

void Maze::InitMazeMaterials() {
	this->material = new Material(glm::vec3(0.95));
}

void Maze::InitMazeTextures() {
	this->wallTexture = new Texture("Textures/grass.bmp");
}

void Maze::InitObjModels() {

	std::vector<DataOBJ> wallsObjects = readObj("Models/cube.obj");

	for (int i = 0; i < this->mazeDimension; i++)
	{
		for (int j = 0; j < this->mazeDimension; j++)
		{
			if (this->mazeIndexData[i][j] == 1) {
				this->walls.push_back(new GameObject(material, this->wallTexture, wallsObjects, glm::vec3(i * 2.f, -0.5f, j * 2.f)));
			}
			else if (this->mazeIndexData[i][j] == 2) {
				this->camera = new Camera(glm::vec3(i * 2.f, 40.0f, j * 2.f));
			}
		}
	}
}

void Maze::DrawMaze() {
	this->shaderProgram->useShader();
	this->camera->SetCameraUniforms(this->shaderProgram);


	for (auto& element : this->walls) {
		element->Draw(this->shaderProgram);
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