#pragma once 

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Transform.hpp>
#include "Utilities/rect.hpp"

namespace ph { 

class Camera
{
public:
	Camera();
	Camera(FloatRect& rect);
	Camera(Vec2 center, Vec2 size);

	void setCenter(Vec2 center);
	void move(Vec2 offset);
	Vec2 center() const { return mCenter; }

	void setRotation(float rotation);
	void rotate(float rotation);
	float getRotation() const { return mRotation; }

	void setSize(Vec2 size);
	void zoom(float factor);
	Vec2 getSize() const { return mSize; }

	const sf::Transform& getViewProjectionMatrix4x4();

	FloatRect getBounds();

private:
	sf::Transform mViewProjectionMatrix;
	Vec2 mCenter;
	Vec2 mSize;
	float mRotation;
	bool mViewProjectionMatrixNeedsUpdate;
};

}
