#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "Mesh.h"

class MeshLoader
{
private:
	static std::unordered_map<std::string, std::shared_ptr<Mesh>> materials;

public:
	static std::shared_ptr<Mesh> loadMesh(std::string file);
	static void unloadAll();
};

