#pragma once
#include "..\libimport\glew.h"
#include "..\libimport\glfw.h"

#include "..\GameImport.h"
#include GAMESTATE_H

#include "..\physics\PhysicsPipeline.h"

#include "..\graphics\camera.h"
#include "..\input\inputHandler.h"
#include "..\input\debugCameraHandler.h"
#include "..\graphics\HudWriter.h"

#include "../graphics/RenderPipeline.h"

#define FPS_MEMORY_SIZE 120

class Gameloop {
public:
	Gameloop(unsigned int fps);
	~Gameloop();

private:
	unsigned int fps;
	float sPerFrame;
	float fpsMemory[FPS_MEMORY_SIZE];
	int fpsIndex;
	Glfw glfw;
	Window window;
	Glew glew;
	InputHandler inputHandler;
	bool isWireframe;
	bool isCulling;
	Camera debugCamera;
	DebugCameraHandler debugCameraHandler;
	GAMESTATE state;
	PhysicsPipeline physixPipe;
	HudWriter writer2D;
	RenderPipeline renderPipeline;

public:
	void run();

private:
	void init();
	void update();
	void render(float dt);

	static void APIENTRY DebugCallbackDefault(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
	static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg);
};