#pragma once

#include "../libimport/glew.h"
#include "../libimport/glm.h"
#include "shaderStage.h"

#include <vector>
#include <string>
#include <unordered_map>

class Shader {
public:
	Shader(std::string path);
	Shader(Shader&& shader);
	~Shader();

private:
	GLuint handle;
	std::string path;

	std::vector<ShaderStage> stages;
	std::unordered_map<std::string, GLint> attributes;
	std::unordered_map<std::string, GLint> uniforms;

public:
	void use();
	GLuint getHandle();
	std::string getName();
	GLint getAttributeLocation(std::string name);

	void setUniform(std::string name, const glm::mat4& matrix);
	void setUniform(std::string name, const glm::mat3& matrix);
	void setUniform(std::string name, const glm::vec3& vector);
	void setUniform(std::string name, const glm::vec2& vector);
	void setUniform(std::string name, const float val);
	void setUniform(std::string name, const int textureId);
	void setUniform(std::string name, std::vector<glm::vec3>& vector);
	//todo: add 'setUniform' methods if needed
	GLint getUniformLocation(std::string name);

private:
	void createStages();
	void attachStages();
	void detachStages();
	void link();

	bool checkIfExists(std::string file);
};
