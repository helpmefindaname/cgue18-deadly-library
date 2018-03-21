#pragma once

#include <GL\glew.h>

class Glew {
public:
	Glew();
	~Glew();

private:
	static int refCount;
};