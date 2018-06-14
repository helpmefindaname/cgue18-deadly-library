#include "Material.h"
#include <fstream>
#include <sstream>
#include "../../config.h"
#include <stdexcept>
#include "../../Globals.h"

Material::Material() :
	color(1.0f),
	specularPower(1.0f),
	ambientFactor(0.1f),
	diffuseFactor(0.6f)
{ }

Material::Material(Material&& material) :
	filepath(material.filepath),
	color(std::move(material.color)),
	specularPower(material.specularPower),
	diffuseFactor(material.diffuseFactor),
	ambientFactor(material.ambientFactor)
{ }

Material::Material(std::string filepath) :
	filepath(filepath)
{
	this->readFile();
}

Material::~Material() {}

void Material::uploadData(Shader& shader) {

	shader.setUniform("materialColor", this->color);
	shader.setUniform("ambientFactor", this->ambientFactor);
	shader.setUniform("diffuseFactor", this->diffuseFactor);
	shader.setUniform("specularFactor", this->specularPower);
}

void Material::uploadLightData(Shader & shader)
{
}

bool Material::appliesToShader(Shader & shader)
{
	std::string shadername = shader.getName();
	if (shadername == "assets/shader/lightMapPass" && !useLightmapping) {
		return false;
	}
	if (shadername == "assets/shader/geometrypass" && useLightmapping && Globals::useLightMap) {
		return false;
	}
	if (shadername == "assets/shader/lightMapGeometryPass" && !(useLightmapping && Globals::useLightMap)) {
		return false;
	}
	return true;
}

glm::vec3 Material::getColor() {
	return this->color;
}

void Material::setColor(glm::vec3 color) {
	this->color = color;
}

void Material::readFile() {
	std::ifstream filestream(this->filepath, std::ifstream::in);

	if (!filestream) {
		throw std::runtime_error("Error loading material " + this->filepath);
	}

	std::string line;
	while (std::getline(filestream, line)) {
		if (line.empty()) continue;

		std::istringstream iss(line);

		std::string valueName;
		if (!(iss >> valueName)) { break; }

		if (valueName == "color") {
			float red, green, blue;
			if (!(iss >> red >> green >> blue)) { break; }
			this->color = glm::vec3(red, green, blue);
		}
		else if (valueName == "specularPower") {
			float value;
			if (!(iss >> value)) { break; }
			this->specularPower = value;
		}
		else if (valueName == "ambientFactor") {
			float value;
			if (!(iss >> value)) { break; }
			this->ambientFactor = value;
		}
		else if (valueName == "diffuseFactor") {
			float value;
			if (!(iss >> value)) { break; }
			this->diffuseFactor = value;
		}
		else if (valueName == "usesTexture") {
			float value;
			if (!(iss >> value)) { break; }
			this->usesTexture = value > 0.2f;
		}
		else if (valueName == "useLightmapping") {
			float value;
			if (!(iss >> value)) { break; }
			this->useLightmapping = value > 0.2f;
		}
	}

	filestream.close();
}