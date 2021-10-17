#pragma once

#include <iostream>
#include <fstream>

struct BitMapFile
{
	int sizeX;
	int sizeY;
	unsigned char* data;
};

class Texture
{
public:
	Texture();
	Texture(const std::string& textureFilePath);

	void bindTexture(int textureId);
	void unbindTexture();

	int GetTextureWidth() const;
	int GetTextureHeight() const;

private:
	GLuint texture;
	BitMapFile* bmp;

	BitMapFile* readBmpImage(const std::string &filePath);

	void initializeTexture();

	~Texture();
};

