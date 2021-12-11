#pragma once

namespace Light
{
	class Attenuation
	{
	public:
		Attenuation() = default;
		Attenuation(float constant, float linear, float quadratic);

		void setAttenuationByRange(float range);

	#pragma region Inline functions
		float getConstant() const
		{
			return constant;
		}
		float getLinear() const
		{
			return linear;
		}
		float getQuadratic() const
		{
			return quadratic;
		}

		void setConstant(float constant)
		{
			this->constant = constant;
		}
		void setLinear(float linear)
		{
			this->linear = linear;
		}
		void setQuadratic(float quadratic)
		{
			this->quadratic = quadratic;
		}
	#pragma endregion
	private:
		float constant = 0.10f;
		float linear = 0.00009f;
		float quadratic = 0.032f;
	};
}
