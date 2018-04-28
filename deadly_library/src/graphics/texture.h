#pragma once

#include "../libimport/glew.h"
#include <string>

class Texture {
public:
	Texture(unsigned int width, unsigned int height, GLenum format, GLint internalFormat, GLenum precision, GLenum attachment, GLint filter, GLint wrap);
	Texture(std::string filepath);
	Texture(Texture&& texture);
	~Texture();

private:
	GLuint handle;
	std::string filepath;

	unsigned int width;
	unsigned int height;

	GLenum format;
	GLint internalFormat;
	GLenum precision;
	GLenum attachment;
	GLint filter;
	GLint wrap;

public:
	GLuint getHandle();
	void bind(int id);
	GLenum getAttachment();

private:
	void readFile();
};