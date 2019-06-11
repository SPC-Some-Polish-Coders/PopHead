#include "entity.hpp"

#include "Utilities/debug.hpp"

using ph::World::Entity::Entity;

Entity::Entity(ph::World::EntityType type, ph::Base::GameData* gameData, std::string name)
:mEntityType(type)
,mGameData(gameData)
,mName(name)
,mParent(nullptr)
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
	const std::string nameOfNewChild = newChild->getName();
    newChild->mParent = this;
    mChildren.emplace_back(std::move(newChild));
	PH_LOG(LogType::Info, "Entity \"" + nameOfNewChild + "\" was added as child of the \"" + mName + "\"");
}

void Entity::removeChild(const std::string& name)
{
	for (auto it = mChildren.begin(); it != mChildren.end(); ++it) {
		if ((*it)->getName() == name) {
            mChildren.erase(it);
			break;
		}
	}
	PH_LOG(LogType::Info, "Entity \"" + name + "\" was removed. It was a child of the \"" + mName + "\"");
}

void Entity::removeChild(Entity* pointerToChildWhichIsSupposedToBeRemoved)
{
	for (auto it = mChildren.begin(); it != mChildren.end(); ++it) {
		if ((*it).get() == pointerToChildWhichIsSupposedToBeRemoved) {
			mChildren.erase(it);
			break;
		}
	}
	PH_LOG(LogType::Info, "Entity \"" + pointerToChildWhichIsSupposedToBeRemoved->getName() + 
		                  "\" was removed. It was a child of the \"" + mName + "\"");
}

auto Entity::getChild(std::string name) const -> Entity&
{
    for(auto const &child : mChildren){
        if(child->getName() == name)
            return *(child.get());
    }
}