#pragma once

#include <SFML/System.hpp>
#include <string>
#include <list>
#include <memory>

namespace ph {

class Entity
{
public:
	Entity(const std::string& name);

	virtual void input();
	virtual void update(sf::Time delta);

	void addChild(std::unique_ptr<Entity>);
	void removeChild(const std::string& name);
	void removeChild(Entity* childToRemove);

	auto getParent() const -> Entity& { return *mParent; }
	auto getName() const -> const std::string& { return mName; }
	auto getChild(const std::string& name) const -> Entity&;
	auto getChildren() const -> const std::list<std::unique_ptr<Entity>>& { return mChildren; }

protected:
	std::string getUniqueName(std::string& childName) const;
	void correctChildName(std::string& childName) const;
	void incrementNumberInChildName(std::string& childName) const;

protected:
	std::list<std::unique_ptr<Entity>> mChildren;
	std::string mName;
	Entity* mParent;
};

}
