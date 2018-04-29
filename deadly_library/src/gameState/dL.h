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
class DeadlyLibrary
{
public:
	DeadlyLibrary();
	~DeadlyLibrary();

private:
	Camera gameCamera;
	Camera* usedCamera;

public:
	void init(PhysicsPipeline& physiX);
	void update(InputHandler& input, float dt);
	void render();

	Camera& getGameCamera();
	void setUsedCamera(Camera& usedCamera);

private:
	std::shared_ptr<Shader> textureShader;
	std::shared_ptr<Texture> blockTexture;
	std::shared_ptr<TextureMaterial> blockMaterial;
	std::shared_ptr<Geometry> world;

	Framebuffer gBuffer;
};

