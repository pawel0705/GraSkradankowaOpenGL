#include "pch.h"
#include "material.h"

Material::Material() {

}

Material::Material(glm::vec3 ambient) {
	this->ambient = ambient;
}

void Material::SetMaterialAmbientUniform(ShaderProgram& shaderProgram) {
	shaderProgram.setVec3f("material.ambient", this->ambient);
}

Material::~Material()
{

}