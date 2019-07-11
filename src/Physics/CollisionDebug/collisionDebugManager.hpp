#pragma once

#include "collisionDebugRect.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

namespace ph {

class CollisionBody;

class CollisionDebugManager : public sf::Drawable
{
public:
	void addStaticBodyCollisionDebugRect(const CollisionBody&);
	void addKinematicBodyCollisionDebugRect(const CollisionBody&);

	void draw(sf::RenderTarget&, const sf::RenderStates) const override;

private:
	std::vector<CollisionDebugRect> mStaticBodyCollisionDebugRects;
	std::vector<CollisionDebugRect> mKinematicBodyCollisionDebugRects;
};

}