#include "shaderStage.h"
#include <stdexcept>
#include <fstream>

ShaderStage::ShaderStage(std::string filepath, ShaderStageType type) :
	filepath(filepath),
	type(type)
{
	this->handle = glCreateShader(GLenum(type));
	if (this->handle == 0) {
		throw std::runtime_error("Failed to create shader of type " + GLenum(type));
	}

	this->readFile();
	this->compile();
}

ShaderStage::ShaderStage(ShaderStage&& shaderStage) :
	handle(shaderStage.handle),
	filepath(shaderStage.filepath),
	source(shaderStage.source),
	type(shaderStage.type)
{
	shaderStage.handle = 0;
}

ShaderStage::~ShaderStage() {
	if (this->handle != 0) {
		glDeleteShader(this->handle);
	}
}

GLuint ShaderStage::getHandle() {
	return this->handle;
}

ShaderStageType ShaderStage::getType() {
	return this->type;
}

std::string& ShaderStage::getSource() {
	return this->source;
}

void ShaderStage::readFile() {
	std::ifstream filestream(this->filepath, std::ifstream::in);

	this->source = std::string(std::istreambuf_iterator<char>(filestream), std::istreambuf_iterator<char>());

	filestream.close();

	if (this->source.size() == 0) {
		throw std::runtime_error("Failed to load shader source file from path '" + std::string(this->filepath) + "'");
	}
}

void ShaderStage::compile() {
	const GLchar* source = this->source.c_str();

	glShaderSource(this->handle, 1, &source, 0);
	glCompileShader(this->handle);

	GLint success = 0;
	glGetShaderiv(this->handle, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE || !glIsShader(this->handle)) {
		GLint logSize;
		glGetShaderiv(this->handle, GL_INFO_LOG_LENGTH, &logSize);

		if (logSize > 0) {
			std::vector<GLchar> log(logSize);
			glGetShaderInfoLog(this->handle, logSize, &logSize, &log[0]);

			throw std::runtime_error("Failed to compile shader stage'" + std::string(this->filepath) + "'\n" + std::string(&log[0]));
		}
		else {
			throw std::runtime_error("Failed to compile shader stage'" + std::string(this->filepath) + "'. Infolog not available.");
		}
	}
}