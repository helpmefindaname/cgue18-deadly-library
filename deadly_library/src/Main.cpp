#include "config.h"
#include "loop\Gameloop.h"

int main(int argc, char** argv)
{
	Config::readFile("assets/settings.ini");
	Gameloop gameloop(60);
	gameloop.run();

	return EXIT_SUCCESS;
}