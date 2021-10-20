#pragma once
#include "font.h"

class Text
{
public:
	Text(int32_t x, int32_t y, std::string text, const Font& font, const glm::vec3& color = { 0.0f, 0.0f, 0.0f });

	void render(const ShaderProgram& shader);

	void setText(std::string newText);
	const std::string& getText() const;

	void setColor(const glm::vec3& newColor);
	const std::string& getColor() const;

private:

	int32_t x, y;
	Font* fontPtr;

	
};

