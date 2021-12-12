#include "pch.h"
#include "particleEmitter.h"

uint32_t ParticleEmitter::maxParticles = 100;

ParticleEmitter::ParticleEmitter(const glm::vec3& position, const glm::vec3& startVelocity, const glm::vec3& startAcceleration, const Texture& texture, const glm::vec2& scale)
	: position(position), particleStartVelocity(startVelocity), particleStartAcceleration(startAcceleration), scale(scale), texture(&texture)
{
}

ParticleEmitter::ParticleEmitter(ParticleEmitter&& other) noexcept
	: position(other.position), particleStartVelocity(other.particleStartVelocity), particleStartAcceleration(other.particleStartAcceleration), accelerationUpdateFunction(std::move(other.accelerationUpdateFunction)),
	scale(other.scale), particles(std::move(other.particles)), timePassed(other.timePassed), texture(other.texture)
{
}

ParticleEmitter& ParticleEmitter::operator=(ParticleEmitter&& other) noexcept
{
	if(this != &other)
	{
		position = other.position;
		particleStartVelocity = other.particleStartVelocity;
		particleStartAcceleration = other.particleStartAcceleration;
		accelerationUpdateFunction = std::move(other.accelerationUpdateFunction);

		scale = other.scale;
		particles = std::move(other.particles);
		timePassed = other.timePassed;
		texture = other.texture;
	}

	return *this;
}

void ParticleEmitter::update(float deltaTime)
{
	if(!particles.empty())
	{
		particles.erase(std::remove_if(
			particles.begin(),
			particles.end(),
			[](const Particle& particle) -> bool
			{
				return particle.getLifetime().passed >= particle.getLifetime().max;
			}),
			particles.end()
		);

		for(auto iter = particles.begin(); iter != particles.end(); iter++)
		{
			iter->update(deltaTime);
		}
	}
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

void ParticleEmitter::render(const ShaderProgram& shader, const glm::vec3& cameraPosition)
{
	texture->bindTexture(0);
	shader.setVec2f("scale", scale);

	std::sort(particles.begin(), particles.end(), 
			  [&cameraPosition](const Particle& left, const Particle& right) -> bool
			  {
				  return glm::distance2(cameraPosition, left.getPosition()) > glm::distance2(cameraPosition, right.getPosition());
			  });

	for(auto& particle : particles)
	{
		particle.render(shader);
	}

	texture->unbindTexture();
}
