#pragma once

class EBO
{
public:
	EBO()
	{
		glGenBuffers(1, &id);
	}
	~EBO()
	{
		glDeleteBuffers(1, &id);
	}

	void bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	}
	void unbind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void bufferData(GLuint indices[], GLsizei size, GLenum usage = GL_STATIC_DRAW)
	{
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, usage);
	}
	void drawElements(GLenum mode, int size, GLenum type, const void* offset = nullptr)
	{
		glDrawElements(mode, size, type, offset);
	}

private:
	GLuint id;
};

