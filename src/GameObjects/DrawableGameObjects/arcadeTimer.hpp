#pragma once

#include "GameObjects/gameObject.hpp"

namespace ph {

class GUI;

class ArcadeTimer : public GameObject
{
public:
	ArcadeTimer(GUI& gui);
	void drawCurrent(sf::RenderTarget&, const sf::RenderStates) const override {}
	void updateCurrent(const sf::Time delta) override;

private:
	GUI& mGui;
	sf::Clock mArcadeClock;
};

}