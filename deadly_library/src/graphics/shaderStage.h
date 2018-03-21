#pragma once

#include "../libimport/glew.h"
#include <vector>
#include <string>

enum class ShaderStageType : GLenum {
	VERTEX = GL_VERTEX_SHADER,
	FRAGMENT = GL_FRAGMENT_SHADER,
	GEOMETRY = GL_GEOMETRY_SHADER,
	COMPUTE = GL_COMPUTE_SHADER,
	TESS_CONTROL = GL_TESS_CONTROL_SHADER,
	TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
};

class ShaderStage {
public:
	ShaderStage(std::string filepath, ShaderStageType type);
	ShaderStage(ShaderStage&& shaderStage);
	~ShaderStage();

private:
	GLuint handle;
	std::string filepath;
	std::string source;
	ShaderStageType type;

public:
	GLuint getHandle();
	ShaderStageType getType();
	std::string& getSource();

private:
	void readFile();
	void compile();
};