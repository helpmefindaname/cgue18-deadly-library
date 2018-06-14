#include "inputHandler.h"

InputHandler::InputHandler(Window& window) :
	window(window),
	mouseMovement(0)
{
	glfwSetInputMode(this->window.getPointer(), GLFW_STICKY_KEYS, GLFW_TRUE);
	glfwSetInputMode(this->window.getPointer(), GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

	// Setup Events
	this->events["cursorCaptured"] = false;

	this->events["cameraMoveForward"] = false;
	this->events["cameraMoveBackward"] = false;
	this->events["cameraMoveLeft"] = false;
	this->events["cameraMoveRight"] = false;
	this->events["cameraMoveUp"] = false;
	this->events["cameraMoveDown"] = false;

	this->events["pauseGame"] = false;
}

InputHandler::~InputHandler() {}

void InputHandler::process() {
	glfwPollEvents();

	keyState.swap(newKeyState);

	this->events["pauseGame"] = false;


	if (this->events["cursorCaptured"]) {
		double x, y;
		glfwGetCursorPos(this->window.getPointer(), &x, &y);
		glfwSetInputMode(this->window.getPointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwSetCursorPos(this->window.getPointer(), 0, 0);
		this->mouseMovement.x = (float)x;
		this->mouseMovement.y = (float)y;
	}
	else {
		this->mouseMovement.x = 0.0f;
		this->mouseMovement.y = 0.0f;
		glfwSetInputMode(this->window.getPointer(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	this->onKeyPress(GLFW_KEY_TAB, [this]() {
		this->events["cursorCaptured"] = !this->events["cursorCaptured"];
		glfwSetCursorPos(this->window.getPointer(), 0, 0);
		this->mouseMovement.x = 0.0f;
		this->mouseMovement.y = 0.0f;
	});
	
	this->onPressedEvent(GLFW_KEY_ENTER, "next");
	this->onPressedEvent(GLFW_KEY_ESCAPE, "close");
	this->onPressedEvent(GLFW_KEY_F1, "toggleHelp");
	this->onPressedEvent(GLFW_KEY_F2, "toggleFps");
	this->onPressedEvent(GLFW_KEY_F3, "toggleWireFrame");
	this->onPressedEvent(GLFW_KEY_F4, "toggleNormalMapping");
	this->onPressedEvent(GLFW_KEY_F5, "toggleLightMap");
	this->onPressedEvent(GLFW_KEY_TAB, "toggleDebug");

	this->onPressed({ GLFW_KEY_W, GLFW_KEY_UP }, "moveForward");
	this->onPressed({ GLFW_KEY_D, GLFW_KEY_RIGHT }, "turnRight");
	this->onPressed({ GLFW_KEY_A, GLFW_KEY_LEFT }, "turnLeft");
	this->onPressed({ GLFW_KEY_S, GLFW_KEY_DOWN }, "moveBackward");

	this->onPressed({ GLFW_KEY_W, GLFW_KEY_UP }, "cameraForward");
	this->onPressed({ GLFW_KEY_S, GLFW_KEY_DOWN }, "cameraBackward");
	this->onPressed({ GLFW_KEY_D, GLFW_KEY_RIGHT }, "cameraRight");
	this->onPressed({ GLFW_KEY_A, GLFW_KEY_LEFT }, "cameraLeft");

	this->onPressed({ GLFW_KEY_E, GLFW_KEY_PAGE_DOWN }, "cameraDown");
	this->onPressed({ GLFW_KEY_Q, GLFW_KEY_PAGE_UP }, "cameraUp");


	this->onPressedEvent(GLFW_KEY_SPACE, "jump");

}

bool InputHandler::getEvent(std::string name) {
	if (this->events.find(name) == this->events.end()) {
		return false;
	}
	else {
		return this->events[name];
	}
}

glm::vec2 InputHandler::getMouseMovement() {
	return this->mouseMovement;
}

void InputHandler::setMouseFixed(bool isMouseFixed) {
	if (isMouseFixed && !this->events["cursorCaptured"]) {
		glfwSetCursorPos(this->window.getPointer(), 0, 0);
	}
	this->events["cursorCaptured"] = isMouseFixed;
}

bool InputHandler::isMouseFixed() {
	return this->events["cursorCaptured"];
}

void InputHandler::onPressedEvent(int key, const std::string eventName) {
	onPressedEvent(std::vector<int>(1, key), eventName);
}

void InputHandler::onPressedEvent(std::vector<int> keys, const std::string eventName) {
	this->events[eventName] = false;
	for each (int key in keys) {
		onKeyPress(key, [this, eventName]() {this->events[eventName] = true; });
	}
}

void InputHandler::onPressed(int key, std::string eventName) {
	this->onPressed(std::vector<int>({ key }), eventName);
}

void InputHandler::onPressed(std::vector<int> keys, std::string eventName) {
	for each (int key in keys) {
		onKeyPress(key, [this, eventName]() {this->events[eventName] = true; });
		onKeyRelease(key, [this, eventName]() {this->events[eventName] = false; });
	}
}

void InputHandler::onKeyRelease(int key, std::function<void()> callback) {
	bool oldKeyState = this->keyState[key];
	bool newKeyState = GLFW_PRESS == glfwGetKey(this->window.getPointer(), key);

	this->newKeyState[key] = newKeyState;

	if (oldKeyState != newKeyState && newKeyState == GLFW_RELEASE) {
		callback();
	}
}

void InputHandler::onKeyPress(int key, std::function<void()> callback) {
	bool oldKeyState = this->keyState[key];
	bool newKeyState = GLFW_PRESS == glfwGetKey(this->window.getPointer(), key);

	this->newKeyState[key] = newKeyState;

	if (oldKeyState != newKeyState && newKeyState == GLFW_PRESS) {
		callback();
	}
}

void InputHandler::onMouseRelease(int key, std::function<void()> callback) {
	bool oldKeyState = this->keyState[key];
	bool newKeyState = GLFW_PRESS == glfwGetMouseButton(this->window.getPointer(), key);

	this->newKeyState[key] = newKeyState;

	if (oldKeyState != newKeyState && newKeyState == GLFW_RELEASE) {
		callback();
	}
}

void InputHandler::onMousePress(int key, std::function<void()> callback) {
	bool oldKeyState = this->keyState[key];
	bool newKeyState = GLFW_PRESS == glfwGetMouseButton(this->window.getPointer(), key);

	this->newKeyState[key] = newKeyState;

	if (oldKeyState != newKeyState && newKeyState == GLFW_PRESS) {
		callback();
	}
}