#include "pch.h"
#include "point.h"

Light::Point::Point(const glm::vec3& position, const glm::vec3& color)
	: Light(color), position(position)
{
	setColor(color);
}
