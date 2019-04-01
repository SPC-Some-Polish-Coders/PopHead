#ifndef POPHEAD_WORLD_ENTITY_H_
#define POPHEAD_WORLD_ENTITY_H_

#include <string>
#include <list>
#include <memory>

#include "entityType.hpp"
#include "Base/gameData.hpp"

namespace PopHead {
namespace World {
namespace Entity {


class Entity
{
public:
    using EntityPtr = std::unique_ptr<Entity>;

    Entity();
    Entity(EntityType, Base::GameData*, std::string name);

    virtual void input();
    virtual void update(sf::Time delta);

    void addChild(EntityPtr);
    void removeChild(const std::string& name);
    void removeChild(unsigned int id);
    void removeChild(EntityType);
    void setName(const std::string& name);

    auto getEntityType() const -> EntityType;
    auto getID() const -> unsigned int;
    auto getParent() const -> Entity&;
    auto getChild(std::string name) const -> Entity&;
    auto getChildren() -> std::list< std::unique_ptr<Entity> >&;
    auto getName() const -> const std::string&;

private:
    const EntityType mEntityType;
    unsigned int mID;
    std::string mName;

    Entity* mParent;
    std::list< std::unique_ptr<Entity> > mChildren;

    Base::GameData* mGameData;
};


}}}

#endif // !POPHEAD_WORLD_ENTITY_H_
