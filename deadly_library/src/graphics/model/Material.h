/*
* Copyright 2017 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/
#pragma once


#include <memory>
#include "../../libimport/glm.h"
#include "../shader.h"

class Material {
public:
	Material();
	Material(Material&& material);
	Material(std::string filepath);
	~Material();

private:
	std::string filepath;

	glm::vec3 color;

	float ambientFactor;
	float diffuseFactor;
	float specularPower;

	bool usesTexture;
	bool useLightmapping;

public:
	void uploadData(Shader& shader);
	void uploadLightData(Shader& shader);

	bool appliesToShader(Shader& shader);

	glm::vec3 getColor();
	void setColor(glm::vec3 color);

private:
	void readFile();
};

