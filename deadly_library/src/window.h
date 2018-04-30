#pragma once

#include "libimport\glfw.h"
#include <string>

class Window {
public:
	Window();
	~Window();

private:
	GLFWwindow* window;

public:
	GLFWwindow* getPointer();

	void swapBuffers();
	bool isClosed();
	void close();
	void setTitle(std::string title);
};