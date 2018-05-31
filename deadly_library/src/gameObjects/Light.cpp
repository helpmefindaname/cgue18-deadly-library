#include "Light.h"

Light::Light(glm::vec3 position)
	: position(position)
{
}


Light::~Light()
{
}

void Light::setPosition(glm::vec3 position) {
	this->position = position;
}

glm::vec3 Light::getPosition() {
	return this->position;
}