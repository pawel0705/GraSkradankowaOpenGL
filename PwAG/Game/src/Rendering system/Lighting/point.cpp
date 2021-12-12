#include "pch.h"
#include "point.h"

Light::Point::Point(const glm::vec3& position, const glm::vec3& color)
	: Light(color), position(position)
{
	setColor(color);
	attenuation.setAttenuationByRange(range);

	fbo.bind();
	fbo.attachDepthMap(this->depthMap);
	fbo.drawBufferNone();
	fbo.readBufferNone();
	fbo.unbind();

	glm::mat4 lightProjection =	glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
		
	lightSpaceMatrix =
	{
		lightProjection * glm::lookAt(position, position + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
		lightProjection * glm::lookAt(position, position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
		lightProjection * glm::lookAt(position, position + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
		lightProjection * glm::lookAt(position, position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
		lightProjection * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
		lightProjection * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f))
	};
}

void Light::Point::renderDepthMap() const
{
	glViewport(0, 0, depthMap.getWidth(), depthMap.getHeight());
	
	fbo.bind();
	glClear(GL_DEPTH_BUFFER_BIT);

	fbo.unbind();
	glViewport(0, 0, Config::g_defaultWidth, Config::g_defaultHeight);
}
