#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "../../libimport/glm.h"
#include "../shader.h"
#include "Attributes.h"

struct pairhash {
public:
	template <typename T, typename U>
	std::size_t operator()(const std::pair<T, U> &x) const
	{
		return std::hash<T>()(x.first) ^ std::hash<U>()(x.second);
	}
};

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
	std::unordered_map<std::pair<glm::vec3, glm::vec3>, std::vector<int>, pairhash> edges;

	std::vector<unsigned int> renderIndices;
	std::vector<glm::vec3> renderVertices;
	std::vector<glm::vec3> renderNormals;
	std::vector<glm::vec2> renderUvs;
	std::unordered_map<std::pair<glm::vec3, glm::vec3>, std::vector<int>, pairhash> renderEdges;

	bool inMemory;
	bool isSubdivision;
	bool lastRenderSubdivision;

	float collisionRadius;

public:
	void prepareSubdivision();
	void render(Shader& shader);

	void deleteData();

	void load(std::string filepath);

	float getCollisionRadius();

	static std::shared_ptr<Mesh> createCubeMesh(float width, float height, float depth);
	static std::shared_ptr<Mesh> createPlaneMesh(glm::vec3 aa, glm::vec3 ab, glm::vec3 ba, glm::vec3 bb, glm::vec3 normal);

private:
	void uploadData(Shader& shader);
	void readFile();
	void copyData();
	void createVAO(GLint verticeLocation, GLint normalLocation, GLint uvLocation);
	void calculateCollisionRadius();
	void addEdge(std::unordered_map<std::pair<glm::vec3, glm::vec3>, std::vector<int>, pairhash>& edges, int i1, int i2, int i3);
	void applySubdivision();
};