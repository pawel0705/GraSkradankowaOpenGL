#pragma once

class VAO
{
public:
	VAO()
	{
		glGenVertexArrays(1, &id);
	}
	~VAO()
	{
		glDeleteVertexArrays(1, &id);
	}

	void bind()
	{
		glBindVertexArray(id);
	}
	void unbind()
	{
		glBindVertexArray(0);
	}

private:
	GLuint id;
};

