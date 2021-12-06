#pragma once
#include "particle.h"
#include "../Texture/texture.h"

class ParticleEmitter
{
public:
	ParticleEmitter(const glm::vec3& position);
	ParticleEmitter(const ParticleEmitter&) = delete;
	ParticleEmitter(ParticleEmitter&&) noexcept;

	ParticleEmitter& operator=(const ParticleEmitter&) = delete;
	ParticleEmitter& operator=(ParticleEmitter&&) noexcept;

	void update(float deltaTime);

	void render(const ShaderProgram& shader);
private:
	glm::vec3 position;
	static uint32_t maxParticles;
	std::vector<Particle> particles;
	const float timeToCreateAnotherParticle = 0.4f;
	float timePassed = 0.0f;
};

