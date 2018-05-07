#include "Light.h"
#include "../config.h"
#include "../graphics/model/MeshLoader.h"

Light::Light(glm::vec3 position, float intensity, glm::vec3 color)
	: attenuationConstant(Config::getFloat("LightAttenuationConstant")),
	attenuationLinear(Config::getFloat("LightAttenuationLinear")),
	attenuationSquared(Config::getFloat("LightAttenuationSquared")),
	mesh(MeshLoader::loadMesh("assets/objects/lightBounds.mesh")),
	position(position),
	intensity(intensity),
	color(color)
{
	this->calculateRadius();
}


Light::~Light()
{
}

void Light::render(Shader & shader)
{
	shader.setUniform("lightRadius", this->lightRadius);
	shader.setUniform("attenuationConstant", this->attenuationConstant);
	shader.setUniform("attenuationLinear", this->attenuationLinear);
	shader.setUniform("attenuationSquared", this->attenuationSquared);
	shader.setUniform("lightColor", this->color);
	shader.setUniform("lightIntensity", this->intensity);
	shader.setUniform("lightPosition", this->position);
	shader.setUniform("modelMatrix", glm::translate(this->position) * glm::scale(glm::vec3(this->lightRadius)));

	mesh->uploadData(shader);
	mesh->render();
}

void Light::setPosition(glm::vec3 position) {
	this->position = position;
}

void Light::calculateRadius() {
	float maxChannel = std::fmaxf(std::fmaxf(this->color.r, this->color.g), this->color.b);
	const float threshold = 256.0f / 1.0f;

	float a = attenuationSquared;
	float b = attenuationLinear;
	float c = attenuationConstant - threshold * maxChannel * this->intensity;

	float d = b * b - 4.0f*a*c;

	this->lightRadius = (-b + std::sqrt(d)) / (2.0f*a);
}