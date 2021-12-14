#include "pch.h"
#include "point.h"
#include "../../Game Objects/maze.h"

Light::Point::Point(const glm::vec3& position, const glm::vec3& color)
	: Light(color), position(position), depthMap(Texture::createDepthTexture())
{
	setColor(color);
	attenuation.setAttenuationByRange(range);

	fbo.bind();
	fbo.attachTexture(this->depthMap, FBO::Type::eDepth);
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

void Light::Point::renderDepthMap(const ShaderProgram& shader, const Maze& maze) const
{
	shader.setMat4("lightSpaceMatrix", lightSpaceMatrix[0]);

	glViewport(0, 0, depthMap.getTextureWidth(), depthMap.getTextureHeight());
	fbo.bind();
	glClear(GL_DEPTH_BUFFER_BIT);


	fbo.unbind();
}
