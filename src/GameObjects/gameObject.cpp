#include "gameObject.hpp"
#include "Logs/logs.hpp"
#include <stdexcept>

namespace ph {

GameObject::GameObject(const std::string& name)
	:mName(name)
	,mParent(nullptr)
{
}

void GameObject::input()
{
	for(auto& child : mChildren)
		child->input();
}

void GameObject::update(sf::Time delta)
{
	updateCurrent(delta);
	updateChildren(delta);
}

void GameObject::updateCurrent(sf::Time delta)
{
}

void GameObject::updateChildren(sf::Time delta)
{
	for(auto& child : mChildren)
		child->update(delta);
}

void GameObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	drawCurrent(target, states);
	drawChildren(target, states);
}

void GameObject::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
}

void GameObject::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	for(const auto& child : mChildren)
		child->draw(target, states);
}

std::string GameObject::getUniqueName(std::string& childName) const
{
	for (const auto& child : mChildren)
		if (child->getName() == childName)
			while(child->getName() == childName)
				correctChildName(childName);
	return childName;
}

void GameObject::correctChildName(std::string& childName) const
{
	if (childName.find('_') != std::string::npos)
		incrementNumberInChildName(childName);
	else
		childName += "_2";
}

void GameObject::incrementNumberInChildName(std::string& childName) const
{
	std::size_t begin = childName.find('_');
	childName.replace(begin + 1, std::string::npos,
		std::to_string(std::stoi(childName.substr(begin + 1))+1));
}


void GameObject::addChild(std::unique_ptr<GameObject> newChild)
{
	const std::string nameOfNewChild = getUniqueName(newChild->mName);
	newChild->mName = nameOfNewChild;
    newChild->mParent = this;
    mChildren.emplace_back(std::move(newChild));
	PH_LOG_INFO("GameObject \"" + nameOfNewChild + "\" was added as child of the \"" + mName + "\"");
}

void GameObject::removeChild(const std::string& name)
{
	for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
		if ((*it)->getName() == name) {
            mChildren.erase(it);
			break;
		}

	PH_LOG_INFO("GameObject \"" + name + "\" was removed. It was a child of the \"" + mName + "\"");
}

void GameObject::removeChild(GameObject* childToRemove)
{
	PH_LOG_INFO("GameObject \"" + childToRemove->getName() + "\" was removed. It was a child of the \"" + mName + "\"");

	for(auto it = mChildren.begin(); it != mChildren.end(); ++it)
		if(it->get() == childToRemove) {
			mChildren.erase(it);
			break;
		}
}

void GameObject::changeParentOfChild(GameObject* child, GameObject* newParent)
{
	for (auto it = mChildren.begin(); it != mChildren.end(); ++it)
		if (it->get() == child)
		{
			newParent->addChild(std::move(*it));
			mChildren.erase(it);
			break;
		}

	PH_LOG_INFO("GameObject \"" + child->getName() + "\" became a child of the \"" + newParent->getName() + "\". It was a child of the \"" + mName + "\"");
}

auto GameObject::getChild(const std::string& name) const -> GameObject*
{
    for(auto const &child : mChildren)
        if(child->getName() == name)
            return child.get();
	return nullptr;
}

sf::Vector2f GameObject::getWorldPosition() const
{
	sf::Vector2f position;
	for(const GameObject* gameObject = this; gameObject != nullptr; gameObject = &gameObject->getParent())
		position += gameObject->getPosition();

	return position;
}

sf::FloatRect GameObject::getGlobalBounds() const
{
	sf::Vector2f worldPosition = getWorldPosition();
	return sf::FloatRect(worldPosition.x, worldPosition.y, 0, 0);
}

}