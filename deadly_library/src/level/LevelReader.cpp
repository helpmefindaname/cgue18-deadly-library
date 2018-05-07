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
	std::shared_ptr<Texture> texture = TextureLoader::loadTexture("assets/textures/block.png");

	for (int i = 0; i < this->width; i++)
	{
		for (int j = 0; j < this->height; j++)
		{
			if (data[j][i] == 1) {
				scene->addChild(std::make_unique<model::Geometry>(cube, material, texture, glm::translate(glm::vec3(i - width / 2.0f, -2, -j))));
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
			if (data[j][i] == 1) {
				physiX.createStaticCube(PxVec3(i - width / 2.0f, -2.0f, (float)-j));
			}
		}
	}
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
	//1 -> piece at height 1

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