#pragma once

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
