#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_COLLISIONDEBUGRECT_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_COLLISIONDEBUGRECT_H_

#include "World/Entity/object.hpp"
#include "Physics/bodyType.hpp"

namespace PopHead {
namespace Physics {

class CollisionDebugSettings;
class CollisionBody;

class CollisionDebugRect : public World::Entity::Object
{
public:
    CollisionDebugRect(PopHead::Base::GameData* gameData, sf::FloatRect rect, CollisionBody* owner);

	void move(sf::Vector2f velocity) { mShape.move(velocity); }
	void setPosition(sf::Vector2f position) { mShape.setPosition(position); }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setColor(sf::Color color) { mShape.setFillColor(color); }

private:
	bool shouldDisplay() const;

	static void setShouldDisplay(bool shouldDisplay) { mShouldDisplay = shouldDisplay; }
	static void setShouldDisplayKinematicBodies(bool shouldDisplay) { mShouldDisplayKinematicBodies = shouldDisplay; }
	static void setShouldDisplayStaticBodies(bool shouldDisplay) { mShouldDisplayStaticBodies = shouldDisplay; }

	friend CollisionDebugSettings;

private:
    sf::RectangleShape mShape;
	CollisionBody* mOwner;
	
	static bool mShouldDisplay;
	static bool mShouldDisplayKinematicBodies;
	static bool mShouldDisplayStaticBodies;
};


}}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_COLLISIONDEBUGRECT_H_
