#pragma once
#include "../graphics/camera.h"
#include "inputHandler.h"

class GameCameraHandler
{
public:
	GameCameraHandler(Camera& camera);
	~GameCameraHandler();

private:
	Camera & usedCamera;

	int width;
	int height;
	float characterSpeed;


public:
	void checkInput(InputHandler& inputhandler);
};

