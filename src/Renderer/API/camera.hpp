#pragma once 

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>
#include "Utilities/rect.hpp"

namespace ph { 

class Camera
{
public:
	Camera();
	Camera(const FloatRect& rect);
	Camera(const sf::Vector2f center, const sf::Vector2f size);

	void setCenter(const sf::Vector2f center);
	void setCenterSmoothly(sf::Vector2f center, float speed);
	void move(const sf::Vector2f offset);
	sf::Vector2f getCenter() const { return mCenter; }

	void setRotation(const float rotation);
	void rotate(const float rotation);
	float getRotation() const { return mRotation; }

	void setSize(const sf::Vector2f size);
	void zoom(const float factor);
	sf::Vector2f getSize() const { return mSize; }

	const sf::Transform& getViewProjectionMatrix4x4();

	FloatRect getBounds() const;

private:
	sf::Transform mViewProjectionMatrix;
	sf::Vector2f mCenter;
	sf::Vector2f mSize;
	float mRotation;
	bool mViewProjectionMatrixNeedsUpdate;
};

}
