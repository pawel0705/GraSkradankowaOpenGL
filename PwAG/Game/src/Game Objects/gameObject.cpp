#include "pch.h"
#include "gameObject.h"

GameObject::GameObject() {

}

GameObject::GameObject(
	Material* material,
	Texture* texture,
	std::vector<DataOBJ> mesh,
	TransformationOBJ transformation,
	std::vector<GLfloat> offsets,
	int instances) {
	
	this->transformation = transformation;
	this->offsets = offsets;
	this->instances = instances;

	this->material = material;
	this->texture = texture;

	if (mesh.size() > 0) {
		this->mesh = new Mesh(mesh.data(), mesh.size(), nullptr, 0);
	}

	this->initGameObject();
}

void GameObject::initGameObject() {
	glGenVertexArrays(1, &this->mesh->VAO);
	glBindVertexArray(this->mesh->VAO);

	glGenBuffers(1, &this->mesh->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->mesh->verticesSize * sizeof(DataOBJ), this->mesh->vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DataOBJ), (GLvoid*)offsetof(DataOBJ, vertex));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DataOBJ), (GLvoid*)offsetof(DataOBJ, color));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(DataOBJ), (GLvoid*)offsetof(DataOBJ, normal));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(DataOBJ), (GLvoid*)offsetof(DataOBJ, uv));
	glEnableVertexAttribArray(3);

	GLuint offsetBuffer;
	glGenBuffers(1, &offsetBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, offsetBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->instances * 3 * sizeof(GLfloat), &this->offsets[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(4);

	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
	glVertexAttribDivisor(4, 1);

	glBindVertexArray(0);
}

void GameObject::draw(ShaderProgram* shaderProgram) {
	shaderProgram->useShader();
	
	this->texture->bindTexture(0);

	if (this->normalMapTexture != nullptr) {
		this->normalMapTexture->bindTexture(1);
	}

	this->material->setMaterialShaderUniforms(*shaderProgram);


	this->mesh->setMatrixModel(
		this->transformation.objectPosition,
		this->transformation.objectOrigin,
		this->transformation.objectRotation,
		this->transformation.objectScale);

	this->mesh->setMeshUniform(shaderProgram);

	glBindVertexArray(this->mesh->VAO);

	if (this->mesh->indicesSize > 0) {
		glDrawElementsInstanced(GL_TRIANGLES, this->mesh->indicesSize, GL_UNSIGNED_INT, 0, this->instances);
	}
	else {
		glDrawArraysInstanced(GL_TRIANGLES, 0, this->mesh->verticesSize, this->instances);
	}
}

void GameObject::setPosition(const glm::vec3 position)
{
	this->transformation.objectPosition = position;
}

void GameObject::setOrigin(const glm::vec3 origin)
{
	this->transformation.objectOrigin = origin;
}

void GameObject::setRotation(const glm::vec3 rotation)
{
	this->transformation.objectRotation = rotation;
}

void GameObject::setScale(const glm::vec3 scale)
{
	this->transformation.objectScale = scale;
}

void GameObject::setNormalMapTexture(Texture* normalMapTexture)
{
	this->normalMapTexture = normalMapTexture;
}

glm::vec3 GameObject::getPosition() {
	return this->transformation.objectPosition;
}

glm::vec3 GameObject::getRotation() {
	return this->transformation.objectRotation;
}

std::vector<GLfloat> GameObject::getOffsets()
{
	return this->offsets;
}

GameObject::~GameObject()
{
	delete this->material;
	delete this->texture;
	delete this->mesh;

	if (this->normalMapTexture != nullptr) {
		delete this->normalMapTexture;
	}
}