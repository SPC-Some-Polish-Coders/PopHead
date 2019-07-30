#pragma once

#include <SFML/System.hpp>
#include <string>
#include <list>
#include <memory>

namespace ph {

class GameObject
{
public:
	GameObject(const std::string& name);

	virtual void input();
	virtual void update(sf::Time delta);

	void addChild(std::unique_ptr<GameObject>);
	void removeChild(const std::string& name);
	void removeChild(GameObject* childToRemove);

	auto getParent() const -> GameObject& { return *mParent; }
	auto getName() const -> const std::string& { return mName; }
	auto getChild(const std::string& name) const -> GameObject&;
	auto getChildren() const -> const std::list<std::unique_ptr<GameObject>>& { return mChildren; }

protected:
	std::string getUniqueName(std::string& childName) const;
	void correctChildName(std::string& childName) const;
	void incrementNumberInChildName(std::string& childName) const;

protected:
	std::list<std::unique_ptr<GameObject>> mChildren;
	std::string mName;
	GameObject* mParent;
};

}
