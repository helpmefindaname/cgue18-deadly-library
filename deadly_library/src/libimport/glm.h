#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>


class Glm {
public:
	static glm::tmat4x4<float> lookAt(glm::tvec3<float> const & eye,
		glm::tvec3<float> const & center,
		glm::tvec3<float> const & up);

	static float pi;

	static int sign(float f);
};
