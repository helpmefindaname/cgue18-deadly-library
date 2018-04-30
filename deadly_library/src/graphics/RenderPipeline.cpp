#include "RenderPipeline.h"
#include "../config.h"


RenderPipeline::RenderPipeline(GAMESTATE& state, int width, int height)
	:emptyShader("assets/shader/empty"),
	state(state),
	geometryPassShader("assets/shader/geometrypass"),
	width(Config::getInt("WindowWidth")),
	height(Config::getInt("WindowHeight")),
	gBuffer(
		true, width, height,
		{ "color", "position", "normal", "material", "light", "final" },
		{ width, width, width, width, width, width },
		{ height, height, height, height, height, height },
		{ GL_RGBA16F, GL_RGBA16F, GL_RGBA16F, GL_RGBA8, GL_RGBA16F, GL_RGBA8 }
	)
{
}


RenderPipeline::~RenderPipeline()
{
}

void RenderPipeline::render() {

	this->reset();

	this->doGeometryPass();

	//this->doLightPass();

	this->doFinalPass();

	//this->doHudPass();
}

void RenderPipeline::useShader(Shader& shader) {
	shader.use();
	this->activeShader = &shader;
}

void RenderPipeline::reset() {
	bindDefaultFramebuffer();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
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

	glDepthMask(GL_TRUE);
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	this->gBuffer.bindTargetColorBuffers({ "color", "position", "normal", "material" });

	this->state.getUsedCamera().uploadData(*this->activeShader);

	this->state.render(*this->activeShader);

	glDepthMask(GL_FALSE);

	lastPass = "color";
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
