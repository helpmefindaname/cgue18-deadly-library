#include "Light.h"
#include "../config.h"
#include "../graphics/model/MeshLoader.h"

Light::Light(glm::vec3 position)
	: position(position)
{
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
	mesh->deleteData();
}

void Light::setPosition(glm::vec3 position) {
	this->position = position;
}

glm::vec3 Light::getPosition() {
	return this->position;
}