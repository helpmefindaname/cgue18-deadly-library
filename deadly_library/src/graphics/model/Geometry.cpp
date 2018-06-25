#include "Geometry.h"
#include "../../Globals.h"

namespace model {
	Geometry::Geometry(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material, std::shared_ptr<Texture> texture, std::shared_ptr<Texture> normalMap, std::shared_ptr<Texture> depthMap, glm::mat4 position)
		: attributes(position),
		mesh(mesh),
		material(material),
		texture(texture),
		isEmpty(false),
		normalMap(normalMap),
		depthMap(depthMap),
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
		child->attributes.setParentAttributes(&this->attributes);
		children.push_back(std::move(child));
		return (children.end() - 1)->get();
	}

	void Geometry::render(Shader& shader)
	{
		for (int i = 0; i < (int)children.size(); i++)
		{
			children[i]->render(shader);
		}
		if (isEmpty) {
			return;
		}
		if (!material->appliesToShader(shader)) {
			return;
		}
		if (Globals::isFrustumculling) {
			glm::vec3& position = this->attributes.getPosition();
			float radius = this->mesh->getCollisionRadius();
			std::vector<glm::vec4> frustumPlanes = Globals::frustumPlanes;

			for (int i = 0; i < 6; i++) {
				float distance = frustumPlanes[i].x * position.x + frustumPlanes[i].y * position.y + frustumPlanes[i].z * position.z + frustumPlanes[i].w;
				if (distance < -radius - 2.0f) {
					return;
				}
			}
		}

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

		if (normalMap) {
			normalMap->bind(9);
			shader.setUniform("normalMapBuffer", 9);
			depthMap->bind(11);
			shader.setUniform("depthMapBuffer", 11);
		}
		mesh->render(shader);
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
				if (normalMap) {
					shader.setUniform("useNormals", true);
					normalMap->bind(9);
					shader.setUniform("normalMapBuffer", 9);
					depthMap->bind(11);
					shader.setUniform("depthMapBuffer", 11);
				}
				else {
					shader.setUniform("useNormals", false);
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