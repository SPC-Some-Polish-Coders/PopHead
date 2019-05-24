#ifndef POPHEAD_PHYSICS_COLLISIONSBODY_H_
#define POPHEAD_PHYSICS_COLLISIONSBODY_H_

#include <SFML/Graphics.hpp>
#include "Base/gameData.hpp"
#include "World/Entity/object.hpp"
#include "CollisionDebug/collisionDebugRect.hpp"
#include "Utilities/math.hpp"

namespace PopHead {
namespace Physics {

enum class BodyType;

class CollisionBody
{
public:
	CollisionBody(sf::FloatRect rect, float mass, BodyType, PopHead::World::Entity::Object* const owner, PopHead::Base::GameData*);
	~CollisionBody();

	void updatePush(sf::Time delta);
	void setForceVector(sf::Vector2f forceVector);

	void move(sf::Vector2f velocity);
	void setPosition(sf::Vector2f position);

	float getMass() { return mMass; }
	auto getPosition() -> sf::Vector2f { return sf::Vector2f(mRect.left, mRect.top); }
	auto getVelocity() -> sf::Vector2f { return mVelocity; }
	auto getPositionOfCenter() -> sf::Vector2f { return PopHead::Utilities::Math::getCenter(mRect); }
	auto getBodyType() const -> const BodyType & { return mBodyType; }
	bool getStunStatus() { return (mForceVector.x != 0 || mForceVector.y != 0); }

private:
	void updateOwnerPosition();
	void setPreviousPositionToCurrentPosition();
	sf::FloatRect getPreviousRect();

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

	friend PhysicsEngine;
	friend StaticCollisionHandler;

	friend KinematicCollisionHandler; //temporary
};


}}

#endif // POPHEAD_PHYSICS_COLLISIONSBODY_H_
