#pragma once

#include <string>
#include <unordered_map>
#include "libimport\glm.h"

class Config {
public:

	static void readFile(std::string file);

	static bool getBool(std::string value);
	static int getInt(std::string value);
	static float getFloat(std::string value);
	static std::string getString(std::string value);
	static glm::vec3 getVec3(std::string value);

private:
	static std::unordered_map<std::string, bool> boolValues;
	static std::unordered_map<std::string, int> intValues;
	static std::unordered_map<std::string, float> floatValues;
	static std::unordered_map<std::string, std::string> stringValues;
	static std::unordered_map<std::string, glm::vec3> vec3Values;
};