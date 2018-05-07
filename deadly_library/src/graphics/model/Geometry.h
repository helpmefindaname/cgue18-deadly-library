#pragma once
#include <vector>
#include <memory>
#include "Attributes.h"
#include "Mesh.h"
#include "Material.h"
#include "../shader.h"
#include "../texture.h"
namespace model {
	class Geometry
	{
	public:
		Geometry(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, std::shared_ptr<Texture> texture = NULL, glm::mat4 position=glm::mat4(1.0f));
		Geometry(glm::mat4 position = glm::mat4(1.0f));
		~Geometry();

	private:
		std::vector<std::unique_ptr<Geometry>> children;
		std::shared_ptr<Mesh> mesh;
		Attributes attributes;
		std::shared_ptr<Material> material;
		std::shared_ptr<Texture> texture;
		bool isEmpty;

	public:
		Geometry* addChild(std::unique_ptr<Geometry> child);
		void render(Shader& shader);
		void setTransformMatrix(glm::mat4 matrix);
	};
};