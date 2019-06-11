#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_ITEM_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_ITEM_H_

#include "World/Entity/object.hpp"

namespace ph{

class Item : public Object
{
public:
    void pickUp();

private:
    //std::unique_ptr<Item::Item> mItem;
};


}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_ITEM_H_
