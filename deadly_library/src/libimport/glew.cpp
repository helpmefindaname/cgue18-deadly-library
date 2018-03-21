#include "glew.h"
#include <stdexcept>
#include <iostream>

Glew::Glew() {
	if (refCount++ == 0) {
		glewExperimental = GL_TRUE;
		GLenum status;
		if ((status = glewInit()) != GLEW_OK) {
			std::cerr << glewGetErrorString(status);
			throw std::runtime_error("Failed to initialize glew! ");
		}
	}
}

Glew::~Glew() {
	refCount--;
}

int Glew::refCount = 0;