#pragma once
#include <string>

#include "shader.h"
#include "Framebuffer.h"
#include "../GameImport.h"
#include "../graphics/HudWriter.h"
#include GAMESTATE_H

class RenderPipeline
{
public:
	RenderPipeline(GAMESTATE& state);
	~RenderPipeline();

private:
	GAMESTATE& state;
	Shader* activeShader;
	std::string lastPass;
	GLint currentTargetFramebuffer;
	GLint currentSourceFramebuffer;
	Shader emptyShader;
	Shader debugShader;
	Shader lightShader;
	Shader celShader;
	Shader geometryPassShader;
	Shader lightMapShader;
	Shader lightMapGeometryShader;
	Shader normalMappingGeometryShader;
	HudWriter writer2D;

	int width;
	int height;
	int lightMapWidth;
	int lightMapHeight;
	Framebuffer gBuffer;

	float lightAttenuationConstant;
	float lightAttenuationLinear;
	float lightAttenuationSquared;
	glm::vec3 lightColor;
	float lightIntensity;
	float lightRadius;

public:
	void init();
	void render();

private:
	void useShader(Shader& shader);
	void reset();

	void bindTargetFramebuffer(Framebuffer& framebuffer);
	void bindSourceFramebuffer(Framebuffer& framebuffer);
	void bindDefaultFramebuffer();

	void doWireFrameMode();
	void doGeometryPass();
	void doLightPass();
	void doCelShadingPass();
	void doFinalPass();
	void doHudPass();

	void calculateRadius();
};

