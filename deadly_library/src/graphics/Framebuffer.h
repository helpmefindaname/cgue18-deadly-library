#pragma once
#include "texture.h"
#include <vector>
#include <unordered_map>
#include "shader.h"
#include "model/Mesh.h"

class Framebuffer
{
public:
public:
	Framebuffer(bool useDepthStencilBuffer,
		int depthStencilBufferWidth,
		int depthStencilBufferHeight,
		std::vector<std::string> colorBufferNames,
		std::vector<int> colorBufferWidths,
		std::vector<int> colorBufferHeights,
		std::vector<GLint> internalFormats);
	Framebuffer(Framebuffer&& framebuffer);
	~Framebuffer();

private:
	GLuint handle;

	bool useDepthStencilBuffer;

	std::unordered_map<std::string, Texture> colorBuffers;
	Texture depthStencilBuffer;

	Mesh fullscreenQuad;

public:
	GLuint getHandle();

	void renderQuad(Shader& shader);

	void bindTargetColorBuffers(std::vector<std::string> colorBufferNames);
	void bindSourceColorBuffer(std::string bufferName);

	void bindTextures(Shader& shader, std::vector<std::string> colorBufferNames, std::vector<std::string> targetBufferNames, bool bindDepthTexture = false, std::string targetBufferName = "");
	void bindTextures(Shader& shader, int startPosition, std::vector<std::string> colorBufferNames, std::vector<std::string> targetBufferNames);

	std::shared_ptr<Texture> createScreenShot(std::string colorBufferName);
};