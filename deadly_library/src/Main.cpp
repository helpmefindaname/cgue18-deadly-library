/*
* Copyright 2017 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/

#include "libimport\glew.h"
#include "libimport\glfw.h"

#include <sstream>

#include "Geometry.h"

#include "graphics\camera.h"
#include "graphics\shader.h"
#include "graphics\texture.h"

#include "Material.h"
#include "Light.h"

#include "input\debugCameraHandler.h"
#include "input\inputHandler.h"

#include "window.h"
#include "config.h"
#include <iostream>

#define EXIT_WITH_ERROR(err) \
	std::cout << "ERROR: " << err << std::endl; \
	system("PAUSE"); \
	return EXIT_FAILURE;

/* --------------------------------------------- */
// Prototypes
/* --------------------------------------------- */

static void APIENTRY DebugCallbackDefault(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam);
static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg);
static void handleInput(InputHandler& inputHandler, Window& window, Camera& defaultCamera, Camera& debugCamera);

void setPerFrameUniforms(graphics::Shader* shader, Camera& camera, DirectionalLight& dirL, PointLight& pointL);


/* --------------------------------------------- */
// Global variables
/* --------------------------------------------- */

static bool _wireframe = false;
static bool _isDebug = false;
static bool _culling = true;
static bool _dragging = false;
static bool _strafing = false;
static float _zoom = 12.0f;

static DebugCameraHandler* debugCameraHandler;


/* --------------------------------------------- */
// Main
/* --------------------------------------------- */

int main(int argc, char** argv)
{
	/* --------------------------------------------- */
	// Load settings.ini
	/* --------------------------------------------- */

	Config::readFile("assets/settings.ini");

	int window_width = Config::getInt("WindowWidth");
	int window_height = Config::getInt("WindowHeight");

	float fov = Config::getFloat("FieldOfView");
	float nearZ = Config::getFloat("NearPlane");
	float farZ = Config::getFloat("FarPlane");

	/* --------------------------------------------- */
	// Create context
	/* --------------------------------------------- */

	Glfw glfw;

	Window window;

	// Initialize GLEW
	Glew glew;

#if _DEBUG
	// Register your callback function.
	glDebugMessageCallback(DebugCallbackDefault, NULL);
	// Enable synchronous callback. This ensures that your callback function is called
	// right after an error has occurred. 
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#endif

	InputHandler inputHandler(window);

	// set some GL defaults
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);


	/* --------------------------------------------- */
	// Initialize scene and render loop
	/* --------------------------------------------- */
	{
		std::shared_ptr<graphics::Shader> textureShader = std::make_shared<graphics::Shader>("assets/shader/texture");
		std::shared_ptr<Texture> sunTexture = std::make_shared<Texture>("assets/textures/sun.png");
		std::shared_ptr<Texture> moonTexture = std::make_shared<Texture>("assets/textures/moon.png");
		std::shared_ptr<Texture> earthTexture = std::make_shared<Texture>("assets/textures/earth.png");


		// Create materials
		std::shared_ptr<Material> sunMaterial = std::make_shared<TextureMaterial>(textureShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, sunTexture);
		std::shared_ptr<Material> earthMaterial = std::make_shared<TextureMaterial>(textureShader, glm::vec3(0.05f, 0.9f, 0.1f), 5.0f, earthTexture);
		std::shared_ptr<Material> moonMaterial = std::make_shared<TextureMaterial>(textureShader, glm::vec3(0.05f, 0.9f, 0.05f), 2.0f, moonTexture);

		// Create geometry
		Geometry sun = Geometry(glm::mat4(1.0f), Geometry::createSphereGeometry(64, 32, 2.0f), sunMaterial);
		Geometry* earth = sun.addChild(std::make_unique<Geometry>(glm::mat4(1.0f), Geometry::createSphereGeometry(64, 32, 1.0f), earthMaterial));
		earth->setTransformMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(12, 0, 0)));
		Geometry* moon = earth->addChild(std::make_unique<Geometry>(glm::mat4(1.0f), Geometry::createSphereGeometry(64, 32, 0.5f), moonMaterial));
		moon->setTransformMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(3, 0, 0)));

		// Initialize camera
		Camera defaultCamera;
		defaultCamera.setPosition(glm::vec3(0, 0, -10));

		Camera debugCamera(defaultCamera);

		PointLight pointL(glm::vec3(1, 1, 1), glm::vec3(0), glm::vec3(1, 0, 0));
		DirectionalLight dirL(glm::vec3(0), glm::vec3(0));

		debugCameraHandler = new DebugCameraHandler(&debugCamera);

		// Render loop
		float t = float(glfwGetTime());
		float dt = 0.0f;
		float t_sum = 0.0f;
		while (!window.isClosed()) {
			// Clear backbuffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			handleInput(inputHandler, window, defaultCamera, debugCamera);

			// Set per-frame uniforms
			setPerFrameUniforms(textureShader.get(),
				_isDebug ? debugCamera : defaultCamera, dirL, pointL);

			// Hierarchical animation
			sun.transform(glm::rotate(glm::mat4(1.0f), glm::radians(15.0f) * dt, glm::vec3(0, 1, 0)));
			earth->transform(glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f) * dt, glm::vec3(0, 1, 0)));
			moon->transform(glm::rotate(glm::mat4(1.0f), glm::radians(60.0f) * dt, glm::vec3(0, 1, 0)));

			// Render
			sun.draw();

			// Compute frame time
			dt = t;
			t = float(glfwGetTime());
			dt = t - dt;
			t_sum += dt;

			inputHandler.process();

			window.swapBuffers();
		}
	}

	return EXIT_SUCCESS;
}

static void handleInput(InputHandler& inputHandler, Window& window, Camera& defaultCamera, Camera& debugCamera) {
	if (inputHandler.getEvent("toggleDebug")) {
		_isDebug = !_isDebug;
		debugCamera.setInverseViewMatrix(defaultCamera.getInverseViewMatrix());
	}

	if (_isDebug) {
		debugCameraHandler->checkInput(inputHandler);
	}

	if (inputHandler.getEvent("close")) {
		window.close();
	}

	if (inputHandler.getEvent("toggleWireFrame")) {
		_wireframe = !_wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, _wireframe ? GL_LINE : GL_FILL);
	}
	if (inputHandler.getEvent("toggleCulling")) {
		_culling = !_culling;
		if (_culling) glEnable(GL_CULL_FACE);
		else glDisable(GL_CULL_FACE);
	}
}


void setPerFrameUniforms(graphics::Shader* shader, Camera& camera, DirectionalLight& dirL, PointLight& pointL)
{
	shader->use();
	camera.uploadData(*shader);

	shader->setUniform("dirL.color", dirL.color);
	shader->setUniform("dirL.direction", dirL.direction);
	shader->setUniform("pointL.color", pointL.color);
	shader->setUniform("pointL.position", pointL.position);
	shader->setUniform("pointL.attenuation", pointL.attenuation);
}

static void APIENTRY DebugCallbackDefault(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const GLvoid* userParam) {
	if (id == 131185 || id == 131218) return; // ignore performance warnings (buffer uses GPU memory, shader recompilation) from nvidia
	std::string error = FormatDebugOutput(source, type, id, severity, message);
	std::cout << error << std::endl;
}

static std::string FormatDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg) {
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