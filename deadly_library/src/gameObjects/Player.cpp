#include "Player.h"


Player::Player(glm::vec3 position, float direction)
	:position(position),
	direction(direction),
	velocityY(0.0f),
	playerGeometry()
{}

Player::~Player()
{}

void Player::init(std::shared_ptr<TextureMaterial> playerMaterial, PhysicsPipeline& physiX)
{
	this->playerGeometry= std::make_shared<Geometry>(glm::mat4(1.0f), Geometry::createSphereGeometry(64, 32, 0.5f), playerMaterial); //TODO:SUN
	this->playerMesh = new Mesh("assets/objects/raccoon.mesh");
	//this->playerGeometry = playerMesh-> ;
	playerObject = physiX.createController(PxVec3(position.x, position.y, position.z), PxVec3(0.25f));
	
}

glm::vec3 Player::getPosition() {
	return this->position;
}

float Player::getDirection() {
	return this->direction;
}

void Player::update(InputHandler& inputHandler, float dt)
{
	playerObject->move(playerObject->getUpDirection()*velocityY*dt, 0.0001f, dt, 0);

	velocityY -= gravity * dt;

	if (inputHandler.getEvent("moveForward")) {
		playerObject->move(forward() * dt* playerSpeed, 0.0001f, dt, 0);
	}

	if (inputHandler.getEvent("moveBackward")) {
		playerObject->move(-forward() * dt* playerSpeed, 0.0001f, dt, 0);
	}

	if (inputHandler.getEvent("turnLeft")) {
		direction -= roatationSpeed * dt;
	}

	if (inputHandler.getEvent("turnRight")) {
		direction += roatationSpeed * dt;
	}

	if (direction > Glm::pi)direction -= 2 * Glm::pi;
	if (direction < Glm::pi)direction += 2 * Glm::pi;

	float lastY = position.y;
	PxExtendedVec3 pos = playerObject->getPosition();
	position = glm::vec3(pos.x, pos.y, pos.z);

	if (abs(lastY - position.y) < 0.00001f && inputHandler.getEvent("jump")) {
		velocityY = jumpPower;
	}

	this->playerGeometry->setTransformMatrix(glm::translate(position)*glm::rotate(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Player::draw()
{
	this->playerGeometry->draw();
}

PxVec3 Player::forward()
{
	return PxVec3(sin(direction), 0.0f, -cos(direction));
}
