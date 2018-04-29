#include "LevelReader.h"
#include <fstream>

LevelReader::LevelReader(std::string file)
	:fileName(file)
{
	this->readFile();
}


LevelReader::~LevelReader()
{
}

std::shared_ptr<Geometry> LevelReader::createWorldGeometry(std::shared_ptr<Material> material)
{
	GeometryData cube = Geometry::createCubeGeometry(1.0f, 1.0f, 1.0f);
	std::shared_ptr<Geometry> scene = std::make_shared<Geometry>();

	for (int i = 0; i < this->width; i++)
	{
		for (int j = 0; j < this->height; j++)
		{
			if (data[j][i] == 1) {
				scene->addChild(std::make_unique<Geometry>(glm::translate(glm::vec3(i - width / 2.0f, -2, -j)), cube, material));
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
				physiX.createStaticCube(PxVec3(i - width / 2.0f, -2, -j));
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