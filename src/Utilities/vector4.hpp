#pragma once

namespace ph {

// TODO: Add overloaded operators + - * /

template<typename T>
struct Vector4
{
	T x, y, z, w;
};

using Vector4f = Vector4<float>;
using Vector4i = Vector4<int>;
using Vector4u = Vector4<unsigned>;

}