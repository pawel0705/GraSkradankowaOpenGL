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

	void bindTexture(unsigned int unit);
	void unbindTexture();

	int getTextureWidth() const;
	int getTextureHeight() const;

	virtual ~Texture();

private:
	GLuint texture;
	BitMapFile* bmp;

	BitMapFile* readBmpImage(const std::string &filePath);

	void initializeTexture();
};

