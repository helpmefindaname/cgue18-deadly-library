#include "HudWriter.h"
#include "../libimport/glm.h"
#include <vector>



HudWriter::HudWriter() :
	font("assets/textures/font.png"),
	fontShader("assets/shader/hudshader")
{
	glGenBuffers(1, &hudVertexBufferID);
	glGenBuffers(1, &hudUVBufferID);

	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> UVs;

	vertices.push_back(glm::vec3(0, 1.0f, 0));
	vertices.push_back(glm::vec3(1.0f, 1.0f, 0));
	vertices.push_back(glm::vec3(1.0f, 0, 0));
	vertices.push_back(glm::vec3(0.0f, 0, 0));

	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(0);

	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(2);

	UVs.push_back(glm::vec2(0.0f, 0.0f));
	UVs.push_back(glm::vec2(1.0f, 0.0f));
	UVs.push_back(glm::vec2(1.0f, 1.0f));
	UVs.push_back(glm::vec2(0.0f, 1.0f));

	textureMesh = std::make_unique<Mesh>(indices, vertices, std::vector<glm::vec3>(), UVs);
}


HudWriter::~HudWriter()
{
}

void HudWriter::print(const char * text, int x, int y, int size) {

	unsigned int length = strlen(text);

	std::vector<unsigned int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> UVs;

	for (unsigned int i = 0; i < length; i++) {

		glm::vec3 vertex_up_left = glm::vec3(x + i * size, y + size, 0);
		glm::vec3 vertex_up_right = glm::vec3(x + i * size + size, y + size, 0);
		glm::vec3 vertex_down_right = glm::vec3(x + i * size + size, y, 0);
		glm::vec3 vertex_down_left = glm::vec3(x + i * size, y, 0);

		indices.push_back(i * 4 + 0);
		indices.push_back(i * 4 + 1);
		indices.push_back(i * 4 + 2);
		indices.push_back(i * 4 + 3);
		indices.push_back(i * 4 + 2);
		indices.push_back(i * 4 + 1);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_right);

		char character = text[i];
		float uv_x = (character % 16) / 16.0f;
		float uv_y = (character / 16) / 16.0f;

		glm::vec2 uv_up_left(uv_x, uv_y);
		glm::vec2 uv_up_right(uv_x + 1.0f / 16.0f, uv_y);
		glm::vec2 uv_down_right(uv_x + 1.0f / 16.0f, uv_y + 1.0f / 16.0f);
		glm::vec2 uv_down_left(uv_x, uv_y + 1.0f / 16.0f);

		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_right);
	}

	Mesh drawMesh(indices, vertices, std::vector<glm::vec3>(), UVs);

	fontShader.use();

	font.bind(5);
	fontShader.setUniform("myTextureSampler", 5);
	fontShader.setUniform("transform", false);

	drawMesh.render(fontShader);

	drawMesh.deleteData();
}

void HudWriter::drawtexture(int x, int y, int width, int height, Texture & texture)
{

	fontShader.use();

	texture.bind(5);
	fontShader.setUniform("x", (float)x);
	fontShader.setUniform("y", (float)y);
	fontShader.setUniform("width", (float)width);
	fontShader.setUniform("height", (float)height);
	fontShader.setUniform("transform", true);
	fontShader.setUniform("myTextureSampler", 5);

	textureMesh->render(fontShader);
}
