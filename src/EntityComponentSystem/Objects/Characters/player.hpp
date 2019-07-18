#pragma once

#include "EntityComponentSystem/Objects/character.hpp"

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
	void cameraMovement(sf::Time delta) const;
	void updateListenerPosition() const;
};

}
