#include "dL.h"
#include "../level/LevelReader.h"

DeadlyLibrary::DeadlyLibrary()
	:gameCamera(),
	usedCamera(&gameCamera),
	gBuffer(false, 0, 0, {}, {}, {}, {})
{}


DeadlyLibrary::~DeadlyLibrary()
{}

void DeadlyLibrary::init(PhysicsPipeline& physiX)
{
	LevelReader reader("assets\\1.lvl");
	this->textureShader = std::make_shared<Shader>("assets/shader/texture");
	this->blockTexture = std::make_shared<Texture>("assets/textures/block.png");
	this->blockMaterial = std::make_shared<TextureMaterial>(this->textureShader, glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, this->blockTexture);

	this->world = reader.createWorldGeometry(this->blockMaterial);
	reader.createWorldPhysics(physiX);
}

void DeadlyLibrary::update(InputHandler& input, float dt)
{
}

void DeadlyLibrary::render()
{
	this->textureShader->use();
	this->usedCamera->uploadData(*this->textureShader);

	textureShader->setUniform("dirL.color", glm::vec3(1.0f));
	textureShader->setUniform("dirL.direction", glm::vec3(1.0f, 0.0f, 0.0f));
	textureShader->setUniform("pointL.color", glm::vec3(1.0f));
	textureShader->setUniform("pointL.position", glm::vec3(1.0f));
	textureShader->setUniform("pointL.attenuation", glm::vec3(1.0f));


	this->world->draw();
}

Camera& DeadlyLibrary::getGameCamera()
{
	return this->gameCamera;
}

void DeadlyLibrary::setUsedCamera(Camera& usedCamera)
{
	this->usedCamera = &usedCamera;
}
