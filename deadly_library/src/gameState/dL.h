#pragma once
#include "..\libimport\glew.h"
#include "..\libimport\glfw.h"
#include "../input/inputHandler.h"
#include "../graphics/camera.h"
#include "../physics/PhysicsPipeline.h"
#include "../graphics/Framebuffer.h"
#include <memory>
#include "../graphics/texture.h"
#include "../graphics\model\Material.h"
#include "../graphics/model/Geometry.h"
#include "../gameObjects/Player.h"
#include "../input/GameCameraHandler.h"
#include "../gameObjects/Light.h"
#include "../graphics/HudWriter.h"

class DeadlyLibrary
{
public:
	DeadlyLibrary();
	~DeadlyLibrary();

private:
	Camera gameCamera;
	Camera* usedCamera;

	Player* player;
	std::shared_ptr<model::Geometry> world;

	GameCameraHandler gameCameraHandler;

	std::vector<std::shared_ptr<Light>> lights;

	std::string lvlFile = Config::getString("LvlFile");

	bool isStart;
	bool isWin;
	bool isEnd;

	int lives;

	std::shared_ptr<Texture> startScreen;
	std::shared_ptr<Texture> winScreen;
	std::shared_ptr<Texture> looseScreen;
	std::shared_ptr<Texture> heartScreen;

public:
	void init(PhysicsPipeline& physiX);
	void generateLightMaps(Shader& shader, Framebuffer& lightMapBuffer);
	void update(InputHandler& input, float dt);
	void render(Shader& activeShader);

	Camera& getGameCamera();
	void setUsedCamera(Camera& usedCamera);
	Camera& getUsedCamera();

	std::vector<std::shared_ptr<Light>> getLights();

	void renderHud(HudWriter& writer2D);

private:
};

