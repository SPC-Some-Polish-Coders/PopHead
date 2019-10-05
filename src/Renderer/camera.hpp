#pragma once 

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>
#include "Utilities/rect.hpp"

namespace ph { 

// TODO: Make Camera rotation possible

class Camera
{
public:
	Camera();
	Camera(const FloatRect& rect);
	Camera(const sf::Vector2f center, const sf::Vector2f size);

	void setCenter(const sf::Vector2f center);
	void move(const sf::Vector2f offset);
	sf::Vector2f getCenter() const { return mCenter; }

	void setSize(const sf::Vector2f size);
	void zoom(const float factor);
	sf::Vector2f getSize() const { return mSize; }

	const float* getViewMatrix4x4();
	const float* getProjectionMatrix4x4();
	const float* getViewProjectionMatrix4x4();

private:
	void updateViewMatrix();
	void updateProjectionMatrix();
	void updateViewProjectionMatrix();

private:
	sf::Transform mViewMatrix;
	sf::Transform mProjectionMatrix;
	sf::Transform mViewProjectionMatrix;
	sf::Vector2f mCenter;
	sf::Vector2f mSize;
	bool mViewMatrixNeedsUpdate;
	bool mProjectionMatrixNeedsUpdate;
	bool mViewProjectionMatrixNeedsUpdate;
};

}
