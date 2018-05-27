#pragma once
#include "../graphics/shader.h"
#include <memory>
#include "../graphics/model/Mesh.h"
class Light
{
public:
	Light(glm::vec3 position);
	~Light();


private:
	glm::vec3 position;

public:
	void render(Shader& shader);
	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();
};

