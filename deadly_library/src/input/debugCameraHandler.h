#pragma once

#include "../graphics/camera.h"
#include "inputHandler.h"

class DebugCameraHandler {
public:
	DebugCameraHandler(Camera* camera);
	~DebugCameraHandler();

private:
	Camera* usedCamera;

	int width;
	int height;
	float cameraSpeed;

public:
	void checkInput(InputHandler& inputhandler);
};
