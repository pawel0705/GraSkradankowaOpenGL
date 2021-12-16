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
		for (int i = 0; i < mesh.size(); i+=3) {
			// Shortcuts for vertices
			glm::vec3& v0 = mesh[i + 0].vertex;
			glm::vec3& v1 = mesh[i + 1].vertex;
			glm::vec3& v2 = mesh[i + 2].vertex;

			// Shortcuts for UVs
			glm::vec2& uv0 = mesh[i + 0].uv;
			glm::vec2& uv1 = mesh[i + 1].uv;
			glm::vec2& uv2 = mesh[i + 2].uv;

			// Edges of the triangle : position delta
			glm::vec3 deltaPos1 = v1 - v0;
			glm::vec3 deltaPos2 = v2 - v0;

			// UV delta
			glm::vec2 deltaUV1 = uv1 - uv0;
			glm::vec2 deltaUV2 = uv2 - uv0;

			float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
			glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
			glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

			mesh[i + 0].tangent = tangent;
			mesh[i + 1].tangent = tangent;
			mesh[i + 2].tangent = tangent;

			mesh[i + 0].bittangent = bitangent;
			mesh[i + 1].bittangent = bitangent;
			mesh[i + 2].bittangent = bitangent;
		}

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

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DataOBJ), (GLvoid*)offsetof(DataOBJ, vertex));

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DataOBJ), (GLvoid*)offsetof(DataOBJ, color));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(DataOBJ), (GLvoid*)offsetof(DataOBJ, normal));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(DataOBJ), (GLvoid*)offsetof(DataOBJ, tangent));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(DataOBJ), (GLvoid*)offsetof(DataOBJ, bittangent));

	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(DataOBJ), (GLvoid*)offsetof(DataOBJ, uv));


	GLuint offsetBuffer;
	glGenBuffers(1, &offsetBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, offsetBuffer);
	glBufferData(GL_ARRAY_BUFFER, this->instances * 3 * sizeof(GLfloat), &this->offsets[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(6);

	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
	glVertexAttribDivisor(6, 1);

	glBindVertexArray(0);
}

void GameObject::draw(ShaderProgram* shaderProgram) {
	shaderProgram->useShader();
	
	this->texture->bindTexture(0);
	if(this->specular)
	{
		this->specular->bindTexture(1);
	}

	if (this->normalMapTexture != nullptr) {
		this->normalMapTexture->bindTexture(2);
	}

	this->mesh->setMeshUniform(shaderProgram);

	this->material->setMaterialShaderUniforms(*shaderProgram);

	this->mesh->setMatrixModel(
		this->transformation.objectPosition,
		this->transformation.objectOrigin,
		this->transformation.objectRotation,
		this->transformation.objectScale);

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

void GameObject::setTexture(Texture* texture)
{
	this->texture = texture;
}

void GameObject::setSpecular(Texture* texture)
{
	this->specular = texture;
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