#pragma once
#include "../libimport/glm.h"
#include "../input/inputHandler.h"
#include <shared_mutex>

#include "../Geometry.h"

class Player {
public:
	Player(glm::vec3 position, float direction);
	~Player();

private:
	glm::vec3 position;
	float direction;
	std::shared_ptr<Geometry> sun; //SUN
	
	float playerSpeed = 0.5f;
	float roatationSpeed = 0.1f;

public:
	void init(std::shared_ptr<TextureMaterial> playerMaterial);
	glm::vec3 getPosition();
	float getDirection();
	void update(InputHandler& inputHandler, float dt);
	void draw();

private:

	glm::vec3 forward();
};
