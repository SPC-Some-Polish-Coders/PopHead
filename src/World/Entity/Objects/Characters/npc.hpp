#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTERS_NPC_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTERS_NPC_H_

#include "World/Entity/Objects/character.hpp"

namespace PopHead{
namespace World{
namespace Entity{


class NPC : public Character
{
public:
    NPC(Base::GameData* gameData, std::string name);

    void input() override;
    void update(sf::Time delta) override;

    void talk();

private:
    bool wasDialogueButtonClicked;
};


}}}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTERS_NPC_H_
