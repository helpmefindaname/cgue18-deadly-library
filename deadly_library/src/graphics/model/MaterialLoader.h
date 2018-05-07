#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "Material.h"

class MaterialLoader
{
public:
	static std::shared_ptr<Material> loadMaterial(std::string file);
	static void unloadAll();
};

