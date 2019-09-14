#pragma once

#include "GameObjects/gameObject.hpp"

namespace ph {

class CollisionBody;
class PhysicsEngine;

class Gate : public GameObject
{
public:
	Gate(const sf::Texture& gateTexture, const sf::Vector2f position, PhysicsEngine&, const bool isOpen);

	void drawCurrent(sf::RenderTarget&, const sf::RenderStates) const override;

	void open();
	void close();

private:
	sf::Sprite mSprite;
	const CollisionBody& mCollisionBody;
	PhysicsEngine& mPhysicsEngine;
	bool mIsOpen;
};

}
