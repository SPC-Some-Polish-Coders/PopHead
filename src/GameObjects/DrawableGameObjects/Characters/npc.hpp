#pragma once

#include "GameObjects/DrawableGameObjects/character.hpp"

namespace ph{

class Npc : public Character
{
public:
    Npc(GameData*);

    void update(sf::Time delta) override;
};

}
