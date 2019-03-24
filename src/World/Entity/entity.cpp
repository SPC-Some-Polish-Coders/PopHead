#include "entity.hpp"

using PopHead::World::Entity::Entity;


auto Entity::getEntityType() const -> EntityType
{
    return mEntityType;
}

auto Entity::getID() const -> unsigned int
{
    return mID;
}

auto Entity::getParent() const -> Entity&
{
    return *mParent;
}

auto Entity::getChild(std::string name) const -> Entity&
{
    for(auto const &child : mChildren){
        if(child->getName() == name)
            return *(child.get());
    }
}

auto Entity::getChildren() -> std::list< std::unique_ptr<Entity> >&
{
    return mChildren;
}

auto Entity::getName() const -> const std::string&
{
    return mName;
}
