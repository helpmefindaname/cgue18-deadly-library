#include "Player.h"


Player::Player(glm::vec3 position, float direction)
	:position(position),
	direction(direction),
	sun()//SUN
{}

Player::~Player()
{}

void Player::init(std::shared_ptr<TextureMaterial> playerMaterial)
{
	this->sun = std::make_shared<Geometry>(glm::mat4(1.0f), Geometry::createSphereGeometry(64, 32, 2.0f), playerMaterial); //SUN
}

glm::vec3 Player::getPosition() {
	return this->position;
}

float Player::getDirection() {
	return this->direction;
}

void Player::update(InputHandler& inputHandler, float dt)
{
	

	if (inputHandler.getEvent("moveForward")) {
		position += forward() * dt* playerSpeed;
	}

	if (inputHandler.getEvent("moveBackward")) {
		position -= forward() * dt* playerSpeed;
	}

	if (inputHandler.getEvent("turnLeft")) {
		direction -= roatationSpeed * dt;
	}

	if (inputHandler.getEvent("turnRight")) {
		direction += roatationSpeed * dt;
	}

	if (direction > Glm::pi)direction -= 2 * Glm::pi;
	if (direction < Glm::pi)direction += 2 * Glm::pi;


	this->sun->setTransformMatrix(glm::translate(position)*glm::rotate(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Player::draw()
{
	this->sun->draw();
}

glm::vec3 Player::forward()
{
	return glm::vec3(sin(direction), 0.0f, -cos(direction));
}
