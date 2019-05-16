#ifndef POPHEAD_WORLD_ENTITY_H_
#define POPHEAD_WORLD_ENTITY_H_

#include <string>
#include <list>
#include <memory>

#include <SFML/System.hpp>

#include "World/Entity/entityType.hpp"

namespace PopHead {

namespace Base { class GameData; }

namespace World {
namespace Entity {


class Entity
{
public:
    using EntityPtr = std::unique_ptr<Entity>;

    Entity(EntityType, PopHead::Base::GameData*, std::string name);

    virtual void input();
    virtual void update(sf::Time delta);

    void addChild(EntityPtr);
    void removeChild(const std::string& name);
    void removeChild(Entity* pointerToChildWhichIsSupposedToBeRemoved);

	auto getParent() const -> Entity& { return *mParent; }
	auto getName() const -> const std::string& { return mName; }
    auto getChild(std::string name) const -> Entity&;
	
protected:
    Base::GameData* mGameData;

    const EntityType mEntityType;
    std::string mName;

    Entity* mParent;
    std::list< std::unique_ptr<Entity> > mChildren;
};


}}}

#endif // !POPHEAD_WORLD_ENTITY_H_
