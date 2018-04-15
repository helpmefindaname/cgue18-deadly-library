#pragma once

#include <string>
#include <functional>
#include <unordered_map>

#include "../libimport/glm.h"
#include "../window.h"

class InputHandler {
public:
	InputHandler(Window& window);
	~InputHandler();

private:
	Window& window;

	glm::vec2 mouseMovement;
	std::unordered_map<int, bool> keyState;
	std::unordered_map<int, bool> newKeyState;
	std::unordered_map<std::string, bool> events;

public:
	void process();
	bool getEvent(std::string name);
	glm::vec2 getMouseMovement();
	void setMouseFixed(bool isMouseFixed);
	bool isMouseFixed();

private:

	void onPressedEvent(int key, std::string eventName);
	void onPressedEvent(std::vector<int> keys, std::string eventName);


	void onPressed(int key, std::string eventName);
	void onPressed(std::vector<int> keys, std::string eventName);

	void onKeyRelease(int key, std::function<void()> callback);
	void onKeyPress(int key, std::function<void()> callback);

	void onMouseRelease(int key, std::function<void()> callback);
	void onMousePress(int key, std::function<void()> callback);
};