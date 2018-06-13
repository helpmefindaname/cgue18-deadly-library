#include "texture.h"

#include <stdexcept>
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stbimage/stb_image.h"

Texture::Texture(unsigned int width, unsigned int height, GLenum format, GLint internalFormat, GLenum precision, GLenum attachment, GLint filter, GLint wrap) :
	handle(0),
	width(width),
	height(height),
	format(format),
	internalFormat(internalFormat),
	precision(precision),
	attachment(attachment),
	filter(filter),
	wrap(wrap)
{
	glGenTextures(1, &this->handle);
	glBindTexture(GL_TEXTURE_2D, this->handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap);
	glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, this->width, this->height, 0, this->format, this->precision, 0);

	if (this->filter == GL_LINEAR_MIPMAP_LINEAR) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(int handle) :
	handle(handle)
{
}

Texture::Texture(std::string filepath) :
	handle(0),
	filepath(filepath)
{
	this->readFile();
}

Texture::Texture(int width, int height, unsigned char* data)
{
	this->applyData(width, height, data);
}

Texture::Texture(Texture&& texture) :
	handle(texture.handle),
	filepath(texture.filepath),
	width(texture.width),
	height(texture.height),
	format(texture.format),
	internalFormat(texture.internalFormat),
	precision(texture.precision),
	attachment(texture.attachment),
	filter(texture.filter),
	wrap(texture.wrap)
{
	glGenTextures(1, &this->handle);
	glBindTexture(GL_TEXTURE_2D, this->handle);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap);
	glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, this->width, this->height, 0, this->format, this->precision, 0);

	if (this->filter == GL_LINEAR_MIPMAP_LINEAR) {
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

}

Texture::~Texture() {
	if (this->handle) {
		glDeleteTextures(1, &this->handle);
	}
}

void Texture::bind(int unit) {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, this->handle);
}

GLuint Texture::getHandle() {
	return this->handle;
}

GLenum Texture::getAttachment() {
	return this->attachment;
}

GLenum Texture::getInternalFormat()
{
	return this->internalFormat;
}

int Texture::getWidth()
{
	return this->width;
}

int Texture::getHeight()
{
	return this->height;
}

void Texture::applyData(int width, int height, unsigned char* data) {
	glGenTextures(1, &this->handle);
	glBindTexture(GL_TEXTURE_2D, this->handle);
	this->width = width;
	this->height = height;
	this->format = GL_RGB;
	this->internalFormat = GL_RGB;
	this->precision = GL_UNSIGNED_BYTE;
	this->wrap = GL_CLAMP_TO_EDGE;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrap);
	glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, this->width, this->height, 0, this->format, this->precision, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::readFile() {
	int imageWidth;
	int imageHeight;
	int channels;

	stbi_uc* data = stbi_load(this->filepath.c_str(), &imageWidth, &imageHeight, &channels, STBI_rgb);

	if (data) {
		applyData(imageWidth, imageHeight, data);
		stbi_image_free(data);
	}
	else {
		throw std::runtime_error("Error loading texture file " + this->filepath);
	}
}

void Texture::resetHandle() {
	this->handle = 0;
}