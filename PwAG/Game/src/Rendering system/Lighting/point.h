#pragma once

namespace Light
{
	class Point
	{
	public:
		struct Attenuation
		{
			float constant = 1.0f;
			float linear = 0.09f;
			float quadratic = 0.032f;
		};

		Point(const glm::vec3& position, const glm::vec3& color);
		
	#pragma region Inline functions
		const glm::vec3& getPosition() const
		{
			return position;
		}
		const glm::vec3& getColor() const
		{
			return color;
		}
		const glm::vec3& getAmbient()
		{
			return ambient;
		}
		const glm::vec3& getSpecular()
		{
			return specular;
		}
		const glm::vec3& getDiffuse()
		{
			return diffuse;
		}
		const Attenuation& getAttenuation() const
		{
			return attenuation;
		}
		float getRange() const
		{
			return range;
		}

		void setPosition(const glm::vec3& position)
		{
			this->position = position;
		}
		void setColor(const glm::vec3& color)
		{
			this->color = color;

			this->ambient = color * 0.05f;
			this->specular = color * 1.0f;
			this->diffuse = color * 0.8f;
		}
		void setAttenuation(const Attenuation& attenuation)
		{
			this->attenuation = attenuation;
		}
		void setRange(float range)
		{
			this->range = range;
		}

		void setAttenuationByRange(float range)
		{
			this->range = range;

			attenuation.linear = 4.5452f * powf(range, -1.004f);
			attenuation.quadratic = 82.445f * powf(range, -2.019f);
		}
		#pragma endregion
	private:
		glm::vec3 position;
		
		glm::vec3 color;
		glm::vec3 ambient;
		glm::vec3 specular;
		glm::vec3 diffuse;

		Attenuation attenuation;
		float range = 50;
	};
}
