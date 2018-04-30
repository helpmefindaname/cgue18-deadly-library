#ifndef ENGINE_PHYSICS_ATTRIBUTES
#define ENGINE_PHYSICS_ATTRIBUTES

#include "../../libimport/glm.h"
#include "../../libimport/glew.h"

class Attributes {
public:
	Attributes(glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0), glm::vec3 scale = glm::vec3(1));
	Attributes(glm::mat4 matrix);
	Attributes(Attributes&& attributes);
	~Attributes();

private:
	glm::mat4 matrix;
	Attributes* parentAttributes;

public:

	void setValues(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void setMatrix(glm::mat4 matrix);
	void setParentAttributes(Attributes* parentAttributes);

	void setPosition(glm::vec3 position);
	void scale(glm::vec3 scale);

	glm::vec3 getPosition();
	float getScale();

	glm::mat4 getParentMatrix();

	glm::mat4 getMatrix();
	glm::mat4 copyMatrix();

	Attributes clone();

	Attributes& translateForward(float value);
	Attributes& translateUp(float value);
	Attributes& translate(glm::vec3 translation);

	Attributes& rotateLeft(float degrees);
	Attributes& rotateRight(float degrees);

	Attributes& rotateUp(float degrees);
	Attributes& rotateBack(float degrees);

private:
};

#endif