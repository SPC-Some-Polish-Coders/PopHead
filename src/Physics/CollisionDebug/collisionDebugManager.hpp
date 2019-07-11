#pragma once

#include "collisionDebugRect.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

namespace ph {

class CollisionBody;
class PhysicsEngine;

class CollisionDebugManager : public sf::Drawable
{
public:
	void addStaticBodyCollisionDebugRect(const CollisionBody&);
	void addKinematicBodyCollisionDebugRect(const CollisionBody&);

	void draw(sf::RenderTarget&, const sf::RenderStates) const override;
	
private:
	mutable std::vector<CollisionDebugRect> mStaticBodyCollisionDebugRects;
	mutable std::vector<CollisionDebugRect> mKinematicBodyCollisionDebugRects;
	friend PhysicsEngine;
};

}