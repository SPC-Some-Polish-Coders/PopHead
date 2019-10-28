#pragma once

#include "GameObjects/gameObject.hpp"
#include "Renderer/renderer.hpp"

namespace ph {

class CollisionBody;
class PhysicsEngine;

class Gate : public GameObject
{
public:
	Gate(const Texture& texture, const sf::Vector2f position, PhysicsEngine&, const bool isOpen);

	void drawCurrent(sf::Transform) override;

	void open();
	void close();

private:
	const Texture& mTexture;
	const CollisionBody& mCollisionBody;
	PhysicsEngine& mPhysicsEngine;
	bool mIsOpen;
};

}
