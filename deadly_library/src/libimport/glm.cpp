#include "glm.h"

glm::tmat4x4<float> Glm::lookAt(glm::tvec3<float> const & eye, glm::tvec3<float> const & center, glm::tvec3<float> const & up)
{
	glm::tvec3<float> const f(normalize(center - eye));
	glm::tvec3<float> const s(normalize(cross(f, up)));
	glm::tvec3<float> const u(cross(s, f));

	glm::tmat4x4<float> Result(1);
	Result[0][0] = s.x;
	Result[1][0] = s.y;
	Result[2][0] = s.z;
	Result[0][1] = u.x;
	Result[1][1] = u.y;
	Result[2][1] = u.z;
	Result[0][2] = -f.x;
	Result[1][2] = -f.y;
	Result[2][2] = -f.z;
	Result[3][0] = -dot(s, eye);
	Result[3][1] = -dot(u, eye);
	Result[3][2] = dot(f, eye);
	return Result;
}