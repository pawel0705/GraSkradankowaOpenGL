#pragma once

enum TextureType {
	BMP = 0,
	PNG = 1,
};

struct BitMapFile
{
	int sizeX;
	int sizeY;
	int nrChannels;
	unsigned char* data;
};

class Texture
{
public:
	Texture();
	Texture(const std::string& textureFilePath, TextureType textureType);

	void bindTexture(unsigned int unit);
	void unbindTexture();

	int getTextureWidth() const;
	int getTextureHeight() const;

	virtual ~Texture();

private:
	GLuint texture;
	BitMapFile* bmp;

	TextureType textureType;

	BitMapFile* readBmpImage(const std::string &filePath);

	void initializeTexture();
};

