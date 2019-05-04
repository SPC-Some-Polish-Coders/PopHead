#include "entity.hpp"

using PopHead::World::Entity::Entity;

Entity::Entity(PopHead::World::EntityType type, PopHead::Base::GameData* gameData, std::string name)
:mEntityType(type)
,mGameData(gameData)
,mName(name)
{

}

void Entity::input()
{
    for(auto it = mChildren.begin(); it != mChildren.end(); ++it)
        (*it)->input();
}

void Entity::update(sf::Time delta)
{
    for(auto it = mChildren.begin(); it != mChildren.end(); ++it)
        (*it)->update(delta);
}

void Entity::addChild(EntityPtr newChild)
{
    newChild->mParent = this;
    mChildren.emplace_back(std::move(newChild));
}

void Entity::removeChild(const std::string& name)
{
	for (auto it = mChildren.begin(); it != mChildren.end(); ++it) {
		if ((*it)->getName() == name) {
            mChildren.erase(it);
			break;
		}
	}
}

void Entity::removeChild(Entity* pointerToChildWhichIsSupposedToBeRemoved)
{
	for (auto it = mChildren.begin(); it != mChildren.end(); ++it) {
		if ((*it).get() == pointerToChildWhichIsSupposedToBeRemoved) {
			mChildren.erase(it);
			break;
		}
	}
}

auto Entity::getChild(std::string name) const -> Entity&
{
    for(auto const &child : mChildren){
        if(child->getName() == name)
            return *(child.get());
    }
}