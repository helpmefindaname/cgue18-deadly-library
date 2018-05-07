#include "MeshLoader.h"

std::unordered_map<std::string, std::shared_ptr<Mesh>> MeshLoader::materials = std::unordered_map<std::string, std::shared_ptr<Mesh>>();

std::shared_ptr<Mesh> MeshLoader::loadMesh(std::string file)
{
	try {
		return materials.at(file);
	}
	catch (...) {
		materials.emplace(file, std::make_shared<Mesh>(file));
		return materials.at(file);
	}
}

void MeshLoader::unloadAll() {
	for each(auto val in materials) {
		val.second->deleteData();
		val.second.reset();
	}
	materials.clear();
}