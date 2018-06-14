#pragma once

#include "texture.h"
#include "shader.h"
#include "model/Mesh.h"
#include <memory>

class HudWriter
{
public:
	HudWriter();
	~HudWriter();

private:
	Texture font;
	Shader fontShader;
	std::unique_ptr<Mesh> textureMesh;
	unsigned int hudVertexBufferID;
	unsigned int hudUVBufferID;
	unsigned int vaoId;

public:
	void print(const char * text, int x, int y, int size);
	void drawtexture(int x, int y, int width, int heigt, Texture& texture);

private:
};

