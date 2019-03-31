#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTERS_PLAYER_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTERS_PLAYER_H_

#include "World/Entity/Objects/character.hpp"

namespace PopHead{
namespace World{
namespace Entity{


class Player : public Character
{
public:
    void input() override;
    void update(sf::Time delta) override;
    ///void talkTo(NPC&);

    ///auto getPerks() const -> const PerkManager&;

private:
    ///PerkManager mPerks;
    ///AttributesManager mAttributes;
};


}}}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTERS_PLAYER_H_
