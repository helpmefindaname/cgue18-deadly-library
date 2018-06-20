#include "dL.h"
#include "../level/LevelReader.h"
#include "../graphics/TextureLoader.h"
#include "../Globals.h"

DeadlyLibrary::DeadlyLibrary()
	:gameCamera(),
	usedCamera(&gameCamera),
	player(glm::vec3(0, 3, 0), -Glm::pi / 2),
	gameCameraHandler(this->gameCamera, &this->player),
	isStart(true),
	isWin(false),
	isEnd(false),
	lives(3),
	startScreen(TextureLoader::loadTexture("assets/textures/dialog_begin.png")),
	winScreen(TextureLoader::loadTexture("assets/textures/win_condition.png")),
	looseScreen(TextureLoader::loadTexture("assets/textures/loose_condition.png")),
	heartScreen(TextureLoader::loadTexture("assets/textures/redHeart.png"))
{}

DeadlyLibrary::DeadlyLibrary(DeadlyLibrary&& copy)
	:gameCamera(copy.gameCamera),
	usedCamera(copy.usedCamera),
	player(copy.player),
	gameCameraHandler(copy.gameCameraHandler),
	isStart(copy.isStart),
	isWin(copy.isWin),
	isEnd(copy.isEnd),
	lives(copy.lives),
	startScreen(copy.startScreen),
	winScreen(copy.winScreen),
	looseScreen(copy.looseScreen),
	heartScreen(copy.heartScreen)
{}


DeadlyLibrary::~DeadlyLibrary()
{}

void DeadlyLibrary::init(PhysicsPipeline& physiX)
{
	LevelReader reader("assets\\" + (lvlFile));
	this->player.init(physiX);

	this->world = reader.createWorldGeometry();
	this->levelHeight = reader.getHeight();

	reader.createWorldPhysics(physiX);

	lights.clear();

	lights.push_back(std::make_shared<Light>(glm::vec3(0.0f, 2.0f, -2.5f)));
	lights.push_back(std::make_shared<Light>(glm::vec3(0.0f, 8.0f, -5.5f)));
	lights.push_back(std::make_shared<Light>(glm::vec3(10.0f, 10.0f, -25.5f)));
	lights.push_back(std::make_shared<Light>(glm::vec3(-10.0f, 10.0f, -25.5f)));
}

void DeadlyLibrary::generateLightMaps(Shader & shader, Framebuffer & lightMapBuffer)
{
	this->world->generateLightmap(shader, lightMapBuffer);
}

bool DeadlyLibrary::update(InputHandler& input, float dt)
{
	if (!isStart && !isEnd) {
		this->player.update(input, dt);
		this->gameCameraHandler.update(dt);

		if (player.getPosition().y < -50.0f) {
			float f = 53 - player.getPosition().y;
			player.setPosition(glm::vec3(0, 53, 0));
			gameCameraHandler.addPosition(f);
			if (--lives <= 0) {
				isWin = false;
				isEnd = true;
			}
		}
		if (this->player.onFloor() && this->player.getPosition().z < -this->levelHeight + 1) {
			isWin = true;
			isEnd = true;
		}
	}
	else if (isStart) {
		isStart = !input.getEvent("next");
	}
	else {
		if (input.getEvent("next")) {
			player = Player(glm::vec3(0, 3, 0), -Glm::pi / 2);
			gameCameraHandler.setPlayer(&player);
			isEnd = false;
			isWin = false;
			isStart = true;
			lives = 3;
			return true;
		}
	}
	return false;
}

void DeadlyLibrary::render(Shader& activeShader)
{
	this->world->render(activeShader);
	this->player.draw(activeShader);
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

void DeadlyLibrary::renderHud(HudWriter& writer2D)
{
	if (isStart) {
		writer2D.drawtexture(0, 100, 800, 250, *startScreen);
	}
	else if (isEnd) {
		if (isWin) {
			writer2D.drawtexture(0, 0, 800, 600, *winScreen);
		}
		else {
			writer2D.drawtexture(0, 0, 800, 600, *looseScreen);
		}
	}
	else {
		for (int i = 0; i < lives; i++)
		{
			writer2D.drawtexture(10 + 100 * i, 10, 80, 80, *heartScreen);
		}
	}
}
