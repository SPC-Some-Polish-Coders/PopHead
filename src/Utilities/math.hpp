#pragma once

#include "Utilities/forceInline.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <cmath>

namespace ph::Math {

	constexpr float pi = 3.14159265359f;

	constexpr float nullFloat = -99999999.99f;
	const Vec2 nullVec2 = Vec2(nullFloat, nullFloat);

	template <typename T>
	FORCE_INLINE Vec2Base<T> hadamardMul(Vec2Base<T>, Vec2Base<T>);
	template <typename T>
	FORCE_INLINE Vec2Base<T> hadamardDiv(Vec2Base<T>, Vec2Base<T>);

	template <typename T>
	FORCE_INLINE Vec2Base<T> getUnitVector(Vec2Base<T>);

	FORCE_INLINE Vec2 getUnitVector(float angle);

	FORCE_INLINE float getMagnitude(Vec2); 
	
	FORCE_INLINE float degreesToRadians(float angle);
	FORCE_INLINE float radiansToDegrees(float angle);

	FORCE_INLINE Vec2u getTwoDimensionalPositionFromOneDimensionalArrayIndex(u32 index, u32 numberOfColumns);

	FORCE_INLINE bool isPointInsideCircle(Vec2 point, Vec2 circlePos, float radius);

	FORCE_INLINE Vec2 lerp(Vec2 source, Vec2 destination, float speed);
	FORCE_INLINE float lerp(float source, float destination, float speed);

	template <typename T>
	FORCE_INLINE float distanceBetweenPoints(Vec2Base<T> point1, Vec2Base<T> point2);

	template <typename T>
	FORCE_INLINE Vec2Base<T> abs(Vec2Base<T>);

	FORCE_INLINE bool areApproximatelyEqual(float a, float b, float maxApproximation);
	FORCE_INLINE bool areApproximatelyEqual(Vec2 a, Vec2 b, float maxApproximation);
}

#include "math.inl"
