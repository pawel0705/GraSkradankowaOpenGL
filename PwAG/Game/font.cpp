#include "pch.h"
#include "font.h"

Font::Font(const std::string& fontFilePath, uint32_t size)
{
	FT_Library freetype;
	if (FT_Init_FreeType(&freetype))
	{
		std::cout << "Freetype initialization failed!" << std::endl;

		return;
	}

	FT_Face face;
	if (FT_New_Face(freetype, fontFilePath.c_str(), 0, &face))
	{
		std::cout << "Face creation failed! Missing font file?" << std::endl;
	}

	FT_Set_Pixel_Sizes(face, 0, size);

	FT_GlyphSlot glyphSlot = face->glyph;
	uint32_t atlasWidth = 0;
	uint32_t atlasHeight = 0;

	for (uint8_t i = 0; i < 128; ++i)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			continue;
		}

		atlasWidth += glyphSlot->bitmap.width;
		atlasHeight = std::max(atlasHeight, glyphSlot->bitmap.rows);
	}

	GLuint texture;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_ALPHA, GL_UNSIGNED_BYTE, nullptr);

	uint32_t xOffset = 0;
	uint32_t yOffset = 0;
	for (uint8_t i = 0; i < 128; ++i)
	{
		if (FT_Load_Char(face, i, FT_LOAD_RENDER))
		{
			continue;
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, glyphSlot->bitmap.width, glyphSlot->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, glyphSlot->bitmap.buffer);
		xOffset += glyphSlot->bitmap.width;
		yOffset += 0; //for now, atlas is just one row

		Character c;
		c.advance = {glyphSlot->advance.x >> 6, glyphSlot->advance.y >> 6};
		c.size = {glyphSlot->bitmap.width, glyphSlot->bitmap.rows};
		c.bearing = {glyphSlot->bitmap_left, glyphSlot->bitmap_top};
		c.atlasOffset = {xOffset / atlasWidth, yOffset / atlasHeight};

		characters.insert(std::make_pair(i, c));
	}
}
