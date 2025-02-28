#pragma once
#include "../graphics/camera.h"
#include "inputHandler.h"
#include "../gameObjects/Player.h"
#include "../config.h"

class GameCameraHandler
{
public:
	GameCameraHandler(Camera& camera, Player* player);
	~GameCameraHandler();

private:
	Camera& usedCamera;
	Player* usedPlayer;

	float characterSpeed;
	float cameraDirection;
	glm::vec3 cameraPosition;

	float maxDist = Config::getFloat("MaxDist");
	float height = Config::getFloat("Height");
	float dist = Config::getFloat("Dist");
	float distChangePerS = Config::getFloat("DistChangePerS");
	float rotChangePerS =  Config::getFloat("RotChangePerS") *  Glm::pi / 180.0f;


public:
	void update(float dt);
	void addPosition(float v);

	void setPlayer(Player* player);

private:
	glm::vec3 getForwardDirection(float direction);
	glm::vec3 calculateOffset(float direction);
};

