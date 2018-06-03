#pragma once

#include "texture.h"
#include "shader.h"

class HudWriter
{
public:
	HudWriter();
	~HudWriter();

private:
	Texture font;
	Shader fontShader;
	unsigned int hudVertexBufferID;
	unsigned int hudUVBufferID;
	unsigned int vaoId;

public:
	void print(const char * text, int x, int y, int size);
	void drawtexture(int x, int y, int width, int heigt, Texture& texture);

private:
};

