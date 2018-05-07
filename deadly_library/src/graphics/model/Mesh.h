#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "../../libimport/glm.h"
#include "../shader.h"
#include "Attributes.h"

class Mesh {
public:
	Mesh();
	Mesh(Mesh&& mesh);
	Mesh(std::string filepath);
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

public:
	void render();

	void uploadData(Shader& shader);
	void deleteData();

	void load(std::string filepath);

	float getCollisionRadius();

	static std::shared_ptr<Mesh> createCubeMesh(float width, float height, float depth);

private:
	void readFile();
	void copyData();
	void createVAO(GLint verticeLocation, GLint normalLocation, GLint uvLocation);
	void calculateCollisionRadius();
};