#pragma once

namespace ph {

template<typename T>
union Vec4Base
{
	struct {T x, y, z, w; };
	struct {T r, g, b, a; };
};

using Vec4 = Vec4Base<float>;
using Vec4i = Vec4Base<i32>;
using Vec4u = Vec4Base<u32>;

}
