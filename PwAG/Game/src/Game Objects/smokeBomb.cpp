#include "pch.h"
#include "smokeBomb.h"

float SmokeBomb::maxDurationTime = 10.0f;

SmokeBomb::SmokeBomb(const glm::vec3& position)
	: emitter(position, ResourceManager::getInstance().getTexture("smoke"))
{
}

void SmokeBomb::update(float deltaTime)
{
	durationTime += deltaTime;
	emitter.update(deltaTime);
}

void SmokeBomb::render(float deltaTime, const ShaderProgram& particleShader)
{
	emitter.render(particleShader);
}