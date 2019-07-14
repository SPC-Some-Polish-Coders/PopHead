#include "entity.hpp"
#include "Utilities/debug.hpp"

namespace ph {

Entity::Entity(GameData* const gameData, const std::string& name)
	:mName(name)
	,mParent(nullptr)
	,mGameData(gameData)
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
		(*it)->update(delta);
}

std::string Entity::getUniqueName(std::string& childName) const
{
	for (const auto& child : mChildren)
		if (child->getName() == childName)
			while(child->getName() == childName)
				correctChildName(childName);
	return childName;
}

void Entity::correctChildName(std::string& childName) const
{
	if (childName.find('_') != std::string::npos)
		incrementNumberInChildName(childName);
	else
		childName += "_2";
}

void Entity::incrementNumberInChildName(std::string& childName) const
{
	std::size_t begin = childName.find('_');
	childName.replace(begin + 1, std::string::npos,
		std::to_string(std::stoi(childName.substr(begin + 1))+1));
}

void Entity::addChild(EntityPtr newChild)
{
	const std::string nameOfNewChild = getUniqueName(newChild->mName);
	newChild->mName = nameOfNewChild;
    newChild->mParent = this;
    mChildren.emplace_back(std::move(newChild));
	PH_LOG(LogType::Info, "Entity \"" + nameOfNewChild + "\" was added as child of the \"" + mName + "\"");
}

void Entity::removeChild(const std::string& name)
{
	for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
		if ((*it)->getName() == name) {
            mChildren.erase(it);
			break;
		}

	PH_LOG(LogType::Info, "Entity \"" + name + "\" was removed. It was a child of the \"" + mName + "\"");
}

void Entity::removeChild(Entity* childToRemove)
{
	for(auto it = mChildren.begin(); it != mChildren.end(); ++it)
		if(it->get() == childToRemove) {
			mChildren.erase(it);
			break;
		}

	PH_LOG(LogType::Info, "Entity \"" + childToRemove->getName() + "\" was removed. It was a child of the \"" + mName + "\"");
}

auto Entity::getChild(const std::string& name) const -> Entity&
{
    for(auto const &child : mChildren)
        if(child->getName() == name)
            return *(child.get());
}

}