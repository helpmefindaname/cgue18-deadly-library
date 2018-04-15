#include "debugCameraHandler.h"
#include "../config.h"

DebugCameraHandler::DebugCameraHandler(Camera* camera)
	: usedCamera(camera)
{
	this->width = Config::getInt("WindowWidth");
	this->height = Config::getInt("WindowHeight");
	this->cameraSpeed = Config::getFloat("CameraSpeed");
}

DebugCameraHandler::~DebugCameraHandler() {
}

void DebugCameraHandler::checkInput(InputHandler& inputhandler) {
	// Camera Rotation
	glm::vec2 mouse = inputhandler.getMouseMovement() * (1.0f / std::fminf((float)this->width, (float)this->height));
	usedCamera->yaw(-mouse.x);
	usedCamera->pitch(-mouse.y);

	// Camera Movement
	if (inputhandler.getEvent("cameraForward")) {
		usedCamera->translateForward(this->cameraSpeed);
	}
	if (inputhandler.getEvent("cameraBackward")) {
		usedCamera->translateBackward(this->cameraSpeed);
	}
	if (inputhandler.getEvent("cameraRight")) {
		usedCamera->translateRight(this->cameraSpeed);
	}
	if (inputhandler.getEvent("cameraLeft")) {
		usedCamera->translateLeft(this->cameraSpeed);
	}
	if (inputhandler.getEvent("cameraUp")) {
		usedCamera->translateUp(this->cameraSpeed);
	}
	if (inputhandler.getEvent("cameraDown")) {
		usedCamera->translateDown(this->cameraSpeed);
	}
}