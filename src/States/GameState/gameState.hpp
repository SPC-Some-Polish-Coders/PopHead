#pragma once

#include "States/state.hpp"
#include <SFML/Graphics.hpp>
#include "States/sceneParser.hpp"

namespace ph {

class GameState : public State
{
public:
	GameState(GameData* const);
private:
	void playMusic();
public:
	void input() override;
private:
	void windowMinimalizeAndMaximalizeShortcut();

public:
	void update(sf::Time delta) override;
};

}
