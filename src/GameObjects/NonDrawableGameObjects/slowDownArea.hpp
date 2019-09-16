#pragma once 

#include "GameObjects/gameObject.hpp"
#include "Utilities/rect.hpp"

namespace ph { 

class SlowDownArea : public GameObject
{
public:
	SlowDownArea(const sf::FloatRect& area);
	void updateCurrent(const sf::Time delta) override;

private:
	const FloatRect mArea;
};

} 
