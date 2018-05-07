#include "RenderPipeline.h"
#include "../config.h"
#include "../gameObjects/Light.h"

RenderPipeline::RenderPipeline(GAMESTATE& state, int width, int height)
	:emptyShader("assets/shader/empty"),
	state(state),
	geometryPassShader("assets/shader/geometrypass"),
	stencilTestShader("assets/shader/stencilpass"),
	lightShader("assets/shader/lightpass"),
	ambientLightShader("assets/shader/ambient"),
	width(Config::getInt("WindowWidth")),
	currentTargetFramebuffer(-1),
	currentSourceFramebuffer(-1),
	activeShader(nullptr),
	height(Config::getInt("WindowHeight")),
	gBuffer(
		true, width, height,
		{ "color", "position", "normal", "material", "light", "final" },
		{ width, width, width, width, width, width },
		{ height, height, height, height, height, height },
		{ GL_RGBA16F, GL_RGBA16F, GL_RGBA16F, GL_RGBA8, GL_RGBA16F, GL_RGBA8 }
	)
{ }


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

	this->useShader(this->ambientLightShader);
	this->gBuffer.bindTextures(*this->activeShader, { "color", "material" }, { "colorBuffer", "materialBuffer" });

	this->activeShader->setUniform("resolution", glm::vec2(this->width, this->height));
	this->activeShader->setUniform("brightness", Config::getFloat("Brightness"));

	this->gBuffer.renderQuad(*this->activeShader);

	std::vector<std::shared_ptr<Light>> lights = state.getLights();

	for (auto light : lights) {
		this->useShader(this->stencilTestShader);
		this->bindTargetFramebuffer(gBuffer);
		this->bindSourceFramebuffer(gBuffer);
		glDrawBuffer(GL_NONE);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		glDisable(GL_CULL_FACE);
		glClear(GL_STENCIL_BUFFER_BIT);

		glStencilFunc(GL_ALWAYS, 0, 0);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

		this->state.getUsedCamera().uploadData(*this->activeShader);
		light->render(*this->activeShader);

		// ------------------------------

		this->useShader(this->lightShader);

		this->gBuffer.bindTargetColorBuffers({ "light" });
		this->gBuffer.bindTextures(*this->activeShader, { "color", "position", "normal", "material" }, { "colorBuffer", "positionBuffer", "normalBuffer", "materialBuffer" });

		this->activeShader->setUniform("resolution", glm::vec2(this->width, this->height));

		glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
		glDisable(GL_DEPTH_TEST);

		glEnable(GL_BLEND);
		glBlendEquation(GL_FUNC_ADD);
		glBlendFunc(GL_ONE, GL_ONE);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		this->state.getUsedCamera().uploadData(*this->activeShader);
		light->render(*this->activeShader);

		glCullFace(GL_BACK);
		glDisable(GL_BLEND);
	}

	glDisable(GL_STENCIL_TEST);


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
