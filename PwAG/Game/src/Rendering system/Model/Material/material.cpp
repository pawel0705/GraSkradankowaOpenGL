#include "pch.h"
#include "material.h"

Material::Material() {

}

Material::Material(glm::vec3 ambient) {

}

void Material::setMaterialShaderUniforms(ShaderProgram& shaderProgram) {
	shaderProgram.setInt("diffuse", 0);
}

Material::~Material()
{

}