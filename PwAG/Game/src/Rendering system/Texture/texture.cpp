#include "pch.h"
#include "texture.h"
#include <cassert>

#define STB_IMAGE_IMPLEMENTATION
#include "../SourceDep/stb_image.h"

#pragma region BitMapFile
Texture::BitMapFile::BitMapFile() = default;

Texture::BitMapFile::BitMapFile(const std::string & filePath)
{
	this->loadFromFile(filePath);
}

Texture::BitMapFile::BitMapFile(BitMapFile&& other) noexcept
	: sizeX(other.sizeX), sizeY(other.sizeY), nrChannels(other.nrChannels)
{
	data = other.data;
	other.data = nullptr;
}

Texture::BitMapFile::~BitMapFile()
{
	if(data)
	{
		stbi_image_free(data);
	}
}

Texture::BitMapFile& Texture::BitMapFile::operator=(BitMapFile&& other) noexcept
{
	if(this != &other)
	{
		sizeX = other.sizeX;
		sizeY = other.sizeY;

		nrChannels = other.nrChannels;

		data = other.data;
		other.data = nullptr;
	}

	return *this;
}

void Texture::BitMapFile::loadFromFile(const std::string & filePath)
{
	data = stbi_load(filePath.c_str(), &sizeX, &sizeY, &nrChannels, 0);
	if(data == nullptr)
	{
		std::cout << "Failed loading texture: " + filePath << std::endl;
	}
}
#pragma endregion

Texture::Texture(Texture::Type type)
	: textureType(type)
{
	glGenTextures(1, &this->texture);
}

Texture Texture::createTextureFromFile(const std::string & textureFilePath, Texture::Type textureType)
{
	Texture toReturn { textureType };
	toReturn.bmp.loadFromFile(textureFilePath);
	toReturn.initializeTexture();

	return toReturn;
}

Texture Texture::createDepthTexture()
{
	Texture toReturn { Texture::Type::DEPTH };
	toReturn.initializeTexture();

	return toReturn;
}

Texture::Texture(Texture && other) noexcept
	: textureType(other.textureType), bmp(std::move(other.bmp))
{
	texture = other.texture;
	other.texture = 0;
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->texture);
}

Texture& Texture::operator=(Texture && other) noexcept
{
	if(this != &other)
	{
		texture = other.texture;
		other.texture = 0;

		bmp = std::move(other.bmp);

		textureType = other.textureType;
	}

	return *this;
}

void Texture::initializeTexture()
{

	glBindTexture(GL_TEXTURE_2D, this->texture);

	if(this->textureType == Texture::Type::DEPTH)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->bmp.sizeX, this->bmp.sizeY, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		if(this->textureType == Texture::Type::NORMAL_MAP)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->bmp.sizeX, this->bmp.sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->bmp.data);
		}
		else if(this->textureType == Texture::Type::BMP)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->bmp.sizeX, this->bmp.sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, this->bmp.data);
		}
		else if(this->textureType == Texture::Type::PNG)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->bmp.sizeX, this->bmp.sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->bmp.data);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->bmp.sizeX, this->bmp.sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, this->bmp.data);
		}

		glGenerateMipmap(GL_TEXTURE_2D);
	}
}

// Routine to read a bitmap file. 
// Works only for uncompressed bmp files of 24-bit color. !!!
Texture::BitMapFile* Texture::readBmpImage(const std::string& filePath)
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
	bmp->data = new unsigned char[size]; // <- czy to potencjalny wyciek pamiêci?

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

void Texture::bindTexture(unsigned int unit) const
{
	assert(unit >= 0 && unit <= 31);

	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, this->texture);
}

void Texture::unbindTexture() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::getTextureWidth() const
{
	return this->bmp.sizeX;
}

int Texture::getTextureHeight() const
{
	return this->bmp.sizeY;
}
