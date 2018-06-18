#include "RenderPipeline.h"
#include "../config.h"
#include "../gameObjects/Light.h"
#include "../Globals.h"

RenderPipeline::RenderPipeline(GAMESTATE& state)
	:emptyShader("assets/shader/empty"),
	state(state),
	debugShader("assets/shader/debugpass"),
	geometryPassShader("assets/shader/geometrypass"),
	lightMapGeometryShader("assets/shader/lightMapGeometryPass"),
	lightShader("assets/shader/lightpass"),
	lightMapShader("assets/shader/lightMapPass"),
	normalMappingGeometryShader("assets/shader/normalMappingGeometryPass"),
	currentTargetFramebuffer(-1),
	currentSourceFramebuffer(-1),
	activeShader(nullptr),
	width(Config::getInt("WindowWidth")),
	height(Config::getInt("WindowHeight")),
	lightMapWidth(Config::getInt("LightMapWidth")),
	lightMapHeight(Config::getInt("LightMapHeight")),
	lightAttenuationConstant(Config::getFloat("LightAttenuationConstant")),
	lightAttenuationLinear(Config::getFloat("LightAttenuationLinear")),
	lightAttenuationSquared(Config::getFloat("LightAttenuationSquared")),
	lightColor(Config::getVec3("LightColor")),
	lightIntensity(Config::getFloat("LightIntensity")),
	gBuffer(
		true, width, height,
		{ "color", "position", "normal", "material", "light", "final", "lightMap" },
		{ width, width, width, width, width, width, lightMapWidth },
		{ height, height, height, height, height, height, lightMapHeight },
		{ GL_RGB8, GL_RGB16F, GL_RGB16F, GL_RGBA8, GL_RGB8, GL_RGB8,GL_RGB8 }
	),
	writer2D()
{
	this->calculateRadius();
}


RenderPipeline::~RenderPipeline()
{
}

void RenderPipeline::init()
{
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_FALSE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	this->useShader(this->lightMapShader);
	this->bindSourceFramebuffer(this->gBuffer);
	this->bindTargetFramebuffer(this->gBuffer);
	this->gBuffer.bindTargetColorBuffers({ "lightMap" });

	std::vector<std::shared_ptr<Light>> lights = state.getLights();

	this->activeShader->setUniform("brightness", Config::getFloat("Brightness"));
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

	this->state.generateLightMaps(*this->activeShader, this->gBuffer);

	this->bindDefaultFramebuffer();
}

void RenderPipeline::render() {

	this->reset();

	if (Globals::isWireFrameMode) {
		this->doWireFrameMode();
	}
	else 
	{
		this->doGeometryPass();
		this->doLightPass();
		this->doFinalPass();
	}
	this->doHudPass();
}

void RenderPipeline::useShader(Shader& shader) {
	shader.use();
	this->activeShader = &shader;
}

void RenderPipeline::reset() {
	glUseProgram(0);
	this->activeShader = &this->emptyShader;
	lastPass = "color";
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	bindDefaultFramebuffer();
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

void RenderPipeline::doWireFrameMode() 
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	this->useShader(this->debugShader);
	this->state.getUsedCamera().uploadData(*this->activeShader);
	this->state.render(*this->activeShader);
}

void RenderPipeline::doGeometryPass()
{
	this->bindTargetFramebuffer(this->gBuffer);

	this->gBuffer.bindTargetColorBuffers({ "color", "position", "normal", "material" });

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDepthMask(GL_TRUE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	this->useShader(this->lightMapGeometryShader);
	this->state.getUsedCamera().uploadData(*this->activeShader);
	this->state.render(*this->activeShader);

	this->useShader(this->geometryPassShader);
	this->state.getUsedCamera().uploadData(*this->activeShader);
	this->state.render(*this->activeShader);


	this->useShader(this->normalMappingGeometryShader);
	this->state.getUsedCamera().uploadData(*this->activeShader);
	this->state.render(*this->activeShader);

	glDepthMask(GL_FALSE);
	glDisable(GL_DEPTH_TEST);

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

void RenderPipeline::doHudPass()
{
	this->bindDefaultFramebuffer();
	glClear(GL_DEPTH_BUFFER_BIT);
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	state.renderHud(writer2D);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Globals::showFps) {
		std::string s = "FPS: " + std::to_string(Globals::fps);

		writer2D.print(s.c_str(), 32, 550, 32);
	}

	if (Globals::isHelp) {
		std::string help = "Show help-F1:      " + std::to_string(Globals::isHelp);
		std::string debug = "Show debug-F2:     " + std::to_string(Globals::isDebug);
		std::string wireframe = "Show wireframe-F3: " + std::to_string(false);
		std::string normalMap = "Show normalmap-F4: "+std::to_string(Globals::useNormalMap);
		std::string lightMap = "Show lightmap-F5:  " + std::to_string(Globals::useLightMap);
		std::string subdivision = "Show subdivision level-F6:  " + std::to_string(Globals::subdivisionLevel);

		writer2D.print(help.c_str(), 32, 500, 26);
		writer2D.print(debug.c_str(), 32, 470, 26);
		writer2D.print(wireframe.c_str(), 32, 440, 26);
		writer2D.print(normalMap.c_str(), 32, 410, 26);
		writer2D.print(lightMap.c_str(), 32, 380, 26);
		writer2D.print(subdivision.c_str(), 32, 350, 26);
	}

	glDisable(GL_BLEND);
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
