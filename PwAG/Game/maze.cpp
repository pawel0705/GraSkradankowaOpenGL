#include "pch.h"
#include "maze.h"


Maze::Maze() {

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
			maze_line >> this->mazeIndexData[j][i];
		}
	}

	mazefile.close();
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
	// TODO init shaders
	//this->shaderProgram = new ShaderProgram();
}

void Maze::InitMazeMaterials() {
	// TODO
}

void Maze::InitMazeTextures() {
	// TODO
}

void Maze::InitObjModels() {
	// TODO load walls as .obj and other models
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
}