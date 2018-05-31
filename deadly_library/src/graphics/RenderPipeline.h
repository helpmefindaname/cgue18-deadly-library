#pragma once
#include <string>

#include "shader.h"
#include "Framebuffer.h"
#include "../GameImport.h"
#include "../graphics/Text2dWriter.h"
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
	Shader lightShader;
	Shader geometryPassShader;
	Text2dWriter writer2D;

	Framebuffer gBuffer;
	int width;
	int height;

	float lightAttenuationConstant;
	float lightAttenuationLinear;
	float lightAttenuationSquared;
	glm::vec3 lightColor;
	float lightIntensity;
	float lightRadius;

public:
	void render();

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

	void calculateRadius();
};

