#pragma once
#include "src/Rendering system/Shader/shaderProgram.h"
class Material
{
public:
	Material();
	Material(glm::vec3 ambient);

	void setMaterialShaderUniforms(ShaderProgram &shaderProgram);

	virtual ~Material();

private:
	// glm::vec3 ambient;
	// TODO warto�ci definiuj�ce materia�, takie jak �wiat�o ambient itp.
};

