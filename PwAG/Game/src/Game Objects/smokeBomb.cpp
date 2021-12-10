#include "pch.h"
#include "smokeBomb.h"

float SmokeBomb::maxDurationTime = 10.0f;

SmokeBomb::SmokeBomb(const glm::vec3& position)
	: emitter(position, {0.0f, 0.1f, 0.0f}, {0.0f, 0.0f ,0.0f}, ResourceManager::getInstance().getTexture("smoke"), glm::vec2(4.0f, 4.0f))
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