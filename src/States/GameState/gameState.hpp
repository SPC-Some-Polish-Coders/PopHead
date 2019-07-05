#pragma once

#include "States/state.hpp"
#include <SFML/Graphics.hpp>
#include "States/sceneParser.hpp"

namespace ph {

class GameState : public State
{
public:
	GameState(GameData* const);

	void input() override;
	void update(sf::Time delta) override;

private:
	void windowMinimalizeAndMaximalizeShortcut();
};

}
