#ifndef POPHEAD_PHYSICS_COLLISIONSBODY_H_
#define POPHEAD_PHYSICS_COLLISIONSBODY_H_

#include <SFML/Graphics.hpp>
#include "Base/gameData.hpp"
#include "World/Entity/object.hpp"
#include "Physics/CollisionDebug/collisionDebugRect.hpp"
#include "Utilities/math.hpp"

namespace PopHead {
namespace Physics {

enum class BodyType;

class CollisionBody
{
public:
	CollisionBody(sf::FloatRect rect, float mass, BodyType, PopHead::World::Entity::Object* const owner, PopHead::Base::GameData*);
	~CollisionBody();

	//the methods below should be called from owner
	void move(sf::Vector2f velocity);
	void setPosition(sf::Vector2f position);
	bool isBeingPushed() { return (mForceVector.x != 0 || mForceVector.y != 0); }

	//the methods below should be called only from physics module
	void updatePush(sf::Time delta);
	void setForceVector(sf::Vector2f forceVector) { mForceVector = forceVector; }
	
	void actionsAtTheEndOfPhysicsLoopIteration();
private:
	void setPreviousPositionToCurrentPosition();
	void updateOwnerPosition();

public:
	auto getBodyType() const -> const BodyType { return mBodyType; }
	auto getNameOfOwner() -> const std::string& { return mOwner->getName(); }
	auto getVelocity() -> sf::Vector2f { return mVelocity; }
	auto getPosition() -> sf::Vector2f { return sf::Vector2f(mRect.left, mRect.top); }
	auto getPositionOfCenter() -> sf::Vector2f { return PopHead::Utilities::Math::getCenter(mRect); }
	auto getRect() -> const sf::FloatRect& { return mRect; }
	auto getPreviousRect() -> sf::FloatRect { return sf::FloatRect(mPreviousPosition.x, mPreviousPosition.y, mRect.width, mRect.height); }
	float getMass() { return mMass; }

private:
	sf::FloatRect mRect;
	sf::Vector2f mPreviousPosition;
	sf::Vector2f mVelocity;
	sf::Vector2f mForceVector;
	float mMass;
	const BodyType mBodyType;
	CollisionDebugRect mCollisionDebugRect;
	World::Entity::Object* const mOwner;
	Base::GameData* mGameData;
};


}}

#endif // POPHEAD_PHYSICS_COLLISIONSBODY_H_
