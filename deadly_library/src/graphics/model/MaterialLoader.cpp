#include "MaterialLoader.h"

std::unordered_map<std::string, std::shared_ptr<Material>> materials = std::unordered_map<std::string, std::shared_ptr<Material>>();

std::shared_ptr<Material> MaterialLoader::loadMaterial(std::string file)
{
	try {
		return materials.at(file);
	}
	catch (...) {
		materials.emplace(file, std::make_shared<Material>(file));
		return materials.at(file);
	}
}

void MaterialLoader::unloadAll() {
	materials.clear();
}