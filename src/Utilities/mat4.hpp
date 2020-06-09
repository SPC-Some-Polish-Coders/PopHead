#include "rect.hpp"

namespace ph {

struct mat4
{
	float e[16];
};

static inline
mat4 makeMat4(float a00, float a01, float a02,
			  float a10, float a11, float a12,
			  float a20, float a21, float a22)
{
	mat4 res;
	res.e[0] = a00; res.e[4] = a01; res.e[8]  = 0.f; res.e[12] = a02;
	res.e[1] = a10; res.e[5] = a11; res.e[9]  = 0.f; res.e[13] = a12;
	res.e[2] = 0.f; res.e[6] = 0.f; res.e[10] = 1.f; res.e[14] = 0.f;
	res.e[3] = a20; res.e[7] = a21; res.e[11] = 0.f; res.e[15] = a22;
	return res;
}

static inline
mat4 makeOrthographic(FloatRect cameraBounds)
{
	return makeMat4(
		2.f / cameraBounds.w, 0.f, -((cameraBounds.x + cameraBounds.right()) / cameraBounds.w),
		0.f, 2.f / -cameraBounds.h, ((cameraBounds.y + cameraBounds.bottom()) / cameraBounds.h),
		0.f, 0.f, 1.f
	);
}

}

