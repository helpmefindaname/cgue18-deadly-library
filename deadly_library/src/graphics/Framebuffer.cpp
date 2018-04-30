#include "Framebuffer.h"
#include "../libimport/glew.h"
#include <stdexcept>
#include "../config.h"

Framebuffer::Framebuffer(bool useDepthStencilBuffer, int depthStencilBufferWidth, int depthStencilBufferHeight, std::vector<std::string> colorBufferNames, std::vector<int> colorBufferWidths, std::vector<int> colorBufferHeights, std::vector<GLint> internalFormats) :
	handle(0),
	useDepthStencilBuffer(useDepthStencilBuffer),
	depthStencilBuffer(depthStencilBufferWidth, depthStencilBufferHeight, GL_DEPTH_STENCIL, GL_DEPTH32F_STENCIL8, GL_FLOAT_32_UNSIGNED_INT_24_8_REV, GL_DEPTH_STENCIL_ATTACHMENT, GL_NEAREST, GL_CLAMP_TO_EDGE),
	fullscreenQuad({ 0, 1, 2, 2, 1, 3 }, {
		glm::vec3(-1.0f, -1.0f, 0.0f),
		glm::vec3(1.0f, -1.0f, 0.0f),
		glm::vec3(-1.0f, 1.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, 0.0f)
		})
{
	for (size_t i = 0; i < colorBufferNames.size(); i++) {
		this->colorBuffers.emplace(colorBufferNames.at(i), Texture(colorBufferWidths.at(i), colorBufferHeights.at(i), GL_RGBA, internalFormats.at(i), GL_FLOAT, GL_COLOR_ATTACHMENT0 + (int)i, GL_LINEAR, GL_CLAMP_TO_EDGE));
	}

	glGenFramebuffers(1, &this->handle);
	glBindFramebuffer(GL_FRAMEBUFFER, this->handle); // GL_FRAMEBUFFER

	for (auto& iterator : this->colorBuffers) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, iterator.second.getAttachment(), GL_TEXTURE_2D, iterator.second.getHandle(), 0);
	}

	if (this->useDepthStencilBuffer) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, this->depthStencilBuffer.getAttachment(), GL_TEXTURE_2D, this->depthStencilBuffer.getHandle(), 0);
	}
	int i = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if ((this->useDepthStencilBuffer || this->colorBuffers.size() > 0) && glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		throw std::runtime_error("Failed to create framebuffer!");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

Framebuffer::Framebuffer(Framebuffer&& framebuffer) :
	handle(framebuffer.handle),
	useDepthStencilBuffer(useDepthStencilBuffer),
	colorBuffers(std::move(framebuffer.colorBuffers)),
	depthStencilBuffer(std::move(framebuffer.depthStencilBuffer))
{
	framebuffer.handle = 0;
}

Framebuffer::~Framebuffer() {
	if (this->handle != 0) {
		glDeleteFramebuffers(1, &this->handle);
	}
}

GLuint Framebuffer::getHandle() {
	return this->handle;
}

void Framebuffer::renderQuad(Shader& shader) {
	this->fullscreenQuad.uploadData(shader);
	this->fullscreenQuad.render();
}

void Framebuffer::bindTargetColorBuffers(std::vector<std::string> colorBufferNames) {
	std::vector<GLenum> bufferAttachments(colorBufferNames.size());

	for (unsigned int i = 0; i < colorBufferNames.size(); i++) {
		Texture& colorBuffer = this->colorBuffers.at(colorBufferNames.at(i));
		bufferAttachments[i] = colorBuffer.getAttachment();
	}

	glDrawBuffers((int)bufferAttachments.size(), &bufferAttachments[0]);
}

void Framebuffer::bindSourceColorBuffer(std::string bufferName) {
	if (this->colorBuffers.find(bufferName) == this->colorBuffers.end()) {
		this->colorBuffers.at(bufferName);
	}

	Texture& colorBuffer = this->colorBuffers.at(bufferName);
	glReadBuffer(colorBuffer.getAttachment());
}

void Framebuffer::bindTextures(Shader& shader, std::vector<std::string> colorBufferNames, std::vector<std::string> targetBufferNames, bool bindDepthTexture, std::string targetBufferName) {
	int unit = 0;

	if (bindDepthTexture) {
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, this->depthStencilBuffer.getHandle());
		glUniform1i(shader.getUniformLocation(targetBufferName), unit);

		unit++;
	}

	for (unsigned int i = 0; i < colorBufferNames.size(); i++) {
		Texture& colorBuffer = this->colorBuffers.at(colorBufferNames[i]);
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, colorBuffer.getHandle());
		glUniform1i(shader.getUniformLocation(targetBufferNames[i]), unit);

		unit++;
	}
}
void Framebuffer::bindTextures(Shader& shader, int startPosition, std::vector<std::string> colorBufferNames, std::vector<std::string> targetBufferNames)
{
	int unit = startPosition;

	for (unsigned int i = 0; i < colorBufferNames.size(); i++) {
		Texture& colorBuffer = this->colorBuffers.at(colorBufferNames[i]);
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, colorBuffer.getHandle());
		glUniform1i(shader.getUniformLocation(targetBufferNames[i]), unit);

		unit++;
	}
}