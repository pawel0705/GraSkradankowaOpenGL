#pragma once
#include "light.h"
#include "attenuation.h"

namespace Light
{
	class Point 
		: public Light
	{
	public:
		Point(const glm::vec3& position, const glm::vec3& color);
		
	#pragma region Inline functions
	#pragma region Getters
		const glm::vec3& getPosition() const
		{
			return position;
		}
		const Attenuation& getAttenuation() const
		{
			return attenuation;
		}
		float getRange() const
		{
			return range;
		}
	#pragma endregion
	#pragma region Setters
		void setPosition(const glm::vec3& position)
		{
			this->position = position;
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

			attenuation.setAttenuationByRange(range);
		}
	#pragma endregion
	#pragma endregion

		void renderDepthMap() const;

	private:
		glm::vec3 position;

		Attenuation attenuation;
		float range = 50;

		FBO fbo;
		DepthMap depthMap;
		std::array<glm::mat4, 6> lightSpaceMatrix;
	};
}
