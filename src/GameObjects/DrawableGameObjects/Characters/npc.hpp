#pragma once

#include "GameObjects/DrawableGameObjects/character.hpp"

namespace ph{

class Npc : public Character
{
public:
    Npc(GameData* const, const std::string& name = "npc");

    void updateCurrent(sf::Time delta) override;
};

}
