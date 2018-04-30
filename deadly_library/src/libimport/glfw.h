#pragma once
#include "glew.h"
#include <glfw/glfw3.h>

class Glfw {
public:
	Glfw();
	~Glfw();

private:
	static int refCount;
};