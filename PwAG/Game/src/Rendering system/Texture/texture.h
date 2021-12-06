#pragma once

enum TextureType {
	BMP = 0,
	PNG = 1,
	NORMAL_MAP = 2
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
	Texture(const Texture&) = delete;
	Texture(Texture&&) noexcept;
	virtual ~Texture();
	
	Texture& operator=(const Texture&) = delete;
	Texture& operator=(Texture&&) noexcept;

	void bindTexture(unsigned int unit) const;
	void unbindTexture() const;

	int getTextureWidth() const;
	int getTextureHeight() const;


private:
	GLuint texture;
	BitMapFile* bmp;

	TextureType textureType;

	BitMapFile* readBmpImage(const std::string &filePath);

	void initializeTexture();
};

