#pragma once

#include "Events/event.hpp"
#include <SFML/System/Clock.hpp>

namespace ph {

class FPSCounter
{
public:
	FPSCounter();

	void handleEvent(Event);
	void update();

private:
	sf::Clock mClock;
	unsigned mFPS;
	unsigned mFramesFromLastSecond;
	bool mIsActive;
};

}

