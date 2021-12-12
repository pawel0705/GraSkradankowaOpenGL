#pragma once
#include "../Texture/depthMap.h"

class FBO
{
public:
	enum class Type
	{
		eColor,
		eDepth,
		eStencil,
		eDepthStencil
	};

	FBO()
	{
		glGenFramebuffers(1, &id);
	}

	FBO(const FBO&) = delete;
	FBO(FBO&& other) noexcept
	{
		id = other.id;
		other.id = 0;
	}

	~FBO()
	{
		if(id != 0)
		{
			glDeleteFramebuffers(1, &id);
			id = 0;
		}
	}

	FBO& operator=(const FBO&) = delete;
	FBO& operator=(FBO&& other) noexcept
	{
		if(this != &other)
		{
			id = other.id;
			other.id = 0;
		}
	}

	bool checkStatus() const
	{
		return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
	}

	void bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, id);
	}
	void unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void drawBufferNone() const
	{
		glDrawBuffer(GL_NONE);
	}

	void readBufferNone() const
	{
		glReadBuffer(GL_NONE);
	}

	void attachDepthMap(const DepthMap& texture) const
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture.id, 0);
	}
private:
	GLuint id {};
};

