#include "pch.h"
#include "gameObject.h"
#include "src/Rendering system/Shader/shaderProgram.h"

GameObject::GameObject() {

}

GameObject::GameObject(glm::vec3 objectPosition,
	glm::vec3 objectOrigin,
	glm::vec3 objectRotation,
	glm::vec3 objectScale) {
	
	this->transformation.objectPosition = objectPosition;
	this->transformation.objectOrigin = objectOrigin;
	this->transformation.objectRotation = objectRotation;
	this->transformation.objectScale = objectScale;
}

void GameObject::Draw(ShaderProgram* shaderProgram) {
	shaderProgram->useShader();

	//TODO texture, material, mesh
}

void GameObject::SetPosition(const glm::vec3 position)
{
	this->transformation.objectPosition = position;
}

void GameObject::SetOrigin(const glm::vec3 origin)
{
	this->transformation.objectOrigin = origin;
}

void GameObject::SetRotation(const glm::vec3 rotation)
{
	this->transformation.objectRotation = rotation;
}

void GameObject::SetScale(const glm::vec3 scale)
{
	this->transformation.objectScale = scale;
}

glm::vec3 GameObject::GetPosition() {
	return this->transformation.objectPosition;
}

GameObject::~GameObject()
{

}