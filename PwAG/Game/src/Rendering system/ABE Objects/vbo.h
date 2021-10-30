#pragma once
class VBO
{
public:
	VBO()
	{
		glGenBuffers(1, &id);
	}
	~VBO()
	{
		glDeleteBuffers(1, &id);
	}

	void bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, id);
	}
	void unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void bufferData(const void* vertices, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW)
	{
		glBufferData(GL_ARRAY_BUFFER, size, vertices, usage);
	}
	void bufferSubData(const void* vertices, GLsizeiptr size)
	{
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
	}
	void setAttributesPointers(GLuint index, GLint size, GLenum type, GLsizei stride, const void* offset)
	{
		glVertexAttribPointer(index, size, type, GL_FALSE, stride, offset);
		glEnableVertexAttribArray(index);
	}

private:
	GLuint id;
};

