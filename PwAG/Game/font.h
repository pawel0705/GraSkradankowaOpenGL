#pragma once

class Font
{
public:
	Font(const std::string& fontFilePath, uint32_t size);

private:
	struct Character
	{
		glm::ivec2 advance;
		glm::ivec2 size;
		glm::ivec2 bearing;
		glm::ivec2 atlasOffset;
	};
	std::unordered_map<char, Character> characters;
};

