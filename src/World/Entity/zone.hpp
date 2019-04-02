#ifndef POPHEAD_WORLD_ZONE_H_
#define POPHEAD_WORLD_ZONE_H_

#include "World/Entity/entity.hpp"
#include "World/Entity/object.hpp"

namespace PopHead{
namespace World{
namespace Entity{


class Zone : public Entity
{
public:
    ///auto getArea() const -> const CollsionRect&
    virtual void onEnter(Object* const) = 0;
    virtual void onExit(Object* const) = 0;
    void update(sf::Time delta) override;

private:
    std::list<Object* const> mEnclosedObjects;
};


}}}

#endif // !POPHEAD_WORLD_ZONE_H_
