#include "RenderPipeline.h"
#include "../config.h"
#include "../gameObjects/Light.h"

RenderPipeline::RenderPipeline(GAMESTATE& state, int width, int height)
	:emptyShader("assets/shader/empty"),
	state(state),
	geometryPassShader("assets/shader/geometrypass"),
	stencilTestShader("assets/shader/stencilpass"),
	lightShader("assets/shader/lightpass"),
	width(Config::getInt("WindowWidth")),
	currentTargetFramebuffer(-1),
	currentSourceFramebuffer(-1),
	activeShader(nullptr),
	height(Config::getInt("WindowHeight")),
	lightAttenuationConstant(Config::getFloat("LightAttenuationConstant")),
	lightAttenuationLinear(Config::getFloat("LightAttenuationLinear")),
	lightAttenuationSquared(Config::getFloat("LightAttenuationSquared")),
	lightColor(Config::getVec3("LightColor")),
	lightIntensity(Config::getFloat("LightIntensity")),
	gBuffer(
		true, width, height,
		{ "color", "position", "normal", "material", "light", "final" },
		{ width, width, width, width, width, width },
		{ height, height, height, height, height, height },
		{ GL_RGBA16F, GL_RGBA16F, GL_RGBA16F, GL_RGBA8, GL_RGBA16F, GL_RGBA8 }
	)
{
	this->calculateRadius();
}


RenderPipeline::~RenderPipeline()
{
}

void RenderPipeline::render(bool debug) {

	this->reset();

	this->doGeometryPass();

	this->doLightPass();

	/*
	if (debug) {
		this->lastPass = "color";
	}/**/

	this->doFinalPass();

	//this->doHudPass();
}

void RenderPipeline::useShader(Shader& shader) {
	shader.use();
	this->activeShader = &shader;
}

void RenderPipeline::reset() {
	glUseProgram(0);
	this->activeShader = &this->emptyShader;
}

void RenderPipeline::bindTargetFramebuffer(Framebuffer& framebuffer) {
	if (this->currentTargetFramebuffer != framebuffer.getHandle()) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer.getHandle());
	}
}

void RenderPipeline::bindSourceFramebuffer(Framebuffer& framebuffer) {
	if (this->currentSourceFramebuffer != framebuffer.getHandle()) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer.getHandle());
	}
}

void RenderPipeline::bindDefaultFramebuffer() {
	if (this->currentTargetFramebuffer != 0) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}

	if (this->currentSourceFramebuffer != 0) {
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	}
}

void RenderPipeline::doGeometryPass()
{
	this->useShader(this->geometryPassShader);
	this->bindTargetFramebuffer(this->gBuffer);

	this->gBuffer.bindTargetColorBuffers({ "color", "position", "normal", "material" });

	glDepthMask(GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);


	this->state.getUsedCamera().uploadData(*this->activeShader);

	this->state.render(*this->activeShader);

	glDepthMask(GL_FALSE);

	lastPass = "color";
}

void RenderPipeline::doLightPass()
{
	this->bindTargetFramebuffer(this->gBuffer);
	this->gBuffer.bindTargetColorBuffers({ "light" });
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	this->useShader(this->lightShader);
	this->gBuffer.bindTextures(*this->activeShader, { "color", "position", "normal", "material" }, { "colorBuffer", "positionBuffer", "normalBuffer", "materialBuffer" });

	this->activeShader->setUniform("resolution", glm::vec2(this->width, this->height));
	this->activeShader->setUniform("brightness", Config::getFloat("Brightness"));

	std::vector<std::shared_ptr<Light>> lights = state.getLights();

	this->activeShader->setUniform("attenuationConstant", this->lightAttenuationConstant);
	this->activeShader->setUniform("attenuationLinear", this->lightAttenuationLinear);
	this->activeShader->setUniform("attenuationSquared", this->lightAttenuationSquared);
	this->activeShader->setUniform("lightColor", this->lightColor);
	this->activeShader->setUniform("lightIntensity", this->lightIntensity);
	this->activeShader->setUniform("lightRadius", this->lightRadius);

	std::vector<glm::vec3> positions;

	for (auto light : lights) {
		positions.push_back(light->getPosition());
	}

	this->activeShader->setUniform("lightPositions", positions);
	this->activeShader->setUniform("lightCount", (int)positions.size());

	this->gBuffer.renderQuad(*this->activeShader);

	lastPass = "light";
}

void RenderPipeline::doFinalPass()
{
	this->bindDefaultFramebuffer();
	this->bindSourceFramebuffer(this->gBuffer);
	this->gBuffer.bindSourceColorBuffer(lastPass);

	glBlitFramebuffer(
		0, 0, this->width, this->height,
		0, 0, this->width, this->height,
		GL_COLOR_BUFFER_BIT, GL_NEAREST
	);
}

void RenderPipeline::calculateRadius()
{
	float maxChannel = std::fmaxf(std::fmaxf(this->lightColor.r, this->lightColor.g), this->lightColor.b);
	const float threshold = 256.0f / 1.0f;

	float a = this->lightAttenuationSquared;
	float b = this->lightAttenuationLinear;
	float c = this->lightAttenuationConstant - threshold * maxChannel * this->lightIntensity;

	float d = b * b - 4.0f*a*c;

	this->lightRadius = (-b + std::sqrt(d)) / (2.0f*a);
}
