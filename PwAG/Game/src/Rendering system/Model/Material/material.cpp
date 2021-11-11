#include "pch.h"
#include "material.h"

Material::Material() {

}

Material::Material(glm::vec3 ambientLight) {
	this->ambientLight = ambientLight;
}

void Material::setMaterialShaderUniforms(ShaderProgram& shaderProgram) {
	shaderProgram.setFloat("diffuse", 0.0f);

	shaderProgram.setVec3f("ambientLight", this->ambientLight);
}

Material::~Material()
{

}