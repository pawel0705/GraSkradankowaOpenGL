#pragma once
#include "pch.h"
#include "src/Rendering system/Shader/shaderProgram.h"
#include "texture.h"
#include "material.h"
#include "mesh.h"
#include "transformationOBJ.h"

class GameObject
{
public:
	GameObject();
	GameObject(
		Material* material,
		Texture* texture,
		std::vector<DataOBJ> mesh,
		TransformationOBJ transformation,
		std::vector<GLfloat> offsets,
		int instances);

	void draw(ShaderProgram* shaderProgram);

	void setPosition(const glm::vec3 position);
	void setOrigin(const glm::vec3 origin);
	void setRotation(const glm::vec3 rotation);
	void setScale(const glm::vec3 scale);

	glm::vec3 getPosition();

	~GameObject();

private:
	TransformationOBJ transformation;

	Texture* texture;
	Material* material;
	Mesh* mesh;

	std::vector<GLfloat> offsets;

	int instances;

	void initGameObject();
};

