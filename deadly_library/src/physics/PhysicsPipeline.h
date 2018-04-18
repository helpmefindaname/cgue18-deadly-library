#pragma once
#include "../libimport/physX.h"
class PhysicsPipeline
{
public:
	PhysicsPipeline();
	~PhysicsPipeline();

	void update();

private:
	void init(bool interactive);
};



