#include "camera.hpp"
#include "Utilities/math.hpp"
#include <cmath>

namespace ph {

Camera::Camera()
	:Camera({0, 0}, {640, 360})
{
}

Camera::Camera(const FloatRect& rect)
	:Camera(rect.getCenter(), {rect.width, rect.height})
{
}

Camera::Camera(const sf::Vector2f center, const sf::Vector2f size)
	:mViewProjectionMatrix(sf::Transform::Identity)
	,mCenter(center)
	,mSize(size)
	,mRotation(0.f)
	,mViewProjectionMatrixNeedsUpdate(true)
{
}

void Camera::setCenter(const sf::Vector2f center)
{
	mCenter = center;
	mViewProjectionMatrixNeedsUpdate = true;
}

void Camera::move(const sf::Vector2f offset)
{
	mCenter += offset;
	mViewProjectionMatrixNeedsUpdate = true;
}

void Camera::setCenterSmoothly(sf::Vector2f center, float speed)
{
	mCenter = Math::lerp(mCenter, center, speed);
	mViewProjectionMatrixNeedsUpdate = true;
}

void Camera::setRotation(const float rotation)
{
	mRotation = rotation;
	mViewProjectionMatrixNeedsUpdate = true;
}

void Camera::rotate(const float rotation)
{
	mRotation += rotation;
	mViewProjectionMatrixNeedsUpdate = true;
}

void Camera::setSize(const sf::Vector2f size)
{
	mSize = size;
	mViewProjectionMatrixNeedsUpdate = true;
}

void Camera::zoom(const float factor)
{
	mSize *= factor;
	mViewProjectionMatrixNeedsUpdate = true;
}

const sf::Transform& Camera::getViewProjectionMatrix4x4()
{
	if(mViewProjectionMatrixNeedsUpdate)
	{
		// Rotation components
		float angle = mRotation * 3.141592654f / 180.f;
		float cosine = static_cast<float>(std::cos(angle));
		float sine = static_cast<float>(std::sin(angle));
		float tx = -mCenter.x * cosine - mCenter.y * sine + mCenter.x;
		float ty = mCenter.x * sine - mCenter.y * cosine + mCenter.y;

		// Projection components
		float a = 2.f / mSize.x;
		float b = -2.f / mSize.y;
		float c = -a * mCenter.x;
		float d = -b * mCenter.y;

		// Rebuild the view projection matrix
		mViewProjectionMatrix = sf::Transform(
		    a * cosine, a * sine  , a * tx + c,
		   -b * sine  , b * cosine, b * ty + d,
		    0.f       , 0.f       , 1.f
		);
		mViewProjectionMatrixNeedsUpdate = false;
	}
	return mViewProjectionMatrix;
}

}
