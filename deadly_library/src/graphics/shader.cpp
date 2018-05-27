#include "shader.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <regex>

Shader::Shader(std::string path) :
	path(path),
	handle(0)
{
	this->createStages();

	this->handle = glCreateProgram();
	if (this->handle == 0) {
		throw std::runtime_error("Failed to create shader program!");
	}

	this->attachStages();
	this->link();
	this->detachStages();
}

Shader::Shader(Shader&& shader) :
	handle(shader.handle),
	path(shader.path),
	stages(std::move(shader.stages)),
	attributes(std::move(shader.attributes)),
	uniforms(std::move(shader.uniforms))
{
	shader.handle = 0;
}

Shader::~Shader() {
	if (this->handle != 0) {
		glDeleteProgram(this->handle);
	}
}

void Shader::use() {
	glUseProgram(this->handle);
}

GLuint Shader::getHandle() {
	return this->handle;
}

GLint Shader::getAttributeLocation(std::string name) {
	try {
		return this->attributes.at(name);
	}
	catch (...) {
		this->attributes.emplace(name, glGetAttribLocation(this->getHandle(), name.c_str()));
		return this->attributes.at(name);
	}
}

GLint Shader::getUniformLocation(std::string name) {
	try {
		return this->uniforms.at(name);
	}
	catch (...) {
		this->uniforms.emplace(name, glGetUniformLocation(this->getHandle(), name.c_str()));
		return this->uniforms.at(name);
	}
}

void Shader::createStages() {
	std::string filepath;

	filepath = this->path + std::string(".vert");
	if (this->checkIfExists(filepath.c_str())) {
		this->stages.push_back(ShaderStage(filepath.c_str(), ShaderStageType::VERTEX));
	}

	filepath = this->path + std::string(".frag");
	if (this->checkIfExists(filepath.c_str())) {
		this->stages.push_back(ShaderStage(filepath.c_str(), ShaderStageType::FRAGMENT));
	}

	filepath = this->path + std::string(".geom");
	if (this->checkIfExists(filepath.c_str())) {
		this->stages.push_back(ShaderStage(filepath.c_str(), ShaderStageType::GEOMETRY));
	}

	filepath = this->path + std::string(".comp");
	if (this->checkIfExists(filepath.c_str())) {
		this->stages.push_back(ShaderStage(filepath.c_str(), ShaderStageType::COMPUTE));
	}

	filepath = this->path + std::string(".teco");
	if (this->checkIfExists(filepath.c_str())) {
		this->stages.push_back(ShaderStage(filepath.c_str(), ShaderStageType::TESS_CONTROL));
	}

	filepath = this->path + std::string(".teev");
	if (this->checkIfExists(filepath.c_str())) {
		this->stages.push_back(ShaderStage(filepath.c_str(), ShaderStageType::TESS_EVALUATION));
	}

	if (this->stages.size() == 0) {
		throw std::runtime_error("No shader stages added to shader '" + std::string(this->path) + "'");
	}
}

void Shader::attachStages() {
	for (std::size_t i = 0; i < this->stages.size(); i++) {
		glAttachShader(this->getHandle(), this->stages.at(i).getHandle());
	}
}

void Shader::detachStages() {
	for (std::size_t i = 0; i < this->stages.size(); i++) {
		glDetachShader(this->getHandle(), this->stages.at(i).getHandle());
	}
}

void Shader::link() {
	glLinkProgram(this->getHandle());

	GLint success = 0;
	glGetProgramiv(this->handle, GL_LINK_STATUS, &success);
	if (success == GL_FALSE || !glIsProgram(this->handle)) {
		GLint logSize;
		glGetProgramiv(this->handle, GL_INFO_LOG_LENGTH, &logSize);

		if (logSize > 0) {
			std::vector<GLchar> log(logSize);
			glGetProgramInfoLog(this->handle, logSize, &logSize, &log[0]);

			throw std::runtime_error("Failed to link shader '" + std::string(this->path) + "'\n" + std::string(&log[0]));
		}
		else {
			throw std::runtime_error("Failed to link shader '" + std::string(this->path) + "'. Infolog not available.");
		}
	}
}

bool Shader::checkIfExists(std::string file) {
	std::ifstream stream(file);

	if (!stream) {
		return false;
	}
	else if (stream.is_open()) {
		stream.close();
	}

	return true;
}

void Shader::setUniform(std::string name, const glm::mat4& matrix) {
	glUniformMatrix4fv(this->getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setUniform(std::string name, const glm::mat3& matrix) {
	glUniformMatrix3fv(this->getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

void Shader::setUniform(std::string name, const glm::vec3& vector) {
	glUniform3f(this->getUniformLocation(name), vector.x, vector.y, vector.z);
}

void Shader::setUniform(std::string name, const glm::vec2& vector) {
	glUniform2f(this->getUniformLocation(name), vector.x, vector.y);
}

void Shader::setUniform(std::string name, float val) {
	glUniform1f(this->getUniformLocation(name), val);
}

void Shader::setUniform(std::string name, int textureId) {
	glUniform1i(this->getUniformLocation(name), textureId);
}

void Shader::setUniform(std::string name, std::vector<glm::vec3>& vector) {
	glUniform3fv(this->getUniformLocation(name), vector.size(), glm::value_ptr(vector[0]));
}

std::string Shader::getName() {
	return this->path;
}