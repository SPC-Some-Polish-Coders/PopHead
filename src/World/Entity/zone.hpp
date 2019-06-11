#ifndef POPHEAD_WORLD_ZONE_H_
#define POPHEAD_WORLD_ZONE_H_

#include "World/Entity/entity.hpp"
#include "World/Entity/object.hpp"

namespace ph{
namespace World{
namespace Entity{


class Zone : public Entity
{
public:
    ///auto getArea() const -> const CollsionRect&
    virtual void onEnter(Object*) = 0;
    virtual void onExit (Object*) = 0;
    virtual void present(Object*) = 0;
    //void update(sf::Time delta) override;

private:
    std::list<Object*> mEnclosedObjects;
};


}}}

#endif // !POPHEAD_WORLD_ZONE_H_
