#pragma once

#include "../libimport/glew.h"
#include <string>

class Texture {
public:
	Texture(unsigned int width, unsigned int height, GLenum format, GLint internalFormat, GLenum precision, GLenum attachment, GLint filter, GLint wrap);
	Texture(int handle);
	Texture(std::string filepath);
	Texture(int width, int height, unsigned char* data);
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
	GLenum getInternalFormat();
	int getWidth();
	int getHeight();
	void resetHandle();

private:
	void readFile();
	void applyData(int width, int height, unsigned char* data);
};