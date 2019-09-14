#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <list>
#include <memory>

namespace ph {

class GameObject : public sf::Drawable, public sf::Transformable
{
public:
	GameObject(const std::string& name);

	void handleEvent(const sf::Event&);
	void update(sf::Time delta);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void addChild(std::unique_ptr<GameObject>);
	void removeChild(const std::string& name);
	void removeChild(GameObject* childToRemove);
	void changeParentOfChild(GameObject* child, GameObject* newParent);

	auto getParent() const -> GameObject& { return *mParent; }
	auto getName() const -> const std::string& { return mName; }
	auto getChild(const std::string& name) const -> GameObject*;
	auto getChildren() const -> const std::list<std::unique_ptr<GameObject>>& { return mChildren; }

	static void setRoot(GameObject* const root) { GameObject::mRoot = root; }

	sf::Vector2f getWorldPosition() const;
	virtual sf::FloatRect getGlobalBounds() const;

protected:
	std::string getUniqueName(std::string& childName) const;
	void correctChildName(std::string& childName) const;
	void incrementNumberInChildName(std::string& childName) const;
	
	virtual void handleEventOnCurrent(const sf::Event&);
	void handleEventOnChildren(const sf::Event&);

	virtual void updateCurrent(sf::Time delta);
	void updateChildren(sf::Time delta);

	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	std::list<std::unique_ptr<GameObject>> mChildren;
	std::string mName;
	GameObject* mParent;
	inline static GameObject* mRoot;
};

}
