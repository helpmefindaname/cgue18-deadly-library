#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

#include "../../libimport/glew.h"
#include "../vertex.h"

Mesh::Mesh() :
	inMemory(false)
{}

Mesh::Mesh(Mesh&& mesh) :
	filepath(mesh.filepath),
	inMemory(mesh.inMemory),
	vaoHandle(mesh.vaoHandle),
	indicesBufferHandle(mesh.indicesBufferHandle),
	verticesBufferHandle(mesh.verticesBufferHandle),
	normalsBufferHandle(mesh.normalsBufferHandle),
	uvsBufferHandle(mesh.uvsBufferHandle),
	indices(std::move(mesh.indices)),
	vertices(std::move(mesh.vertices)),
	normals(std::move(mesh.normals)),
	uvs(std::move(mesh.uvs)),
	collisionRadius(mesh.collisionRadius)
{
	mesh.inMemory = false;
}

Mesh::Mesh(std::string filepath) :
	filepath(filepath),
	inMemory(false)
{
	this->readFile();
}

Mesh::Mesh(
	std::vector<unsigned int> indices,
	std::vector<glm::vec3> vertices,
	std::vector<glm::vec3> normals,
	std::vector<glm::vec2> uvs
) :
	inMemory(false),
	indices(indices),
	vertices(vertices),
	normals(normals),
	uvs(uvs),
	filepath("gen")
{}

Mesh::~Mesh() {
	this->deleteData();
}

void Mesh::render() {
	if (!this->inMemory) {
		throw std::runtime_error("Mesh data is not in graphics memory");
	}

	glBindVertexArray(this->vaoHandle);

	glDrawElements(GL_TRIANGLES, (GLsizei)this->indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Mesh::uploadData(Shader& shader) {
	if (!this->inMemory) {
		this->copyData();
		this->inMemory = true;

		this->createVAO(
			shader.getAttributeLocation("vertexPosition"),
			shader.getAttributeLocation("vertexNormal"),
			shader.getAttributeLocation("vertexUV")
		);
	}
}

void Mesh::deleteData() {
	if (this->inMemory) {
		glDeleteBuffers(1, &this->indicesBufferHandle);
		glDeleteBuffers(1, &this->verticesBufferHandle);
		glDeleteBuffers(1, &this->normalsBufferHandle);
		glDeleteBuffers(1, &this->uvsBufferHandle);
		glDeleteVertexArrays(1, &this->vaoHandle);

		this->inMemory = false;
	}
}

void Mesh::load(std::string filepath) {
	this->filepath = filepath;
	this->readFile();
}

float Mesh::getCollisionRadius() {
	return this->collisionRadius;
}

std::shared_ptr<Mesh> Mesh::createCubeMesh(float width, float height, float depth)
{
	std::vector<glm::vec3> positions = {
		// front
		glm::vec3(-width / 2.0f, -height / 2.0f,  depth / 2.0f),
		glm::vec3(width / 2.0f, -height / 2.0f,  depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f,  depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f,  depth / 2.0f),
		// back
		glm::vec3(width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		glm::vec3(-width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f,  -depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f,  -depth / 2.0f),
		// right
		glm::vec3(width / 2.0f, -height / 2.0f,  depth / 2.0f),
		glm::vec3(width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f,  -depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f,  depth / 2.0f),
		// left
		glm::vec3(-width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		glm::vec3(-width / 2.0f, -height / 2.0f,  depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f,  depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f,  -depth / 2.0f),
		// top
		glm::vec3(-width / 2.0f, height / 2.0f,  -depth / 2.0f),
		glm::vec3(-width / 2.0f, height / 2.0f,  depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f,  depth / 2.0f),
		glm::vec3(width / 2.0f, height / 2.0f,  -depth / 2.0f),
		// bottom
		glm::vec3(-width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		glm::vec3(width / 2.0f, -height / 2.0f,  -depth / 2.0f),
		glm::vec3(width / 2.0f, -height / 2.0f,  depth / 2.0f),
		glm::vec3(-width / 2.0f, -height / 2.0f,  depth / 2.0f)
	};

	std::vector<glm::vec3> normals = {
		// front
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		// back
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		// right
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		// left
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		// top
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		// bottom
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0)
	};

	float widthOffset = 1.0f / 3.0f;
	float heightOffset = 1.0f / 2.0f;

	std::vector<glm::vec2> uvs = {
		// front
		glm::vec2(0, 0),
		glm::vec2(widthOffset, 0),
		glm::vec2(widthOffset, heightOffset),
		glm::vec2(0, heightOffset),
		// back
		glm::vec2(widthOffset, 0),
		glm::vec2(widthOffset * 2, 0),
		glm::vec2(widthOffset * 2, heightOffset),
		glm::vec2(widthOffset, heightOffset),
		// right
		glm::vec2(widthOffset * 2, 0),
		glm::vec2(widthOffset * 3, 0),
		glm::vec2(widthOffset * 3, heightOffset),
		glm::vec2(widthOffset * 2, heightOffset),
		// left
		glm::vec2(0, heightOffset),
		glm::vec2(widthOffset, heightOffset),
		glm::vec2(widthOffset, heightOffset * 2),
		glm::vec2(0, heightOffset * 2),
		// top
		glm::vec2(widthOffset, heightOffset),
		glm::vec2(widthOffset * 2, heightOffset),
		glm::vec2(widthOffset * 2, heightOffset * 2),
		glm::vec2(widthOffset, heightOffset * 2),
		// bottom
		glm::vec2(widthOffset * 2, heightOffset),
		glm::vec2(widthOffset * 3, heightOffset),
		glm::vec2(widthOffset * 3, heightOffset * 2),
		glm::vec2(widthOffset * 2, heightOffset * 2)
	};

	std::vector<unsigned int> indices = {
		// front
		0, 1, 2,
		2, 3, 0,
		// back
		4, 5, 6,
		6, 7, 4,
		// right
		8, 9, 10,
		10, 11, 8,
		// left
		12, 13, 14,
		14, 15, 12,
		// top
		16, 17, 18,
		18, 19, 16,
		// bottom
		20, 21, 22,
		22, 23, 20
	};

	return std::make_shared<Mesh>(indices, positions, normals, uvs);
}


std::shared_ptr<Mesh> Mesh::createPlaneMesh(glm::vec3 aa, glm::vec3 ab, glm::vec3 ba, glm::vec3 bb, glm::vec3 normal)
{
	std::vector<glm::vec3> positions = {
		aa,
		ab,
		ba,
		bb
	};

	std::vector<glm::vec3> normals = {
		// front
		normal,
		normal,
		normal,
		normal
	};

	std::vector<glm::vec2> uvs = {
		// front
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1)
	};

	std::vector<unsigned int> indices = {
		// front
		0, 1, 2,
		2, 3, 0,
	};

	return std::make_shared<Mesh>(indices, positions, normals, uvs);
}

void Mesh::readFile() {
	tinyobj::attrib_t attributes;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string error;

	if (!tinyobj::LoadObj(&attributes, &shapes, &materials, &error, this->filepath.c_str())) {
		throw std::runtime_error(error);
	}

	std::unordered_map<Vertex, int> uniqueVertices;

	for (const auto& shape : shapes) {
		for (const auto& index : shape.mesh.indices) {
			Vertex vertex;

			vertex.position = {
				attributes.vertices[3 * index.vertex_index + 0],
				attributes.vertices[3 * index.vertex_index + 1],
				attributes.vertices[3 * index.vertex_index + 2]
			};

			vertex.normal = {
				attributes.normals[3 * index.normal_index + 0],
				attributes.normals[3 * index.normal_index + 1],
				attributes.normals[3 * index.normal_index + 2]
			};

			if (index.texcoord_index != -1) {
				vertex.uv = {
					attributes.texcoords[2 * index.texcoord_index + 0],
					attributes.texcoords[2 * index.texcoord_index + 1]
				};
			}
			else {
				vertex.uv = { -1,-1 };
			}

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = (int)this->vertices.size();

				this->vertices.push_back(vertex.position);
				this->normals.push_back(vertex.normal);
				if (vertex.uv.x != -1) {
					this->uvs.push_back(vertex.uv);
				}
			}

			this->indices.push_back(uniqueVertices[vertex]);
		}
	}

	this->calculateCollisionRadius();
}

void Mesh::copyData() {
	// INDICES
	glGenBuffers(1, &this->indicesBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indicesBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// VERTICES
	glGenBuffers(1, &this->verticesBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, this->verticesBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(glm::vec3), &this->vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// NORMALS
	if (this->normals.size() > 0) {
		glGenBuffers(1, &this->normalsBufferHandle);
		glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(glm::vec3), &this->normals[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// UVS
	if (this->uvs.size() > 0) {
		glGenBuffers(1, &this->uvsBufferHandle);
		glBindBuffer(GL_ARRAY_BUFFER, this->uvsBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, this->uvs.size() * sizeof(glm::vec2), &this->uvs[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void Mesh::createVAO(GLint verticeLocation, GLint normalLocation, GLint uvLocation) {
	glGenVertexArrays(1, &this->vaoHandle);

	// Record gl commands
	glBindVertexArray(this->vaoHandle);

	// INDICES
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->indicesBufferHandle);

	// VERTICES
	glBindBuffer(GL_ARRAY_BUFFER, this->verticesBufferHandle);
	glEnableVertexAttribArray(verticeLocation);
	glVertexAttribPointer(verticeLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// NORMALS
	if (normalLocation != -1 && this->normals.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferHandle);
		glEnableVertexAttribArray(normalLocation);
		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// UVS
	if (uvLocation != -1 && this->uvs.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, this->uvsBufferHandle);
		glEnableVertexAttribArray(uvLocation);
		glVertexAttribPointer(uvLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// Stop recording
	glBindVertexArray(0);

	// Cleanup: unbind buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::calculateCollisionRadius() {
	if (this->vertices.size() > 0) {
		for (glm::vec3 vertex : this->vertices) {
			float distance = glm::length(vertex);
			if (distance > this->collisionRadius) {
				this->collisionRadius = distance;
			}
		}
	}
}