#pragma once

class Material
{
public:
	Material();
	Material(glm::vec3 ambientLight);

	void setMaterialShaderUniforms(ShaderProgram &shaderProgram);

	virtual ~Material();

private:
	 glm::vec3 ambientLight = glm::vec3(0.1f, 0.1f, 0.1f);
	// TODO warto�ci definiuj�ce materia�, takie jak �wiat�o ambient itp.
};

