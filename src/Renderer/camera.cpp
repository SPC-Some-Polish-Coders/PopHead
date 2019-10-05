#include "camera.hpp"
#include <cmath>

namespace ph {

Camera::Camera()
	:Camera({0, 0}, {640, 480})
{
}

Camera::Camera(const FloatRect& rect)
	:Camera(rect.getCenter(), {rect.width, rect.height})
{
}

Camera::Camera(const sf::Vector2f center, const sf::Vector2f size)
	:mViewMatrix(sf::Transform::Identity)
	,mProjectionMatrix(sf::Transform::Identity)
	,mViewProjectionMatrix(sf::Transform::Identity)
	,mCenter(center)
	,mSize(size)
	,mViewMatrixNeedsUpdate(true)
	,mProjectionMatrixNeedsUpdate(true)
	,mViewProjectionMatrixNeedsUpdate(true)
{
}

void Camera::setCenter(const sf::Vector2f center)
{
	mCenter = center;
	mViewMatrixNeedsUpdate = true;
	mViewProjectionMatrixNeedsUpdate = true;
}

void Camera::move(const sf::Vector2f offset)
{
	mCenter += offset;
	mViewMatrixNeedsUpdate = true;
	mViewProjectionMatrixNeedsUpdate = true;
}

void Camera::setSize(const sf::Vector2f size)
{
	mSize = size;
	mProjectionMatrixNeedsUpdate = true;
	mViewProjectionMatrixNeedsUpdate = true;
}

void Camera::zoom(const float factor)
{
	mSize *= factor;
	mProjectionMatrixNeedsUpdate = true;
	mViewProjectionMatrixNeedsUpdate = true;
}

const float* Camera::getViewMatrix4x4()
{
	if(mViewMatrixNeedsUpdate)
		updateViewMatrix();

	return mViewMatrix.getMatrix();
}

const float* Camera::getProjectionMatrix4x4()
{
	if(mProjectionMatrixNeedsUpdate)
		updateProjectionMatrix();

	return mProjectionMatrix.getMatrix();
}

const float* Camera::getViewProjectionMatrix4x4()
{
	if(mViewMatrixNeedsUpdate)
		updateViewMatrix();
	if(mProjectionMatrixNeedsUpdate)
		updateProjectionMatrix();
	if(mViewProjectionMatrixNeedsUpdate)
		updateViewProjectionMatrix();

	return mViewProjectionMatrix.getMatrix();
}

void Camera::updateViewMatrix()
{
	mViewMatrix = sf::Transform(
		1, 0, -mCenter.x,
		0, 1, -mCenter.y,
		0, 0, 1
	);

	mViewMatrixNeedsUpdate = false;
}

void Camera::updateProjectionMatrix()
{
	mProjectionMatrix = sf::Transform(
		2/(mSize.x/640), 0              , -1,
		0              , 2/(mSize.y/480), -1,
		0              , 0              ,  1
	);
	
	mProjectionMatrixNeedsUpdate = false;
}

void Camera::updateViewProjectionMatrix()
{
	mViewProjectionMatrix = mProjectionMatrix * mViewMatrix;
	mViewProjectionMatrixNeedsUpdate = false;
}

}
