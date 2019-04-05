#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_ITEM_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_ITEM_H_

#include "World/Entity/object.hpp"

namespace PopHead{
namespace World{
namespace Entity{


class Item : public Object
{
public:
    //void onCollision(Object&) override;
    void pickUp();

private:
    //std::unique_ptr<Item::Item> mItem;
};


}}}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_ITEM_H_
