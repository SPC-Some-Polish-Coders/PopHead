#pragma once

#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>

namespace ph {

class FPSCounter
{
public:
	FPSCounter();

	void handleEvent(sf::Event);
	void update();

private:
	sf::Clock mClock;
	unsigned mFPS;
	unsigned mFramesFromLastSecond;
	bool mIsActive;
};

}

