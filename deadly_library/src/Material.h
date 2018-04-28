/*
* Copyright 2017 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/
#pragma once


#include <memory>
#include <glm/glm.hpp>
#include "graphics/shader.h"

#include "graphics/texture.h"


/* --------------------------------------------- */
// Base material
/* --------------------------------------------- */

class Material
{
protected:
	std::shared_ptr<graphics::Shader> _shader;

	glm::vec3 _materialCoefficients; // x = ambient, y = diffuse, z = specular
	float _alpha;

public:
	Material(std::shared_ptr<graphics::Shader> shader, glm::vec3 materialCoefficients, float specularCoefficient);
	virtual ~Material();

	graphics::Shader* getShader();
	virtual void setUniforms();
};

/* --------------------------------------------- */
// Texture material
/* --------------------------------------------- */

class TextureMaterial : public Material
{
protected:
	std::shared_ptr<Texture> _diffuseTexture;

public:
	TextureMaterial(std::shared_ptr<graphics::Shader> shader, glm::vec3 materialCoefficients, float specularCoefficient, std::shared_ptr<Texture> diffuseTexture);
	virtual ~TextureMaterial();

	virtual void setUniforms();
};

