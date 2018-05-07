#include "dL.h"
#include "../level/LevelReader.h"

DeadlyLibrary::DeadlyLibrary()
	:gameCamera(),
	usedCamera(&gameCamera),
	player(new Player(glm::vec3(0, 5, 0), -Glm::pi / 2)),
	gameCameraHandler(this->gameCamera, this->player)
{}


DeadlyLibrary::~DeadlyLibrary()
{}

void DeadlyLibrary::init(PhysicsPipeline& physiX)
{
	LevelReader reader("assets\\1.lvl");
	this->player->init(physiX);

	this->world = reader.createWorldGeometry();

	reader.createWorldPhysics(physiX);

	lights.push_back(std::make_shared<Light>(glm::vec3(0.0f, 10.0f, 2.5f), 100.0f));
	lights.push_back(std::make_shared<Light>(glm::vec3(0.0f, 10.0f, -5.5f), 100.0f));
	lights.push_back(std::make_shared<Light>(glm::vec3(10.0f, 10.0f, -25.5f), 10.0f));
	lights.push_back(std::make_shared<Light>(glm::vec3(-10.0f, 10.0f, -25.5f), 10.0f));
}

void DeadlyLibrary::update(InputHandler& input, float dt)
{
	this->player->update(input, dt);
	this->gameCameraHandler.update(dt);
}

void DeadlyLibrary::render(Shader& activeShader)
{
	this->world->render(activeShader);
	this->player->draw(activeShader);
}

Camera& DeadlyLibrary::getGameCamera()
{
	return this->gameCamera;
}

void DeadlyLibrary::setUsedCamera(Camera& usedCamera)
{
	this->usedCamera = &usedCamera;
}

Camera& DeadlyLibrary::getUsedCamera()
{
	return *this->usedCamera;
}

std::vector<std::shared_ptr<Light>> DeadlyLibrary::getLights()
{
	return lights;
}
