#pragma once

#include "GameObjects/DrawableGameObjects/character.hpp"

namespace ph{

class Npc : public Character
{
public:
    Npc(GameData*);

    void input() override;
    void update(sf::Time delta) override;

    void talk();

private:
    bool mWasDialogueButtonClicked = false;
};

}
