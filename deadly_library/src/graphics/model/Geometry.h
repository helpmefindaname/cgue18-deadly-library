#pragma once
#include <vector>
#include <memory>
#include "Attributes.h"
#include "Mesh.h"
#include "Material.h"
#include "../shader.h"
#include "../texture.h"
#include "../Framebuffer.h"
namespace model {
	class Geometry
	{
	public:
		Geometry(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, std::shared_ptr<Texture> texture = NULL, std::shared_ptr<Texture> normalMap = nullptr, std::shared_ptr<Texture> depthMap = nullptr, glm::mat4 position = glm::mat4(1.0f));
		Geometry(glm::mat4 position = glm::mat4(1.0f));
		~Geometry();

	private:
		std::vector<std::unique_ptr<Geometry>> children;
		std::shared_ptr<Mesh> mesh;
		Attributes attributes;
		std::shared_ptr<Material> material;
		std::shared_ptr<Texture> texture;
		std::shared_ptr<Texture> normalMap;
		std::shared_ptr<Texture> depthMap;
		bool isEmpty;
		std::shared_ptr<Texture> lightMap;

	public:
		void subdivide();
		Geometry* addChild(std::unique_ptr<Geometry> child);
		void render(Shader& shader);
		void setTransformMatrix(glm::mat4 matrix);
		void generateLightmap(Shader& shader, Framebuffer& framebuffer);
	};
};