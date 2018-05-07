#pragma once
#include <string>

#include "shader.h"
#include "Framebuffer.h"
#include "../GameImport.h"
#include GAMESTATE_H

class RenderPipeline
{
public:
	RenderPipeline(GAMESTATE& state, int width, int height);
	~RenderPipeline();

private:
	GAMESTATE& state;
	Shader* activeShader;
	std::string lastPass;
	GLint currentTargetFramebuffer;
	GLint currentSourceFramebuffer;
	Shader emptyShader;
	Shader stencilTestShader;
	Shader ambientLightShader;
	Shader lightShader;
	Shader geometryPassShader;

	Framebuffer gBuffer;
	int width;
	int height;

public:
	void render(bool debug);

private:
	void useShader(Shader& shader);
	void reset();

	void bindTargetFramebuffer(Framebuffer& framebuffer);
	void bindSourceFramebuffer(Framebuffer& framebuffer);
	void bindDefaultFramebuffer();


	void doGeometryPass();
	void doLightPass();
	void doFinalPass();
	void doHudPass();
};

