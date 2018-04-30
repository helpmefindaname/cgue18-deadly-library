#pragma once
#include "..\libimport\glew.h"
#include "..\libimport\glfw.h"
#include "../input/inputHandler.h"
#include "../graphics/camera.h"
#include "../physics/PhysicsPipeline.h"
#include "../graphics/Framebuffer.h"
#include <memory>
#include "../graphics/texture.h"
#include "../Material.h"
#include "../Geometry.h"
#include "../gameObjects/Player.h"
#include "../input/GameCameraHandler.h"

class DeadlyLibrary
{
public:
	DeadlyLibrary();
	~DeadlyLibrary();

private:
	Camera gameCamera;
	Camera* usedCamera;
	Player* player;

	GameCameraHandler gameCameraHandler;

	std::shared_ptr<Shader> textureShader;
	std::shared_ptr<Texture> blockTexture;
	std::shared_ptr<TextureMaterial> blockMaterial;
	std::shared_ptr<Geometry> world;
	std::shared_ptr<Texture> playerTexture;
	std::shared_ptr<TextureMaterial> playerMaterial;

public:
	void init(PhysicsPipeline& physiX);
	void update(InputHandler& input, float dt);
	void render(Shader& activeShader);

	Camera& getGameCamera();
	void setUsedCamera(Camera& usedCamera);
	Camera& getUsedCamera();

private:
};

