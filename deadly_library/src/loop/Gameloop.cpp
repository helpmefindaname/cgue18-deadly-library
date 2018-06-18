#include "Gameloop.h"
#include <sstream>
#include <iostream>
#include "../Globals.h"

Gameloop::Gameloop(unsigned int fps) :
	fps(fps),
	sPerFrame(1.0f / fps),
	glfw(),
	window(),
	glew(),
	inputHandler(this->window),
	debugCamera(),
	debugCameraHandler(debugCamera),
	physixPipe(),
	state(),
	writer2D(),
	renderPipeline(state),
	fpsIndex(0)
{
	for (int i = 0; i < FPS_MEMORY_SIZE; i++)
	{
		fpsMemory[i] = sPerFrame;
	}
#if _DEBUG
	// Register your callback function.
	glDebugMessageCallback(DebugCallbackDefault, NULL);
	// Enable synchronous callback. This ensures that your callback function is called
	// right after an error has occurred. 
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif
}

Gameloop::~Gameloop()
{}

void Gameloop::run()
{
	this->init();

	float t = float(glfwGetTime());
	float dt = 0.0f;
	float t_sum = 0.0f;
	int updateCount = 0;
	while (!window.isClosed()) {
		updateCount = 0;

		while (t_sum >= this->sPerFrame && updateCount < 30) {
			t_sum -= this->sPerFrame;
			updateCount++;
			this->update();
		}

		this->render(dt);


		dt = t;
		t = float(glfwGetTime());
		dt = (t - dt);
		t_sum += dt;
	}
}

void Gameloop::init()
{
	state.init(physixPipe);
	renderPipeline.init();
}

void Gameloop::update()
{
	if (inputHandler.getEvent("toggleDebug")) {
		Globals::isDebug = !Globals::isDebug;
		debugCamera.setInverseViewMatrix(state.getGameCamera().getInverseViewMatrix());
	}

	if (inputHandler.getEvent("toggleHelp")) {
		Globals::isHelp = !Globals::isHelp;
	}

	if (inputHandler.getEvent("toggleFps")) {
		Globals::showFps = !Globals::showFps;
	}

	if (inputHandler.getEvent("toggleLightMap")) {
		Globals::useLightMap = !Globals::useLightMap;
	}

	if (inputHandler.getEvent("toggleNormalMapping")) {
		Globals::useNormalMap = !Globals::useNormalMap;
	}

	if (inputHandler.getEvent("toogleSubdivisionSurfaces")) {
		Globals::subdivisionLevel = (Globals::subdivisionLevel + 1) % 3;
	}

	if (Globals::isDebug) {
		debugCameraHandler.checkInput(inputHandler, this->sPerFrame);
		state.setUsedCamera(debugCamera);
	}
	else {
		state.setUsedCamera(state.getGameCamera());
		if (state.update(inputHandler, this->sPerFrame)) {
			init();
		}
	}

	if (inputHandler.getEvent("close")) {
		window.close();
	}

	if (inputHandler.getEvent("toggleWireFrame")) {
		Globals::isWireFrameMode = !Globals::isWireFrameMode;
	}

	if (inputHandler.getEvent("toggleCelShading")) {
		Globals::isCelShading = !Globals::isCelShading;
	}

	inputHandler.process();
}

void Gameloop::render(float dt)
{
	fpsMemory[fpsIndex++ % FPS_MEMORY_SIZE] = dt;
	float avgDt = 0.0f;
	for (int i = 0; i < FPS_MEMORY_SIZE; i++)
	{
		avgDt += fpsMemory[i];
	}

	avgDt /= FPS_MEMORY_SIZE;

	Globals::fps = int(1.0f / avgDt);

	renderPipeline.render();

	this->window.swapBuffers();
}

void Gameloop::DebugCallbackDefault(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * message, const GLvoid * userParam)
{
	if (id == 131185 || id == 131218) return; // ignore performance warnings (buffer uses GPU memory, shader recompilation) from nvidia
	std::string error = FormatDebugOutput(source, type, id, severity, message);
	std::cout << error << std::endl;
}

std::string Gameloop::FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg) {
	std::stringstream stringStream;
	std::string sourceString;
	std::string typeString;
	std::string severityString;

	switch (source) {
	case GL_DEBUG_SOURCE_API: {
		sourceString = "API";
		break;
	}
	case GL_DEBUG_SOURCE_APPLICATION: {
		sourceString = "Application";
		break;
	}
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: {
		sourceString = "Window System";
		break;
	}
	case GL_DEBUG_SOURCE_SHADER_COMPILER: {
		sourceString = "Shader Compiler";
		break;
	}
	case GL_DEBUG_SOURCE_THIRD_PARTY: {
		sourceString = "Third Party";
		break;
	}
	case GL_DEBUG_SOURCE_OTHER: {
		sourceString = "Other";
		break;
	}
	default: {
		sourceString = "Unknown";
		break;
	}
	}

	switch (type) {
	case GL_DEBUG_TYPE_ERROR: {
		typeString = "Error";
		break;
	}
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: {
		typeString = "Deprecated Behavior";
		break;
	}
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: {
		typeString = "Undefined Behavior";
		break;
	}
	case GL_DEBUG_TYPE_PORTABILITY_ARB: {
		typeString = "Portability";
		break;
	}
	case GL_DEBUG_TYPE_PERFORMANCE: {
		typeString = "Performance";
		break;
	}
	case GL_DEBUG_TYPE_OTHER: {
		typeString = "Other";
		break;
	}
	default: {
		typeString = "Unknown";
		break;
	}
	}

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: {
		severityString = "High";
		break;
	}
	case GL_DEBUG_SEVERITY_MEDIUM: {
		severityString = "Medium";
		break;
	}
	case GL_DEBUG_SEVERITY_LOW: {
		severityString = "Low";
		break;
	}
	default: {
		severityString = "Unknown";
		break;
	}
	}

	stringStream << "OpenGL Error: " << msg;
	stringStream << " [Source = " << sourceString;
	stringStream << ", Type = " << typeString;
	stringStream << ", Severity = " << severityString;
	stringStream << ", ID = " << id << "]";

	return stringStream.str();
}
