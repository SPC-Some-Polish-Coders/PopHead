#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_STATICOBJECTTOCAMERA_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_STATICOBJECTTOCAMERA_H_

#include "World/Entity/object.hpp"

///This class is only for test purposes
///It should be deleted when GUI is made

namespace PopHead {
namespace World {
namespace Entity {


class StaticObjectToCamera : public Object
{
public:
	StaticObjectToCamera(PopHead::Base::GameData*);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void move(sf::Vector2f offset, bool recursive = true) override;

private:
	sf::RectangleShape mShape;
};


}}}

#endif //!POPHEAD_WORLD_ENTITY_OBJECTS_STATICOBJECTTOCAMERA_H_
