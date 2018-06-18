#include "GameCameraHandler.h"
#include "../config.h"

GameCameraHandler::GameCameraHandler(Camera& camera, Player* player)
	:usedCamera(camera),
	usedPlayer(player),
	cameraDirection(player->getDirection()),
	cameraPosition(player->getPosition())
{
	usedCamera.updateViewMatrix(cameraPosition + calculateOffset(cameraDirection), getForwardDirection(cameraDirection), glm::vec3(0.0f, 1.0f, 0.0f));
}


GameCameraHandler::~GameCameraHandler()
{
}

void GameCameraHandler::addPosition(float height) {
	cameraPosition.x = 0;
	cameraPosition.y += height;
	cameraPosition.z = 0;
	cameraDirection = -Glm::pi / 2;
}

void GameCameraHandler::setPlayer(Player* player)
{
	this->usedPlayer = player;
}

void GameCameraHandler::update(float dt)
{
	float aimDirection = usedPlayer->getDirection();
	glm::vec3 aimPosition = usedPlayer->getPosition();

	float posL = glm::length(aimPosition - cameraPosition);

	if (posL > distChangePerS * dt) {
		cameraPosition += (aimPosition - cameraPosition)/posL * distChangePerS * dt;
		posL -= distChangePerS * dt;
	}
	else {
		cameraPosition = aimPosition;
	}

	if (posL > maxDist) {
		cameraPosition += (aimPosition - cameraPosition) / posL * (posL- maxDist);
	}

	float dDir = aimDirection - cameraDirection;
	
	if (dDir > Glm::pi) {
		dDir -= 2 * Glm::pi;
	}
	else if (dDir < -Glm::pi) {
		dDir += 2 * Glm::pi;
	}

	if (abs(dDir) < rotChangePerS * dt) {
		cameraDirection = aimDirection;
	}
	else {
		cameraDirection += Glm::sign(dDir) * rotChangePerS * dt;
	}


	usedCamera.updateViewMatrix(cameraPosition + calculateOffset(cameraDirection), getForwardDirection(cameraDirection), glm::vec3(0.0f, 1.0f, 0.0f));
}


glm::vec3 GameCameraHandler::getForwardDirection(float direction) {
	return glm::normalize(-calculateOffset(direction));
}


glm::vec3 GameCameraHandler::calculateOffset(float direction) {
	return glm::vec3(cos(direction)*dist, height, -sin(direction)*dist);
}