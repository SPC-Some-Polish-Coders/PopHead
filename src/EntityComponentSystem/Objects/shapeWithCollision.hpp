#pragma once

#include "EntityComponentSystem/object.hpp"
#include "Physics/CollisionBody/collisionBody.hpp"

///This class is for test purposes
///It should be probably deleted when map collisions and the whole Physics module are made

namespace ph {

class ShapeWithCollision : public Object
{
public:
	ShapeWithCollision(GameData*);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void setPosition(sf::Vector2f, bool recursive = true) override;

private:
	sf::RectangleShape mShape;
	CollisionBody mCollisionBody;
};


}
