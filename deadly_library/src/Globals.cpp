#include "Globals.h"

bool Globals::isDebug = false;
bool Globals::isWireFrameMode = false;
bool Globals::showFps = false;
bool Globals::isHelp = false;
bool Globals::useLightMap = true;
bool Globals::useNormalMap = true;
bool Globals::isCelShading = true;
bool Globals::isFrustumculling = true;
int Globals::fps = 60;
int Globals::subdivisionLevel = 1;
std::vector<glm::vec4> Globals::frustumPlanes;