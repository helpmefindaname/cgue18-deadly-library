#pragma once
#include "../libimport/glm.h"
class Light
{
public:
	Light(glm::vec3 position);
	~Light();


private:
	glm::vec3 position;

public:
	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();
};

