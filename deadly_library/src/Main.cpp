#include "config.h"
#include "loop\Gameloop.h"
#include <stdexcept>
#include <iostream>

int main(int argc, char** argv)
{
	try {
		Config::readFile("assets/settings.ini");
		Gameloop gameloop(60);
		gameloop.run();
	}
	catch (std::runtime_error e) {
		std::cout << e.what() << std::endl;
		std::cin.get();
	}
}