#include "pch.h"
#include "particleEmitter.h"

uint32_t ParticleEmitter::maxParticles = 100;

ParticleEmitter::ParticleEmitter(const glm::vec3& position)
	: position(position)
{
}

ParticleEmitter::ParticleEmitter(ParticleEmitter&& other) noexcept
	: position(other.position), particles(std::move(other.particles)), timePassed(other.timePassed)
{
}

ParticleEmitter& ParticleEmitter::operator=(ParticleEmitter&& other) noexcept
{
	if(this != &other)
	{
		position = other.position;
		particles = std::move(other.particles);
		timePassed = other.timePassed;
	}

	return *this;
}

void ParticleEmitter::update(float deltaTime)
{
	for(auto iter = particles.begin(); iter != particles.end(); iter++)
	{
		auto& particle = *iter;
		auto& lifetime = particle.getLifetime();
		if(lifetime.passed >= lifetime.max)
		{
			iter = particles.erase(iter);
		}
		else
		{
			particle.update(deltaTime);
		}
	}

	uint32_t particlesToCreateCount = 0;
	timePassed += deltaTime;

	while(timePassed >= timeToCreateAnotherParticle)
	{
		timePassed -= timeToCreateAnotherParticle;

		if(particles.size() < maxParticles)
		{
			glm::vec3 posDiff = { (rand() % 10 - 5) / 50.0, 0.0f , (rand() % 10 - 5) / 50.0 };
			particles.push_back(Particle(this->position + posDiff));
		}
	}
}

void ParticleEmitter::render(const ShaderProgram& shader)
{
	const Texture& texture = ResourceManager::getInstance().getTexture("fire");
	texture.bindTexture(0);

	for(auto& particle : particles)
	{
		particle.render(shader);
	}

	texture.unbindTexture();
}
