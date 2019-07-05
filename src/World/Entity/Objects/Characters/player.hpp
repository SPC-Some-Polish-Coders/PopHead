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
	void movementInput();
	void gunInput();
    void updateAnimation(const std::string& stateName);
};


}
