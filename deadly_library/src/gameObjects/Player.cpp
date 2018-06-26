#include "Player.h"
#include "../graphics/model/MeshLoader.h"
#include "../graphics/model/MaterialLoader.h"
#include "../graphics/TextureLoader.h"

Player::Player(glm::vec3 position, float direction)
	:position(position),
	direction(direction),
	velocityY(0.0f),
	playerGeometry()
{}

Player::~Player()
{}

void Player::init(PhysicsPipeline& physiX)
{
	this->playerGeometry = new model::Geometry(MeshLoader::loadMesh("assets/objects/raccoonBody.mesh"),
		MaterialLoader::loadMaterial("assets/materials/raccoon.material"), TextureLoader::loadTexture("assets/textures/raccoon.png"), nullptr, nullptr, glm::translate(position));
	playerGeometry->subdivide();
	leftFoot = playerGeometry->addChild(std::make_unique<model::Geometry>(MeshLoader::loadMesh("assets/objects/raccoonLeftFoot.mesh"),
		MaterialLoader::loadMaterial("assets/materials/raccoon.material"), TextureLoader::loadTexture("assets/textures/raccoon.png")));
	rightFoot = playerGeometry->addChild(std::make_unique<model::Geometry>(MeshLoader::loadMesh("assets/objects/raccoonRightFoot.mesh"),
		MaterialLoader::loadMaterial("assets/materials/raccoon.material"), TextureLoader::loadTexture("assets/textures/raccoon.png")));
	playerGeometry->addChild(std::make_unique<model::Geometry>(MeshLoader::loadMesh("assets/objects/backpack.mesh"),
		MaterialLoader::loadMaterial("assets/materials/raccoon.material"), TextureLoader::loadTexture("assets/textures/backpack.png")))->subdivide();
	playerObject = physiX.createController(PxVec3(position.x, position.y, position.z), PxVec3(0.25f));

}

void Player::setPosition(glm::vec3 position) {
	this->position = position;
	this->direction = -Glm::pi / 2;
	playerObject->setPosition(PxExtendedVec3(position.x, position.y, position.z));
}

glm::vec3 Player::getPosition() {
	return this->position;
}

float Player::getDirection() {
	return this->direction;
}

void Player::update(InputHandler& inputHandler, float dt)
{
	velocityY -= gravity * dt;

	playerObject->move(playerObject->getUpDirection() * velocityY * dt, 0.0001f, dt, 0);
	movecounter++;

	if (inputHandler.getEvent("moveForward")) {
		playerObject->move(forward() * dt * playerSpeed, 0.0001f, dt, 0);
	}

	if (inputHandler.getEvent("moveBackward")) {
		playerObject->move(-forward() * dt * playerSpeed, 0.0001f, dt, 0);
	}

	if (inputHandler.getEvent("turnLeft")) {
		direction += roatationSpeed * dt;
	}

	if (inputHandler.getEvent("turnRight")) {
		direction -= roatationSpeed * dt;
	}

	if (direction > Glm::pi) direction -= 2 * Glm::pi;
	if (direction < Glm::pi) direction += 2 * Glm::pi;

	float lastY = position.y;
	PxExtendedVec3 pos = playerObject->getPosition();
	position = glm::vec3(pos.x, pos.y, pos.z);
	const int animationFrameCount = 16;

	isOnFloor = abs(lastY - position.y) < 0.00001f;

	if (isOnFloor) {
		velocityY = 0.0f;
	}
	else {
		movecounter = animationFrameCount / 2;
	}

	if (isOnFloor && inputHandler.getEvent("jump")) {
		velocityY = jumpPower;
	}

	glm::vec3 forwardVec = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 side = glm::cross(forwardVec, up);

	float footAngle = ((((movecounter / animationFrameCount) % 2) * 2 - 1) * (movecounter % animationFrameCount - (animationFrameCount / 2)) / (animationFrameCount / 2.0f)) * Glm::pi*0.25f;
	glm::vec3 footOffset = glm::vec3(0.0f, .5f, 0.0f);


	leftFoot->setTransformMatrix(glm::translate(footOffset)* glm::rotate(footAngle, side)*glm::translate(-footOffset));
	rightFoot->setTransformMatrix(glm::translate(footOffset)* glm::rotate(-footAngle, side)*glm::translate(-footOffset));
	this->playerGeometry->setTransformMatrix(glm::translate(position)*glm::rotate(direction, glm::vec3(0.0f, 1.0f, 0.0f)));
}

void Player::draw(Shader& activeShader)
{
	this->playerGeometry->render(activeShader);
}

PxVec3 Player::forward()
{
	return PxVec3(-cos(direction), 0.0f, sin(direction));
}

bool Player::onFloor() {
	return isOnFloor;
}