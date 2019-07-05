#include "entity.hpp"
#include "Utilities/debug.hpp"

namespace ph {

Entity::Entity(EntityType type, GameData* gameData, std::string name)
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
	for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
	{
		(*it)->update(delta);
	}
}

std::string Entity::checkName(std::string& childName)
{
	for (const auto& child : mChildren)
	{
		if (child->getName() == childName)
		{
			while (child->getName() == childName)
				correctChildName(childName);
		}
	}
	return childName;
}

void Entity::correctChildName(std::string& childNameToCorrect)
{
	if (childNameToCorrect.find('_') != std::string::npos)
		incrementNumber(childNameToCorrect);
	else
		childNameToCorrect += "_2";
}

void Entity::incrementNumber(std::string& childNameToIncrement)
{
	std::size_t begin = childNameToIncrement.find('_');
	childNameToIncrement.replace(begin + 1, std::string::npos,
		std::to_string(std::stoi(childNameToIncrement.substr(begin + 1))+1));
}

void Entity::addChild(EntityPtr newChild)
{
	const std::string nameOfNewChild = checkName(newChild->mName);
	newChild->mName = nameOfNewChild;
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

}