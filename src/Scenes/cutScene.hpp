#pragma once

#include <SFML/System.hpp>

namespace ph {

class CutScene
{
public:
	CutScene();
	virtual void update(const sf::Time dt) = 0;
	bool isActive() const;

protected:
	bool mIsActive;
};

}