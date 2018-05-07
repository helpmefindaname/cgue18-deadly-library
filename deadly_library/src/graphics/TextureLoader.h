#pragma once
#include "texture.h"
#include <string>
#include <memory>

class TextureLoader
{
public:
	static std::shared_ptr<Texture> loadTexture(std::string file);
	static void unloadAll();
};

