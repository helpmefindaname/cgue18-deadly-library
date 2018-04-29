#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

#include "../libimport/glew.h"
#include "vertex.h"

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

		this->createVAO(
			shader.getAttributeLocation("vertexPosition"),
			shader.getAttributeLocation("vertexNormal"),
			shader.getAttributeLocation("vertexUV")
		);

		this->inMemory = true;
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

			vertex.uv = {
				attributes.texcoords[2 * index.texcoord_index + 0],
				attributes.texcoords[2 * index.texcoord_index + 1]
			};

			if (uniqueVertices.count(vertex) == 0) {
				uniqueVertices[vertex] = (int)this->vertices.size();

				this->vertices.push_back(vertex.position);
				this->normals.push_back(vertex.normal);
				this->uvs.push_back(vertex.uv);
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