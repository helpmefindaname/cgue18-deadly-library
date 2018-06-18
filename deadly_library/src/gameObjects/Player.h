#pragma once
#include "../libimport/glm.h"
#include "../input/inputHandler.h"
#include <shared_mutex>
#include "../config.h"
#include "../graphics/model/Geometry.h"
#include "../physics/PhysicsPipeline.h"
#include "../graphics/model/Mesh.h"

class Player {
public:
	Player(glm::vec3 position, float direction);
	~Player();

private:
	glm::vec3 position;
	float direction;
	model::Geometry* playerGeometry;
	model::Geometry* leftFoot;
	model::Geometry* rightFoot;
	PxController* playerObject;
	float velocityY;
	bool isOnFloor;
	int movecounter = 0;
	
	float jumpPower = Config::getFloat("JumpPower");
	float gravity = Config::getFloat("Gravity");
	float playerSpeed = Config::getFloat("PlayerSpeed");
	float roatationSpeed = Config::getFloat("RotationSpeed");

public:
	void init(PhysicsPipeline& physiX);
	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();
	float getDirection();
	void update(InputHandler& inputHandler, float dt);
	void draw(Shader& activeShader);
	bool onFloor();

private:
	PxVec3 forward();
};
