#include "TextureLoader.h"
#include <unordered_map>
std::unordered_map<std::string, std::shared_ptr<Texture>> textures = std::unordered_map<std::string, std::shared_ptr<Texture>>();

std::shared_ptr<Texture> TextureLoader::loadTexture(std::string file)
{
	try {
		return textures.at(file);
	}
	catch (...) {
		textures.emplace(file, std::make_shared<Texture>(file));
		return textures.at(file);
	}
}

void TextureLoader::unloadAll() {
	textures.clear();
}