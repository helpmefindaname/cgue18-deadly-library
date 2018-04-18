#include "window.h"

#include "config.h"

Window::Window() {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_REFRESH_RATE, Config::getInt("RefreshRate"));
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);  // Create an OpenGL debug context 
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	GLFWmonitor* monitor = nullptr;
	if (Config::getBool("Fullscreen")) {
		monitor = glfwGetPrimaryMonitor();
	}

	this->window = glfwCreateWindow(
		Config::getInt("WindowWidth"),
		Config::getInt("WindowHeight"),
		Config::getString("Title").c_str(),
		monitor,
		nullptr);

	glfwMakeContextCurrent(this->window);
}

Window::~Window() {
	glfwDestroyWindow(this->window);
}

GLFWwindow* Window::getPointer() {
	return this->window;
}

void Window::swapBuffers() {
	glfwSwapBuffers(this->window);
}

bool Window::isClosed() {
	return 0 != glfwWindowShouldClose(this->window);
}

void Window::close() {
	glfwSetWindowShouldClose(window, true);
}

void Window::setTitle(std::string title) {
	glfwSetWindowTitle(this->window, title.c_str());
}