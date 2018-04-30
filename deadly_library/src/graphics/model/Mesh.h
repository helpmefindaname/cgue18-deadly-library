#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#include "../../libimport/glm.h"
#include "../shader.h"
#include "Attributes.h"

class Mesh {
public:
	Mesh();
	Mesh(Mesh&& mesh);
	Mesh(std::string filepath, glm::mat4 modelMatrix = glm::mat4(1.0f));
	Mesh(std::vector<unsigned int> indices, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals = std::vector<glm::vec3>(), std::vector<glm::vec2> uvs = std::vector<glm::vec2>());
	~Mesh();

private:
	std::string filepath;

	GLuint vaoHandle;
	GLuint indicesBufferHandle;
	GLuint verticesBufferHandle;
	GLuint normalsBufferHandle;
	GLuint uvsBufferHandle;

	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;

	bool inMemory;

	float collisionRadius;

	Attributes position;

public:
	void render();

	void setTransformation(glm::mat4 matrix);

	void uploadData(Shader& shader);
	void deleteData();

	void load(std::string filepath);

	float getCollisionRadius();

private:
	void readFile();
	void copyData();
	void createVAO(GLint verticeLocation, GLint normalLocation, GLint uvLocation);
	void calculateCollisionRadius();
};