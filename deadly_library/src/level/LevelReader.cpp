#include "LevelReader.h"
#include <fstream>
#include "../graphics/model/Mesh.h"
#include "../graphics/model/Material.h"
#include "../graphics/model/MaterialLoader.h"
#include "../graphics/texture.h"
#include "../graphics/TextureLoader.h"

LevelReader::LevelReader(std::string file)
	:fileName(file)
{
	this->readFile();
}


LevelReader::~LevelReader()
{
}

std::shared_ptr<model::Geometry> LevelReader::createWorldGeometry()
{
	std::shared_ptr<Mesh> cube = Mesh::createCubeMesh(1.0f, 1.0f, 1.0f);

	std::shared_ptr<model::Geometry> scene = std::make_shared<model::Geometry>();
	std::shared_ptr<Material> material = MaterialLoader::loadMaterial("assets/materials/block.material");
	std::shared_ptr<Material> wallMaterial = MaterialLoader::loadMaterial("assets/materials/wall.material");
	std::shared_ptr<Texture> texture = TextureLoader::loadTexture("assets/textures/block2.png");
	std::shared_ptr<Texture> wallTexture = TextureLoader::loadTexture("assets/textures/regal_color.jpg");
	std::shared_ptr<Texture> wallNormals = TextureLoader::loadTexture("assets/textures/regal_normals.jpg");
	std::shared_ptr<Texture> wallDepth = TextureLoader::loadTexture("assets/textures/regal_depth_gen.jpg");

	std::shared_ptr<Mesh> plane1 = Mesh::createPlaneMesh( //back
		glm::vec3(0.0f, 20.0f, 0.0f),
		glm::vec3(width + 1.0f, 0.0f, 0.0f));

	std::shared_ptr<Mesh> plane2 = Mesh::createPlaneMesh( //front
		glm::vec3(0.0f, 20.0f, 0.0f),
		glm::vec3(width + 1.0f, 0.0f, 0.0f));

	std::shared_ptr<Mesh> plane4 = Mesh::createPlaneMesh( //right
		glm::vec3(0.0f, 20.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, height + 1));

	std::shared_ptr<Mesh> plane5 = Mesh::createPlaneMesh( //left
		glm::vec3(0.0f, 20.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, height + 1));

	scene->addChild(std::make_unique<model::Geometry>(plane1, wallMaterial, wallTexture, wallNormals, wallDepth, glm::translate(glm::vec3(0.0f, 0.0f, 0.5f))));
	scene->addChild(std::make_unique<model::Geometry>(plane2, wallMaterial, wallTexture, wallNormals, wallDepth, glm::translate(glm::vec3(0.0f, 0.0f, 0.5f - height)) * glm::rotate(Glm::pi, glm::vec3(0.0f, 1.0f, 0.0f))));
	scene->addChild(std::make_unique<model::Geometry>(plane4, wallMaterial, wallTexture, wallNormals, wallDepth, glm::translate(glm::vec3(width / 2.0f - .5f, 0, (1.0f - height)*0.5f)) * glm::rotate(Glm::pi, glm::vec3(0.0f, 1.0f, 0.0f))));
	scene->addChild(std::make_unique<model::Geometry>(plane5, wallMaterial, wallTexture, wallNormals, wallDepth, glm::translate(glm::vec3(-width / 2.0f - .5f, 0, (1.0f - height)*0.5f))));

	for (int i = 0; i < this->width; i++)
	{
		for (int j = 0; j < this->height; j++)
		{
			if (data[j][i] >= 1 && data[j][i] <= 9) {
				int blockheight = data[j][i];
				scene->addChild(std::make_unique<model::Geometry>(cube, material, texture, nullptr, nullptr, glm::translate(glm::vec3(i - width / 2.0f, blockheight, -j))));
			}
		}
	}

	return scene;
}

void LevelReader::createWorldPhysics(PhysicsPipeline& physiX)
{
	for (int i = 0; i < this->width; i++)
	{
		for (int j = 0; j < this->height; j++)
		{
			if (data[j][i] >= 1 && data[j][i] <= 9) {
				float blockheight = (float)data[j][i];
				physiX.createStaticCube(PxVec3(i - width / 2.0f, blockheight, (float)-j));
			}
		}
	}

	physiX.createStaticCube(PxVec3(0.0f, 0.0f, .5f), PxVec3((float)width, 20.0f, .25f));
	physiX.createStaticCube(PxVec3(0.0f, 0.0f, 0.5f - height), PxVec3((float)width, 20.0f, .25f));

	physiX.createStaticCube(PxVec3(width / 2.0f - .5f, 0.0f, 0.0f), PxVec3(.5f, 20.0f, height - 1.0f));
	physiX.createStaticCube(PxVec3(-width / 2.0f - .5f, 0.0f, 0.0f), PxVec3(.5f, 20.0f, height - 1.0f));
}

void LevelReader::readFile() {
	std::ifstream file(fileName, std::ifstream::in);

	if (!file) {
		throw std::runtime_error("Cannot open file [" + fileName + "]");
	}

	file >> width;
	file >> height;

	//atm:
	//0 -> hole
	//1 -9 -> piece at height 1 until 9

	data = new int*[height];

	for (int i = 0; i < height; i++) {
		data[i] = new int[width];
		for (int j = 0; j < width; j++)
		{
			if (!(file >> data[i][j])) {
				throw std::runtime_error("not enough data in file  [" + fileName + "]");
			}
		}
	}
}

int LevelReader::getHeight() {
	return height;
}