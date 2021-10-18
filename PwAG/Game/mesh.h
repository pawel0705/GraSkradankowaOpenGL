#pragma once
#include "dataOBJ.h"
#include "src/Rendering system/Shader/shaderProgram.h"

// data set for vertices, indices, textures
class Mesh
{
public:
	Mesh();
	Mesh(DataOBJ* vertices, int verticesSize, GLuint* indices, int indicesSize);

	void DrawMesh(ShaderProgram* shaderProgram);
	void SetMeshUniform(ShaderProgram* shaderProgram);
	void SetMatrixModel(glm::vec3 position, glm::vec3 origin, glm::vec3 rotation, glm::vec3 scale);

	~Mesh();

private:
	DataOBJ* vertices;
	GLuint* indices;
	
	int verticesSize;
	int indicesSize;

	glm::mat4 matrixModel;

	GLuint VAO, VBO, EBO;

	void InitializeMesh();
};

