#pragma once

#include <glfw/glfw3.h>

class Glfw {
public:
	Glfw();
	~Glfw();

private:
	static int refCount;
};