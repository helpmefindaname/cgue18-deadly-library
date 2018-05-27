#include "config.h"
#include <sstream>
#include <fstream>

std::unordered_map<std::string, bool> Config::boolValues;
std::unordered_map<std::string, int> Config::intValues;
std::unordered_map<std::string, float> Config::floatValues;
std::unordered_map<std::string, std::string> Config::stringValues;
std::unordered_map<std::string, glm::vec3> Config::vec3Values;

bool Config::getBool(std::string value) {
	return boolValues.at(value);
}

int Config::getInt(std::string value) {
	return intValues.at(value);
}

float Config::getFloat(std::string value) {
	return floatValues.at(value);
}

glm::vec3 Config::getVec3(std::string value) {
	return vec3Values.at(value);
}

std::string Config::getString(std::string value) {
	return stringValues.at(value);
}

void Config::readFile(std::string filepath) {
	std::ifstream filestream(filepath, std::ifstream::in);

	std::string line;
	while (std::getline(filestream, line)) {
		if (line.empty()) continue;

		std::istringstream iss(line);

		std::string valueName;
		if (line.front() == 'b') {
			bool value;
			if (!(iss >> valueName >> value)) { break; }
			boolValues[valueName.substr(1)] = value;
		}
		else if (line.front() == 'i') {
			int value;
			if (!(iss >> valueName >> value)) { break; }
			intValues[valueName.substr(1)] = value;
		}
		else if (line.front() == 'f') {
			float value;
			if (!(iss >> valueName >> value)) { break; }
			floatValues[valueName.substr(1)] = value;
		}
		else if (line.front() == 's') {
			std::string value;
			if (!(iss >> valueName >> value)) { break; }
			stringValues[valueName.substr(1)] = value;
		}
		else if (line.front() == 'v') {
			if (*(line.begin() + 1) == '3') {
				float value1, value2, value3;
				if (!(iss >> valueName >> value1 >> value2 >> value3)) { break; }
				vec3Values[valueName.substr(2)] = glm::vec3(value1, value2, value3);
			}
		}
	}

	filestream.close();
}
