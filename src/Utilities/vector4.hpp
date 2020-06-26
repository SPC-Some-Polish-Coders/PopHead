#pragma once

namespace ph {

template<typename T>
union Vec4Base
{
	Vec4Base(T x, T y, T z, T w) :x(x), y(y), z(z), w(w) {}
	Vec4Base() :x(0), y(0), z(0), w(0) {}

	struct {T x, y, z, w; };
	struct {T r, g, b, a; };
};

using Vec4 = Vec4Base<float>;
using Vec4i = Vec4Base<i32>;
using Vec4u = Vec4Base<u32>;

}
