#pragma once
#include "../libimport/glm.h"
#include "../input/inputHandler.h"
#include <shared_mutex>
#include "../config.h"
#include "../Geometry.h"

class Player {
public:
	Player(glm::vec3 position, float direction);
	~Player();

private:
	glm::vec3 position;
	float direction;
	std::shared_ptr<Geometry> sun; //SUN
	
	float playerSpeed = Config::getFloat("PlayerSpeed");
	float roatationSpeed = Config::getFloat("RotationSpeed");

public:
	void init(std::shared_ptr<TextureMaterial> playerMaterial);
	glm::vec3 getPosition();
	float getDirection();
	void update(InputHandler& inputHandler, float dt);
	void draw();

private:

	glm::vec3 forward();
};
