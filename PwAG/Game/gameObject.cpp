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

void GameObject::Draw(ShaderProgram* shaderProgram) {
	shaderProgram->useShader();

	this->texture->bindTexture(0);

	this->material->SetMaterialAmbientUniform(*shaderProgram);

	this->mesh->SetMatrixModel(
		this->transformation.objectPosition,
		this->transformation.objectOrigin,
		this->transformation.objectRotation,
		this->transformation.objectScale);

	this->mesh->SetMeshUniform(shaderProgram);
	this->mesh->DrawMesh(shaderProgram);
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
	delete this->material;
	delete this->texture;
	delete this->mesh;
}