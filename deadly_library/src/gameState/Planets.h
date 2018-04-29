#pragma once
#include "..\libimport\glew.h"
#include "..\libimport\glfw.h"
#include "../graphics/camera.h"
#include "../input/inputHandler.h"
#include <shared_mutex>

#include "../graphics/texture.h"
#include "../Material.h"
#include "../Geometry.h"
#include "../physics/PhysicsPipeline.h"
#include "../Light.h"


class Planets {
public:
	Planets();
	~Planets();

private:
	Camera gameCamera;
	Camera* usedCamera;

	std::shared_ptr<Shader> textureShader;

	std::shared_ptr<Texture> sunTexture;
	std::shared_ptr<Texture> moonTexture;
	std::shared_ptr<Texture> earthTexture;

	std::shared_ptr<Material> sunMaterial;
	std::shared_ptr<Material> earthMaterial;
	std::shared_ptr<Material> moonMaterial;

	std::shared_ptr<Geometry> sun;
	Geometry* earth;
	Geometry* moon;

	PointLight pointL;
	DirectionalLight dirL;

public:
	void init(PhysicsPipeline& physiX);
	void update(InputHandler& input, float dt);
	void render();

	Camera& getGameCamera();
	void setUsedCamera(Camera& usedCamera);

private:
};