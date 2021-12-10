#include "pch.h"
#include "particleEmitter.h"

uint32_t ParticleEmitter::maxParticles = 100;

ParticleEmitter::ParticleEmitter(const glm::vec3& position, const glm::vec3& startVelocity, const glm::vec3& startAcceleration, const Texture& texture, const glm::vec2& scale)
	: position(position), particleStartVelocity(startVelocity), particleStartAcceleration(startAcceleration), scale(scale), texture(&texture)
{
}

ParticleEmitter::ParticleEmitter(ParticleEmitter&& other) noexcept
	: position(other.position), particleStartVelocity(other.particleStartVelocity), particleStartAcceleration(other.particleStartAcceleration), scale(other.scale), particles(std::move(other.particles)), timePassed(other.timePassed), texture(other.texture)
{
}

ParticleEmitter& ParticleEmitter::operator=(ParticleEmitter&& other) noexcept
{
	if(this != &other)
	{
		position = other.position;
		particleStartVelocity = other.particleStartVelocity;
		particleStartAcceleration = other.particleStartAcceleration;

		scale = other.scale;
		particles = std::move(other.particles);
		timePassed = other.timePassed;
		texture = other.texture;
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
			glm::vec3 posDiff = 
			{ 
				(rand() % 10 - 5) * 0.02,
				0.0f ,
				(rand() % 10 - 5) * 0.02 
			};
			particles.emplace_back(this->position + posDiff, particleStartVelocity, particleStartAcceleration);
			particles.back().setAccelerationUpdateFunction(accelerationUpdateFunction);
		}
	}
}

void ParticleEmitter::render(const ShaderProgram& shader)
{
	texture->bindTexture(0);
	
	shader.setVec2f("scale", scale);

	for(auto& particle : particles)
	{
		particle.render(shader);
	}

	texture->unbindTexture();
}
