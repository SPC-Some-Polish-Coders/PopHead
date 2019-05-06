#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_COLLISIONDEBUGRECT_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_COLLISIONDEBUGRECT_H_

#include "World/Entity/object.hpp"
#include "Physics/bodyType.hpp"

namespace PopHead {
namespace Physics {


class CollisionDebugRect : public World::Entity::Object
{
public:
    CollisionDebugRect(PopHead::Base::GameData* gameData, sf::FloatRect rect);

	void move(sf::Vector2f velocity) { mShape.move(velocity); }
	void setPosition(sf::Vector2f position) { mShape.setPosition(position); }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void setColor(sf::Color color) { mShape.setFillColor(color); }
	static void setShouldDisplay(bool shouldDisplay) { mShouldDisplay = shouldDisplay; }

private:
    sf::RectangleShape mShape;
	static bool mShouldDisplay;
};


}}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_COLLISIONDEBUGRECT_H_
