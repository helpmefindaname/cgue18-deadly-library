#pragma once
#include "../graphics/camera.h"
#include "inputHandler.h"
#include "../gameObjects/Player.h"

class GameCameraHandler
{
public:
	GameCameraHandler(Camera& camera, Player* player);
	~GameCameraHandler();

private:
	Camera & usedCamera;
	Player * usedPlayer;

	float characterSpeed;
	float cameraDirection;
	glm::vec3 cameraPosition;

	float height = 3.0f;
	float dist = 5.0f;
	float distChangePerS = 0.25f;
	float rotChangePerS = Glm::pi / 4.0f;


public:
	void update(float dt);

private:
	glm::vec3 getForwardDirection(float direction);
	glm::vec3 calculateOffset(float direction);
};

