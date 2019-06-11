#include "entity.hpp"
#include "Utilities/debug.hpp"

ph::Entity::Entity(EntityType type, GameData* gameData, std::string name)
	:mEntityType(type)
	,mGameData(gameData)
	,mName(name)
	,mParent(nullptr)
{

}

void ph::Entity::input()
{
    for(auto it = mChildren.begin(); it != mChildren.end(); ++it)
        (*it)->input();
}

void ph::Entity::update(sf::Time delta)
{
	for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
	{
		(*it)->update(delta);
	}
}

std::string ph::Entity::checkName(const std::string& childName)
{
	for (const auto& child : mChildren)
	{
		if (child->getName()== childName)
			PH_LOG(LogType::Info, "WORKS!");
		//childName=someFuncHere
	}
	return childName;
}

void ph::Entity::addChild(EntityPtr newChild)
{
	const std::string nameOfNewChild = checkName(newChild->getName());
    newChild->mParent = this;
    mChildren.emplace_back(std::move(newChild));
	PH_LOG(LogType::Info, "Entity \"" + nameOfNewChild + "\" was added as child of the \"" + mName + "\"");
}

void ph::Entity::removeChild(const std::string& name)
{
	for (auto it = mChildren.begin(); it != mChildren.end(); ++it) {
		if ((*it)->getName() == name) {
            mChildren.erase(it);
			break;
		}
	}
	PH_LOG(LogType::Info, "Entity \"" + name + "\" was removed. It was a child of the \"" + mName + "\"");
}

void ph::Entity::removeChild(Entity* pointerToChildWhichIsSupposedToBeRemoved)
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

auto ph::Entity::getChild(std::string name) const -> Entity&
{
    for(auto const &child : mChildren){
        if(child->getName() == name)
            return *(child.get());
    }
}