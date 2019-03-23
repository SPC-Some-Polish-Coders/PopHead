#ifndef POPHEAD_WORLD_ENTITY_H_
#define POPHEAD_WORLD_ENTITY_H_

#include <string>

namespace PopHead {
namespace World {
namespace Entity {
class Entity
{
  public:
    Entity( std::string name );

    inline auto getName() -> const std::string&;

  private:
    std::string mName;
};

auto Entity::getName() -> const std::string&
{ return mName; }

}}}

#endif // !POPHEAD_WORLD_ENTITY_H_
