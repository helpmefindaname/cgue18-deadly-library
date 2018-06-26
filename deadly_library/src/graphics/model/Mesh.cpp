#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

#include "../../libimport/glew.h"
#include "../vertex.h"
#include "../../Globals.h"
#include <exception>

Mesh::Mesh() :
	inMemory(false),
	lastRenderSubdivision(false),
	isSubdivision(false)
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
	collisionRadius(mesh.collisionRadius),
	isSubdivision(mesh.isSubdivision),
	lastRenderSubdivision(mesh.lastRenderSubdivision)
{
	mesh.inMemory = false;
}

Mesh::Mesh(std::string filepath) :
	filepath(filepath),
	inMemory(false),
	lastRenderSubdivision(false),
	isSubdivision(false)
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
	filepath("gen"),
	isSubdivision(false),
	lastRenderSubdivision(false)
{
	this->calculateCollisionRadius();
}

Mesh::~Mesh() {
	this->deleteData();
}

void Mesh::render(Shader& shader) {

	if (this->isSubdivision && Globals::subdivisionLevel > 0) {
		this->deleteData();
		lastRenderSubdivision = true;
		renderIndices = std::vector<unsigned int>(indices);
		renderVertices = std::vector<glm::vec3>(vertices);
		renderNormals = std::vector<glm::vec3>(normals);
		renderUvs = std::vector<glm::vec2>(uvs);
		renderEdges = std::unordered_map<std::pair<glm::vec3, glm::vec3>, std::vector<int>, pairhash>(edges);
		for (int i = 0; i < Globals::subdivisionLevel; i++)
		{
			this->applySubdivision();
		}
	}
	else {
		if (lastRenderSubdivision) {
			this->deleteData();
		}
		lastRenderSubdivision = false;
		renderIndices = std::vector<unsigned int>(indices);
		renderVertices = std::vector<glm::vec3>(vertices);
		renderNormals = std::vector<glm::vec3>(normals);
		renderUvs = std::vector<glm::vec2>(uvs);
	}

	this->uploadData(shader);

	glBindVertexArray(this->vaoHandle);

	glDrawElements(GL_TRIANGLES, (GLsizei)this->renderIndices.size(), GL_UNSIGNED_INT, 0);

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


std::shared_ptr<Mesh> Mesh::createPlaneMesh(glm::vec3 height, glm::vec3 width)
{
	glm::vec3 aa = (-width + height) * 0.5f;
	glm::vec3 ab = (+width + height) * 0.5f;
	glm::vec3 ba = (+width - height) * 0.5f;
	glm::vec3 bb = (-width - height) * 0.5f;
	glm::vec3 normal = glm::normalize(glm::cross(width, height));
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
		1, 0, 2,
		3, 2, 0,
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->renderIndices.size() * sizeof(unsigned int), &this->renderIndices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// VERTICES
	glGenBuffers(1, &this->verticesBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, this->verticesBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, this->renderVertices.size() * sizeof(glm::vec3), &this->renderVertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// NORMALS
	if (this->normals.size() > 0) {
		glGenBuffers(1, &this->normalsBufferHandle);
		glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, this->renderNormals.size() * sizeof(glm::vec3), &this->renderNormals[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// UVS
	if (this->uvs.size() > 0) {
		glGenBuffers(1, &this->uvsBufferHandle);
		glBindBuffer(GL_ARRAY_BUFFER, this->uvsBufferHandle);
		glBufferData(GL_ARRAY_BUFFER, this->renderUvs.size() * sizeof(glm::vec2), &this->renderUvs[0], GL_STATIC_DRAW);
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
	if (normalLocation != -1 && this->renderNormals.size() > 0) {
		glBindBuffer(GL_ARRAY_BUFFER, this->normalsBufferHandle);
		glEnableVertexAttribArray(normalLocation);
		glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	// UVS
	if (uvLocation != -1 && this->renderUvs.size() > 0) {
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
	this->collisionRadius = 0.0f;
	if (this->vertices.size() > 0) {
		for (glm::vec3 vertex : this->vertices) {
			float distance = glm::length(vertex);
			if (distance > this->collisionRadius) {
				this->collisionRadius = distance;
			}
		}
	}
}

void Mesh::prepareSubdivision() {
	isSubdivision = true;
	renderIndices = std::vector<unsigned int>(indices);
	renderVertices = std::vector<glm::vec3>(vertices);
	renderNormals = std::vector<glm::vec3>(normals);
	renderUvs = std::vector<glm::vec2>(uvs);
	for (size_t i = 0; i < indices.size(); i += 3)
	{
		addEdge(edges, indices[i], indices[i + 1], i + 2);
		addEdge(edges, indices[i + 1], indices[i + 2], i);
		addEdge(edges, indices[i + 2], indices[i], i + 1);
	}

	std::vector<std::pair<glm::vec3, glm::vec3>> nonDoubles;

	for each (auto edge in edges)
	{
		if (edge.second.size() != 2) {
			nonDoubles.push_back(edge.first);
		}
	}

	if (nonDoubles.size() != 0) {
		throw std::exception("invalid mesh for subdivision surfaces. Is it closed and double triangles removed?");
	}
}

void Mesh::addEdge(std::unordered_map<std::pair<glm::vec3, glm::vec3>, std::vector<int>, pairhash>& edges, int i1, int i2, int i3) {
	if (std::hash<glm::vec3>()(renderVertices[i1]) > std::hash<glm::vec3>()(renderVertices[i2])) {
		i1 ^= i2;
		i2 ^= i1;
		i1 ^= i2;
	}

	std::pair<glm::vec3, glm::vec3> index(renderVertices[i1], renderVertices[i2]);

	edges.try_emplace(index, std::vector<int>()).first->second.push_back(i3);
}

void Mesh::applySubdivision() {
	std::vector<glm::vec3> newVertices(renderVertices);
	std::vector<glm::vec3> newNormals(renderNormals);
	std::vector<glm::vec2> newUvs(renderUvs);
	std::vector<unsigned int> newIndices;
	if (renderEdges.size() == 0) {
		for (size_t i = 0; i < renderIndices.size(); i += 3)
		{
			addEdge(renderEdges, renderIndices[i], renderIndices[i + 1], i + 2);
			addEdge(renderEdges, renderIndices[i + 1], renderIndices[i + 2], i);
			addEdge(renderEdges, renderIndices[i + 2], renderIndices[i], i + 1);
		}
	}

	std::vector<int> edgeVertices(renderIndices.size(), -1);
	int oldEnd = newVertices.size();
	std::unordered_map<glm::vec3, std::vector<int>> realVertices;
	std::unordered_map<int, std::vector<int>> rVerteicesNeighbours;

	for (size_t i = 0; i < renderVertices.size(); i++)
	{
		realVertices.try_emplace(renderVertices[i], std::vector<int>())
			.first->second
			.push_back(i);
	}

	for each (auto edge in renderEdges)
	{
		glm::vec3 vertex =
			edge.first.first * (3 / 8.0f) +
			edge.first.second * (3 / 8.0f) +
			renderVertices[renderIndices[edge.second[0]]] * (1 / 8.0f) +
			renderVertices[renderIndices[edge.second[1]]] * (1 / 8.0f);

		int start1 = (edge.second[0] / 3) * 3;
		int a1 = renderIndices[(edge.second[0] + 1) % 3 + start1];
		int b1 = renderIndices[(edge.second[0] + 2) % 3 + start1];

		glm::vec3 normal1 = glm::normalize(renderNormals[a1] + renderNormals[b1]);
		glm::vec2 uv1 = (renderUvs[a1] + renderUvs[b1]) * 0.5f;

		edgeVertices[edge.second[0]] = newVertices.size();
		newVertices.push_back(vertex);
		newNormals.push_back(normal1);
		newUvs.push_back(uv1);

		int start2 = (edge.second[1] / 3) * 3;
		int a2 = renderIndices[(edge.second[1] + 1) % 3 + start2];
		int b2 = renderIndices[(edge.second[1] + 2) % 3 + start2];

		glm::vec3 normal2 = glm::normalize(renderNormals[a2] + renderNormals[b2]);
		glm::vec2 uv2 = (renderUvs[a2] + renderUvs[b2]) * 0.5f;

		edgeVertices[edge.second[1]] = newVertices.size();
		newVertices.push_back(vertex);
		newNormals.push_back(normal2);
		newUvs.push_back(uv2);
	}

	for (size_t i = 0; i < renderIndices.size(); i += 3)
	{
		newIndices.push_back(edgeVertices[i]);
		newIndices.push_back(edgeVertices[i + 1]);
		newIndices.push_back(edgeVertices[i + 2]);

		newIndices.push_back(renderIndices[i]);
		newIndices.push_back(edgeVertices[i + 2]);
		newIndices.push_back(edgeVertices[i + 1]);
		rVerteicesNeighbours.try_emplace(renderIndices[i], std::vector<int>());
		rVerteicesNeighbours[renderIndices[i]].push_back(edgeVertices[i + 2]);
		rVerteicesNeighbours[renderIndices[i]].push_back(edgeVertices[i + 1]);

		newIndices.push_back(edgeVertices[i]);
		newIndices.push_back(edgeVertices[i + 2]);
		newIndices.push_back(renderIndices[i + 1]);
		rVerteicesNeighbours.try_emplace(renderIndices[i + 1], std::vector<int>());
		rVerteicesNeighbours[renderIndices[i + 1]].push_back(edgeVertices[i + 2]);
		rVerteicesNeighbours[renderIndices[i + 1]].push_back(edgeVertices[i]);

		newIndices.push_back(edgeVertices[i]);
		newIndices.push_back(renderIndices[i + 2]);
		newIndices.push_back(edgeVertices[i + 1]);
		rVerteicesNeighbours.try_emplace(renderIndices[i + 2], std::vector<int>());
		rVerteicesNeighbours[renderIndices[i + 2]].push_back(edgeVertices[i]);
		rVerteicesNeighbours[renderIndices[i + 2]].push_back(edgeVertices[i + 1]);
	}

	for each (auto vertices in realVertices)
	{
		int count = 0;
		glm::vec3 sum = glm::vec3(0.0f);
		for (size_t i = 0; i < vertices.second.size(); i++)
		{
			for (size_t j = 0; j < rVerteicesNeighbours[vertices.second[i]].size(); j++)
			{
				sum += newVertices[rVerteicesNeighbours[vertices.second[i]][j]];
				count++;
			}
		}
		float a = (3 / 8.0f + cosf(2 * 3.14159265f / count) / 4.0f);
		float alpha = (a * a + 3 / 8.f);
		if (count == 3) {
			alpha = 7.0f / 16.0f;
		}
		else {
			alpha = 5.0f / 8.0f;
		}

		float betta = (1.0f - alpha) / count;

		glm::vec3 result = vertices.first * alpha + betta * sum;

		for (size_t i = 0; i < vertices.second.size(); i++)
		{
			newVertices[vertices.second[i]] = result;
		}
	}

	renderIndices = newIndices;
	renderVertices = newVertices;
	renderUvs = newUvs;
	renderNormals = newNormals;
	renderEdges = std::unordered_map<std::pair<glm::vec3, glm::vec3>, std::vector<int>, pairhash>();
}

