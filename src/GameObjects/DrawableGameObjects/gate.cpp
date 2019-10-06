#include "gate.hpp"
#include "Physics/physicsEngine.hpp"

namespace ph{

Gate::Gate(const sf::Texture& gateTexture, const sf::Vector2f position, PhysicsEngine& physicsEngine, const bool isOpen)
	:GameObject("gate")
	,mSprite(gateTexture)
	,mCollisionBody(physicsEngine.createStaticBodyAndGetTheReference(sf::FloatRect(position.x, position.y + 6, 32, 10)))
	,mPhysicsEngine(physicsEngine)
	,mIsOpen(isOpen)
{
	setPosition(position);
}

void Gate::drawCurrent(sf::Transform)
{
	//if(!mIsOpen)
	//	target.draw(mSprite, states);
}

void Gate::open()
{
	if(mIsOpen)
		return;
	mPhysicsEngine.removeStaticBody(mCollisionBody);
	mIsOpen = true;
}

void Gate::close()
{
	if(!mIsOpen)
		return;
	// TODO: Fix bug that gate can be closed only one time, Fixing this requires changes in PhysicsEngine
	mPhysicsEngine.createStaticBodyAndGetTheReference(sf::FloatRect(getPosition().x, getPosition().y + 6, 32, 10));
	mIsOpen = false;
}

}
