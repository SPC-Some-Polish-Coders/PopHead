#include "pch.hpp"
#include "camera.hpp"

namespace ph {

Camera::Camera()
	:Camera({0, 0}, {640, 360})
{
}

Camera::Camera(FloatRect& rect)
	:Camera(rect.center(), {rect.w, rect.h})
{
}

Camera::Camera(Vec2 center, Vec2 size)
	:mViewProjectionMatrix(sf::Transform::Identity)
	,mCenter(center)
	,mSize(size)
	,mRotation(0.f)
	,mViewProjectionMatrixNeedsUpdate(true)
{
}

void Camera::setCenter(Vec2 center)
{
	mCenter = center;
	mViewProjectionMatrixNeedsUpdate = true;
}

void Camera::move(Vec2 offset)
{
	mCenter += offset;
	mViewProjectionMatrixNeedsUpdate = true;
}

void Camera::setRotation(float rotation)
{
	mRotation = rotation;
	mViewProjectionMatrixNeedsUpdate = true;
}

void Camera::rotate(float rotation)
{
	mRotation += rotation;
	mViewProjectionMatrixNeedsUpdate = true;
}

void Camera::setSize(Vec2 size)
{
	mSize = size;
	mViewProjectionMatrixNeedsUpdate = true;
}

void Camera::zoom(float factor)
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
		float cosine = Cast<float>(std::cos(angle));
		float sine = Cast<float>(std::sin(angle));
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

FloatRect Camera::getBounds()
{
	return FloatRect(mCenter.x - mSize.x / 2, mCenter.y - mSize.y / 2, mSize.x, mSize.y);
}

}
