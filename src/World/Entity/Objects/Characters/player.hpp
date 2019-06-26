#pragma once

#include "World/Entity/Objects/character.hpp"

namespace ph{

class Player : public Character
{
public:
    Player(GameData*);

    void input() override;
    void update(sf::Time delta) override;

private:
    void updateAnimation(const std::string& stateName);
    ///void talkTo(Npc&);

    ///auto getPerks() const -> const PerkManager&;

private:
    ///PerkManager mPerks;
    ///AttributesManager mAttributes;
};


}
