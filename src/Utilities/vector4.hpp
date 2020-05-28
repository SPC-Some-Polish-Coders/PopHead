#pragma once

namespace ph {

template<typename T>
struct Vec4Base
{
	T x, y, z, w;
};

using Vec4 = Vec4Base<float>;
using Vec4i = Vec4Base<i32>;
using Vec4u = Vec4Base<u32>;

}
