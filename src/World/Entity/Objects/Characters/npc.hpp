#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTERS_NPC_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTERS_NPC_H_

#include "World/Entity/Objects/character.hpp"

namespace ph{

class Npc : public Character
{
public:
    Npc(GameData* gameData, std::string name);

    void input() override;
    void update(sf::Time delta) override;

    void talk();

private:
    bool mWasDialogueButtonClicked = false;
};

}

#endif // POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTERS_NPC_H_
