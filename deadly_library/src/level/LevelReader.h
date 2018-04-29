#pragma once
#include <string>
#include <vector>
#include "../Geometry.h"
#include "../physics/PhysicsPipeline.h"
class LevelReader
{
public:
	LevelReader(std::string file);
	~LevelReader();

private:
	std::string fileName;
	int width, height;

	int** data;

public:
	std::shared_ptr<Geometry> createWorldGeometry(std::shared_ptr<Material> material);
	void createWorldPhysics(PhysicsPipeline& physiX);

private:
	void readFile();
};