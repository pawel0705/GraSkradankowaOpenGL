#pragma once
#include "src/Rendering system/Shader/shaderProgram.h"
class Material
{
public:
	Material();
	Material(glm::vec3 ambient);

	void SetMaterialAmbientUniform(ShaderProgram &shaderProgram);

	~Material();

private:
	glm::vec3 ambient;
};

