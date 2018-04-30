#pragma once

#include "texture.h"
#include "shader.h"

class Text2dWriter
{
public:
	Text2dWriter();
	~Text2dWriter();

private:
	Texture font;
	Shader fontShader;
	unsigned int Text2DVertexBufferID;
	unsigned int Text2DUVBufferID;
	unsigned int vaoId;

public:
	void print(const char * text, int x, int y, int size);

private:
};

