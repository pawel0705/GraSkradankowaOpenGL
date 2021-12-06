#pragma once

class Particle
{
public:
	struct Lifetime
	{
		explicit Lifetime(float passed = 0);
		Lifetime(const Lifetime&);
		Lifetime(Lifetime&&) noexcept;

		Lifetime& operator=(const Lifetime&);
		Lifetime& operator=(Lifetime&&) noexcept;

		static float max; //in sec
		float passed;
	};

	Particle(const glm::vec3 position);
	Particle(const Particle&) = delete;
	Particle(Particle&&) noexcept;
	Particle& operator=(const Particle&) = delete;
	Particle& operator=(Particle&&) noexcept;

#pragma region Inline functions
	const glm::vec3& getPosition() const
	{
		return position;
	}
	const glm::vec2& getInitialSize() const
	{
		return initialSize;
	}
	const Particle::Lifetime& getLifetime() const
	{
		return lifetime;
	}
#pragma endregion

	void update(float deltaTime);
	void render(const ShaderProgram& shader);
private:
	glm::vec3 position;
	static glm::vec2 initialSize;

	Lifetime lifetime;

	VAO vao;
	VBO vbo;
};

