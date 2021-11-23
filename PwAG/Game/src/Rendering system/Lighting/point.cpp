#include "pch.h"
#include "point.h"

Light::Point::Point(const glm::vec3& position, const glm::vec3& color)
	: position(position), color(color)
{
	setColor(color);
}
