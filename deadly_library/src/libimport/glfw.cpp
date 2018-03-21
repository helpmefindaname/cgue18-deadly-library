#include "glfw.h"
#include <stdexcept>

Glfw::Glfw() {
	if (refCount++ == 0) {
		if (glfwInit() != GLFW_TRUE) {
			throw std::runtime_error("Failed to initialize GLFW!");
		}
	}
}

Glfw::~Glfw() {
	if (--refCount == 0) {
		glfwTerminate();
	}
}

int Glfw::refCount = 0;