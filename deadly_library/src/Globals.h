#pragma once
#include "libimport\glm.h"
#include <vector>
class Globals
{
public:
	static bool isHelp;
	static bool isDebug;
	static bool isWireFrameMode;
	static bool showFps;
	static bool useLightMap;
	static bool useNormalMap;
	static bool isCelShading;
	static bool isFrustumculling;
	static int fps;
	static int subdivisionLevel;
	static std::vector<glm::vec4> frustumPlanes;

};

