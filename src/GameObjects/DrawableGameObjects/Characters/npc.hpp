#pragma once

#include "GameObjects/DrawableGameObjects/character.hpp"

namespace ph{

class Npc : public Character
{
public:
    Npc(GameData* const, const std::string& name = "npc", 
		const std::string& texturePath = "textures/characters/negroDudeWalkingAnimation.png");

    void updateCurrent(sf::Time delta) override;
};

}
