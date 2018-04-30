#include "Text2dWriter.h"
#include "../libimport/glm.h"
#include <vector>
#include "Mesh.h"


Text2dWriter::Text2dWriter() :
	font("assets/textures/font.png"),
	fontShader("assets/shader/textshader")
{
	glGenBuffers(1, &Text2DVertexBufferID);
	glGenBuffers(1, &Text2DUVBufferID);
}


Text2dWriter::~Text2dWriter()
{
}

void Text2dWriter::print(const char * text, int x, int y, int size) {

	unsigned int length = strlen(text);

	// Fill buffers
	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> UVs;
	for (unsigned int i = 0; i < length; i++) {

		glm::vec3 vertex_up_left = glm::vec3(x + i * size, y + size, 0);
		glm::vec3 vertex_up_right = glm::vec3(x + i * size + size, y + size, 0);
		glm::vec3 vertex_down_right = glm::vec3(x + i * size + size, y, 0);
		glm::vec3 vertex_down_left = glm::vec3(x + i * size, y, 0);

		indices.push_back(i * 6 + 0);
		indices.push_back(i * 6 + 1);
		indices.push_back(i * 6 + 2);
		indices.push_back(i * 6 + 3);
		indices.push_back(i * 6 + 4);
		indices.push_back(i * 6 + 5);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i];
		float uv_x = (character % 16) / 16.0f;
		float uv_y = (character / 16) / 16.0f;

		glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
		glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 16.0f, uv_y);
		glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 16.0f, (uv_y + 1.0f / 16.0f));
		glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / 16.0f));
		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_right);

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}

	Mesh drawMesh(indices, vertices, std::vector<glm::vec3>(), UVs);

	fontShader.use();

	font.bind(0);
	fontShader.setUniform("myTextureSampler", 0);


	drawMesh.uploadData(fontShader);

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	drawMesh.render();

	glDisable(GL_BLEND);

	drawMesh.deleteData();
}