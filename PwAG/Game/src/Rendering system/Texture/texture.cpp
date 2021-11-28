#include "pch.h"
#include "texture.h"
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include "../SourceDep/stb_image.h"

Texture::Texture() {

}

Texture::Texture(const std::string& textureFilePath, TextureType textureType)
{
	int x;
	int y;
	int nrChannels;
	unsigned char* data = stbi_load(textureFilePath.c_str(), &x, &y, &nrChannels, 0);

	this->bmp = new BitMapFile();

	this->bmp->data = data;
	this->bmp->sizeX = x;
	this->bmp->sizeY = y;
	this->bmp->nrChannels = nrChannels;

	this->textureType = textureType;

	if (this->bmp == nullptr) {
		std::cout << "Failed loading texture: " + textureFilePath << std::endl;
	}

	this->initializeTexture();
}

void Texture::initializeTexture() {
	glGenTextures(1, &this->texture);
	glBindTexture(GL_TEXTURE_2D, this->texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (this->textureType == TextureType::NORMAL_MAP) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->bmp->sizeX, this->bmp->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->bmp->data);
	}
	else if (this->textureType == TextureType::BMP)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->bmp->sizeX, this->bmp->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, this->bmp->data);
	}
	else if (this->textureType == TextureType::PNG) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->bmp->sizeX, this->bmp->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->bmp->data);
	}
	else 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->bmp->sizeX, this->bmp->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, this->bmp->data);
	}

	glGenerateMipmap(GL_TEXTURE_2D);
}

// Routine to read a bitmap file. 
// Works only for uncompressed bmp files of 24-bit color. !!!
BitMapFile* Texture::readBmpImage(const std::string& filePath)
{
	BitMapFile* bmp = new BitMapFile;
	unsigned int size, offset, headerSize;

	// Read input file name.
	std::ifstream infile(filePath.c_str(), std::ios::binary);

	// Get the starting point of the image data.
	infile.seekg(10);
	infile.read((char*)&offset, 4);

	// Get the header size of the bitmap.
	infile.read((char*)&headerSize, 4);

	// Get width and height values in the bitmap header.
	infile.seekg(18);
	infile.read((char*)&bmp->sizeX, 4);
	infile.read((char*)&bmp->sizeY, 4);

	// Allocate buffer for the image.
	size = bmp->sizeX * bmp->sizeY * 24;
	bmp->data = new unsigned char[size];

	// Read bitmap data.
	infile.seekg(offset);
	infile.read((char*)bmp->data, size);

	// Reverse color from bgr to rgb.
	int temp;
	for (int i = 0; i < size; i += 3)
	{
		temp = bmp->data[i];
		bmp->data[i] = bmp->data[i + 2];
		bmp->data[i + 2] = temp;
	}

	return bmp;
}

void Texture::bindTexture(unsigned int unit) {
	assert(unit >= 0 && unit <= 31);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, this->texture);
}

void Texture::unbindTexture() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::getTextureWidth() const {
	return this->bmp->sizeX;
}

int Texture::getTextureHeight() const {
	return this->bmp->sizeY;
}

Texture::~Texture() {
	stbi_image_free(this->bmp->data);

	delete this->bmp;

	glDeleteTextures(1, &this->texture);
}