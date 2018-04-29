#include "GameCameraHandler.h"
#include "../config.h"

GameCameraHandler::GameCameraHandler(Camera& camera):usedCamera(camera)
{
	this->width = Config::getInt("WindowWidth");
	this->height = Config::getInt("WindowHeight");
	this->characterSpeed = Config::getFloat("CharacterSpeed"); // camera speed - winkelverzoegerung
}


GameCameraHandler::~GameCameraHandler()
{
}

void GameCameraHandler::checkInput(InputHandler& inputhandler)
{
	// Camera Rotation when character turn left or right
	//Camera tuns with character

	// Camera Movement
	if (inputhandler.getEvent("cameraForward")) {
		usedCamera.translateForward(this->characterSpeed);
	}
	if (inputhandler.getEvent("cameraBackward")) {
		usedCamera.translateBackward(this->characterSpeed);
	}
	if (inputhandler.getEvent("jump")) {
		//JUMP //jumps a bit with character
	}
}