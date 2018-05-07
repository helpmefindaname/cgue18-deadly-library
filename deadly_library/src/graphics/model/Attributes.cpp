#include "Attributes.h"

Attributes::Attributes(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) :
	parentAttributes(nullptr)
{
	setValues(position, rotation, scale);
}

Attributes::Attributes(glm::mat4 matrix) :
	matrix(std::move(matrix)),
	parentAttributes(nullptr)
{}

Attributes::Attributes(Attributes&& attributes) :
	matrix(std::move(attributes.matrix)),
	parentAttributes(attributes.parentAttributes)
{}

Attributes::~Attributes() {}

void Attributes::setValues(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
	this->matrix = glm::translate(position) * glm::toMat4(glm::quat(rotation)) * glm::scale(scale);
}

void Attributes::setMatrix(glm::mat4 matrix) {
	this->matrix = matrix;
}

void Attributes::setParentAttributes(Attributes* parentAttributes) {
	this->parentAttributes = parentAttributes;
}

void Attributes::setPosition(glm::vec3 position) {
	matrix[3] = glm::vec4(position, 1.0f);
}

void Attributes::scale(glm::vec3 scale) {
	matrix[0] *= scale.x;
	matrix[1] *= scale.y;
	matrix[2] *= scale.z;
}

glm::vec3 Attributes::getPosition() {
	return getParentMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
}

float Attributes::getScale() {
	glm::mat4 matrix = this->getParentMatrix();
	return (glm::length(matrix[0]) + glm::length(matrix[1]) + glm::length(matrix[2])) / 3.0f;
}

glm::mat4 Attributes::getParentMatrix() {
	if (this->parentAttributes) {
		return (this->parentAttributes->getParentMatrix() * this->matrix);
	}
	else {
		return this->matrix;
	}
}

glm::mat4 Attributes::getMatrix() {
	return this->matrix;
}

glm::mat4 Attributes::copyMatrix() {
	return glm::mat4(this->matrix);
}

Attributes Attributes::clone() {
	return Attributes(this->matrix);
}

Attributes& Attributes::translateForward(float value) {
	return translate(glm::vec3(0.0f, 0.0f, value));
}

Attributes& Attributes::translateUp(float value) {
	return translate(glm::vec3(0.0f, value, 0.0f));
}

Attributes& Attributes::translate(glm::vec3 translation) {
	matrix = matrix * glm::translate(glm::mat4(), translation);
	return *this;
}

Attributes& Attributes::rotateLeft(float degrees) {
	return rotateRight(-degrees);
}

Attributes& Attributes::rotateRight(float degrees) {
	matrix = matrix * glm::rotate(glm::mat4(), glm::radians(-degrees), glm::vec3(0.0f, 1.0f, 0.0f));
	return *this;
}

Attributes& Attributes::rotateUp(float degrees) {
	matrix = matrix * glm::rotate(glm::mat4(), glm::radians(-degrees), glm::vec3(1.0f, 0.0f, 0.0f));
	return *this;
}

Attributes& Attributes::rotateBack(float degrees) {
	matrix = matrix * glm::rotate(glm::mat4(), glm::radians(-degrees), glm::vec3(0.0f, 0.0f, 1.0f));
	return *this;
}