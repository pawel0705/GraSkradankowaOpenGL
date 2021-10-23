#include "pch.h"
#include "gameObject.h"
#include "src/Rendering system/Shader/shaderProgram.h"

GameObject::GameObject() {

}

GameObject::GameObject(
	Material* material,
	Texture* texture,
	std::vector<DataOBJ> mesh,
	glm::vec3 objectPosition,
	glm::vec3 objectOrigin,
	glm::vec3 objectRotation,
	glm::vec3 objectScale) {
	
	this->transformation.objectPosition = objectPosition;
	this->transformation.objectOrigin = objectOrigin;
	this->transformation.objectRotation = objectRotation;
	this->transformation.objectScale = objectScale;

	this->material = material;
	this->texture = texture;

	if (mesh.size() > 0) {
		this->mesh = new Mesh(mesh.data(), mesh.size(), nullptr, 0);
	}
}

void GameObject::draw(ShaderProgram* shaderProgram) {
	shaderProgram->useShader();

	this->texture->bindTexture(0);

	this->material->setMaterialShaderUniforms(*shaderProgram);

	this->mesh->setMatrixModel(
		this->transformation.objectPosition,
		this->transformation.objectOrigin,
		this->transformation.objectRotation,
		this->transformation.objectScale);

	this->mesh->setMeshUniform(shaderProgram);
	this->mesh->drawMesh(shaderProgram);
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

glm::vec3 GameObject::getPosition() {
	return this->transformation.objectPosition;
}

GameObject::~GameObject()
{
	delete this->material;
	delete this->texture;
	delete this->mesh;
}