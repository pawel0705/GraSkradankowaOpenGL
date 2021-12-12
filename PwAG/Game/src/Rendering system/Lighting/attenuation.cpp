#include "pch.h"
#include "attenuation.h"

Light::Attenuation::Attenuation(float constant, float linear, float quadratic)
	: constant(constant), linear(linear), quadratic(quadratic)
{
}

void Light::Attenuation::setAttenuationByRange(float range)
{
	linear = 50.f * powf(range, -1.104f);
	quadratic = 1750.f * powf(range, -2.119f);
}
