#pragma once 

#include "GameObjects/gameObject.hpp"

namespace ph { 

class SlowDownArea : public GameObject
{
public:
	SlowDownArea(const sf::FloatRect& area);
	void updateCurrent(const sf::Time delta) override;

private:
	const sf::FloatRect mArea;
};

} 
