#pragma once

#include <string>
#include <list>
#include <memory>

#include <SFML/System.hpp>

#include "EntityComponentSystem/entityType.hpp"

namespace ph {

class GameData;

class Entity
{
public:
	using EntityPtr = std::unique_ptr<Entity>;

	Entity(EntityType, GameData*, std::string name);

	virtual void input();
	virtual void update(sf::Time delta);

	void addChild(EntityPtr);
	void removeChild(const std::string& name);
	void removeChild(Entity* pointerToChildWhichIsSupposedToBeRemoved);

	auto getParent() const -> Entity& { return *mParent; }
	auto getName() const -> const std::string& { return mName; }
	auto getChild(std::string name) const->Entity&;

protected:
	std::string checkName(std::string&);
	void correctChildName(std::string&);
	void incrementNumber(std::string&);

protected:
	GameData* mGameData;

	const EntityType mEntityType;

	Entity* mParent;
	std::list< std::unique_ptr<Entity> > mChildren;

	std::string mName;

};

}
