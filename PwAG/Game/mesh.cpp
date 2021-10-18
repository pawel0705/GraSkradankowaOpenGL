#include "pch.h"
#include "mesh.h"

Mesh::Mesh() {

}

Mesh::Mesh(DataOBJ* vertices, int verticesSize, GLuint* indices, int indicesSize) {
	
	this->verticesSize = verticesSize;
	this->indicesSize = indicesSize;

	this->vertices = new DataOBJ[this -> verticesSize];
	for (int i = 0; i < this->verticesSize; i++) {
		this->vertices[i] = vertices[i];
	}

	this->indices = new GLuint[this->indicesSize];
	for (int i = 0; i < this->indicesSize; i++) {
		this->indices[i] = indices[i];
	}

	this->InitializeMesh();
}

void Mesh::InitializeMesh() {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, this->verticesSize * sizeof(DataOBJ), this->vertices, GL_STATIC_DRAW);

	if (this->indicesSize > 0)
	{
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indicesSize * sizeof(GLuint), this->indices, GL_STATIC_DRAW);
	}

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DataOBJ), (GLvoid*)offsetof(DataOBJ, vertex));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DataOBJ), (GLvoid*)offsetof(DataOBJ, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(DataOBJ), (GLvoid*)offsetof(DataOBJ, uv));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(DataOBJ), (GLvoid*)offsetof(DataOBJ, normal));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}

void Mesh::SetMeshUniform(ShaderProgram* shaderProgram) {
	shaderProgram->setMat4("ModelMatrix", matrixModel);
}

void Mesh::SetMatrixModel(glm::vec3 position, glm::vec3 origin, glm::vec3 rotation, glm::vec3 scale)
{
	this->matrixModel = glm::mat4(1.f);
	this->matrixModel = glm::translate(this->matrixModel, origin);
	this->matrixModel = glm::rotate(this->matrixModel, glm::radians(rotation.x), glm::vec3(1.f, 0.f, 0.f));
	this->matrixModel = glm::rotate(this->matrixModel, glm::radians(rotation.y), glm::vec3(0.f, 1.f, 0.f));
	this->matrixModel = glm::rotate(this->matrixModel, glm::radians(rotation.z), glm::vec3(0.f, 0.f, 1.f));
	this->matrixModel = glm::translate(this->matrixModel, position - origin);
	this->matrixModel = glm::scale(this->matrixModel, scale);
}

void Mesh::DrawMesh(ShaderProgram* shaderProgram) {
	glBindVertexArray(VAO);
	shaderProgram->useShader();

	if (this->indicesSize > 0) {
		glDrawElements(GL_TRIANGLES, this->indicesSize, GL_UNSIGNED_INT, 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, this->verticesSize);
	}
}

Mesh::~Mesh() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	if (this->indicesSize > 0)
	{
		glDeleteBuffers(1, &EBO);
	}

	delete[] this->vertices;
	delete[] this->indices;
}