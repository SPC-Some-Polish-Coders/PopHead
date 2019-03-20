#ifndef POPHEAD_WORLD_OBJECT_H_
#define POPHEAD_WORLD_OBJECT_H_

#include "World/Entity/Entity.hpp"

namespace PopHead {
namespace World {
namespace Entity {

class Object : public Entity
{
  public:
    virtual void draw() =0;
};

}}}

#endif // !POPHEAD_WORLD_OBJECT_H_
