#pragma once
#include <string>
#include <vector>
#include "../graphics/model/Geometry.h"
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
	std::shared_ptr<model::Geometry> createWorldGeometry();
	void createWorldPhysics(PhysicsPipeline& physiX);

private:
	void readFile();
};