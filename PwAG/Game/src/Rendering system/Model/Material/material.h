#pragma once

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

