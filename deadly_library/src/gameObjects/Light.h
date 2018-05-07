#pragma once
#include "../graphics/shader.h"
#include <memory>
#include "../graphics/model/Mesh.h"
class Light
{
public:
	Light(glm::vec3 position, float intensity, glm::vec3 color = glm::vec3(1.0f));
	~Light();


private:
	float attenuationConstant;
	float attenuationLinear;
	float attenuationSquared;
	glm::vec3 color;
	float intensity;
	glm::vec3 position;

	float lightRadius;

	std::shared_ptr<Mesh> mesh;
public:
	void render(Shader& shader);
	void setPosition(glm::vec3 position);

private:
	void calculateRadius();
};

