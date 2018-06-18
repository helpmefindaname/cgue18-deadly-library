#include "Geometry.h"

namespace model {
	Geometry::Geometry(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, std::shared_ptr<Texture> texture, glm::mat4 position)
		: attributes(position),
		mesh(mesh),
		material(material),
		texture(texture),
		isEmpty(false),
		lightMap(NULL)
	{
	}

	Geometry::Geometry(glm::mat4 position)
		: attributes(position),
		isEmpty(true)
	{
	}


	Geometry::~Geometry()
	{
	}

	void Geometry::subdivide()
	{
		this->mesh->prepareSubdivision();
	}

	Geometry* Geometry::addChild(std::unique_ptr<Geometry> child)
	{
		children.push_back(std::move(child));
		return (children.end() - 1)->get();
	}

	void Geometry::render(Shader& shader)
	{
		if (!isEmpty) {
			if (material->appliesToShader(shader)) {
				shader.setUniform("modelMatrix", this->attributes.getParentMatrix());
				material->uploadData(shader);
				if (texture) {
					shader.setUniform("useTexture", true);
					texture->bind(10);
					shader.setUniform("textureBuffer", 10);
				}
				else {
					shader.setUniform("useTexture", false);
				}

				if (lightMap) {
					lightMap->bind(8);
					shader.setUniform("lightMapBuffer", 8);
				}
				mesh->render(shader);
			}
		}

		for (int i = 0; i < (int)children.size(); i++)
		{
			children[i]->render(shader);
		}
	}

	void Geometry::setTransformMatrix(glm::mat4 matrix) {
		this->attributes.setMatrix(matrix);
	}
	void Geometry::generateLightmap(Shader & shader, Framebuffer& framebuffer)
	{
		if (!isEmpty) {
			if (material->appliesToShader(shader)) {

				shader.setUniform("modelMatrix", this->attributes.getParentMatrix());
				material->uploadData(shader);
				if (texture) {
					shader.setUniform("useTexture", true);
					texture->bind(12);
					shader.setUniform("textureBuffer", 12);
				}
				else {
					shader.setUniform("useTexture", false);
				}
				mesh->render(shader);

				lightMap = framebuffer.createScreenShot("lightMap");
			}
		}

		for (int i = 0; i < (int)children.size(); i++)
		{
			children[i]->generateLightmap(shader, framebuffer);
		}
	}
};