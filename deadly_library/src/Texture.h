/*
* Copyright 2017 Vienna University of Technology.
* Institute of Computer Graphics and Algorithms.
* This file is part of the ECG Lab Framework and must not be redistributed.
*/
#pragma once


#include <string>
#include "libimport\glew.h"


/* --------------------------------------------- */
// 2D texture
/* --------------------------------------------- */

class Texture
{
private:
	GLuint _handle;
	bool _init;

public:
	Texture(std::string file);
	~Texture();

	void bind(unsigned int unit);
};
