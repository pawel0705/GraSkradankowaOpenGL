#pragma once

class FBO;

class DepthMap
{
	friend class FBO;
public:
	explicit DepthMap(uint32_t width = 1024, uint32_t height = 1024);

#pragma region Inline functions
	uint32_t getWidth() const
	{
		return width;
	}
	uint32_t getHeight() const
	{
		return height;
	}
#pragma endregion
private:
	GLuint id {};
	uint32_t width, height;
};

