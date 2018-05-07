#include "Planets.h"
#include "../libimport/glm.h"

Planets::Planets()
	:gameCamera(),
	usedCamera(&gameCamera)
	//gameCameraHandler(gameCamera)
{}

Planets::~Planets()
{ }

void Planets::init(PhysicsPipeline& physiX)
{
	textureShader = std::make_shared<Shader>("assets/shader/texture");
	sunTexture = std::make_shared<Texture>("assets/textures/sun.png");
	moonTexture = std::make_shared<Texture>("assets/textures/moon.png");
	earthTexture = std::make_shared<Texture>("assets/textures/earth.png");


	// Create materials
	//sunMaterial = std::make_shared<TextureMaterial>(textureShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, sunTexture);
	//earthMaterial = std::make_shared<TextureMaterial>(textureShader, glm::vec3(0.05f, 0.9f, 0.1f), 5.0f, earthTexture);
	//moonMaterial = std::make_shared<TextureMaterial>(textureShader, glm::vec3(0.05f, 0.9f, 0.05f), 2.0f, moonTexture);

	// Create geometry
	sun = std::make_shared<Geometry>(glm::mat4(1.0f), Geometry::createSphereGeometry(64, 32, 2.0f), sunMaterial);
	earth = sun->addChild(std::make_unique<Geometry>(glm::mat4(1.0f), Geometry::createSphereGeometry(64, 32, 1.0f), earthMaterial));
	earth->setTransformMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(12, 0, 0)));
	moon = earth->addChild(std::make_unique<Geometry>(glm::mat4(1.0f), Geometry::createSphereGeometry(64, 32, 0.5f), moonMaterial));
	moon->setTransformMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(3, 0, 0)));

	gameCamera.setPosition(glm::vec3(0, 0, -10));

	pointL = PointLight(glm::vec3(1, 1, 1), glm::vec3(0), glm::vec3(1, 0, 0));
	dirL = DirectionalLight(glm::vec3(0), glm::vec3(0));
}

void Planets::update(InputHandler & input, float dt)
{
	sun->transform(glm::rotate(glm::mat4(1.0f), glm::radians(15.0f) * dt, glm::vec3(0, 1, 0)));
	earth->transform(glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f) * dt, glm::vec3(0, 1, 0)));
	moon->transform(glm::rotate(glm::mat4(1.0f), glm::radians(60.0f) * dt, glm::vec3(0, 1, 0)));
}

void Planets::render()
{
	textureShader->use();
	usedCamera->uploadData(*textureShader);

	textureShader->setUniform("dirL.color", dirL.color);
	textureShader->setUniform("dirL.direction", dirL.direction);
	textureShader->setUniform("pointL.color", pointL.color);
	textureShader->setUniform("pointL.position", pointL.position);
	textureShader->setUniform("pointL.attenuation", pointL.attenuation);
	sun->draw();
}

Camera& Planets::getGameCamera()
{
	return this->gameCamera;
}

void Planets::setUsedCamera(Camera& usedCamera)
{
	this->usedCamera = &usedCamera;
}
