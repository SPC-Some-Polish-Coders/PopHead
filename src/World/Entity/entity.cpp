#include "entity.hpp"

using PopHead::World::Entity::Entity;


auto Entity::getName() const -> const std::string&
{
    return mName;
}
