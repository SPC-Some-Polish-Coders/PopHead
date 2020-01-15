#pragma once

#include "Events/event.hpp"
#include "Resources/resourceHolder.hpp"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Clock.hpp>
#include <memory>

namespace ph {

class FPSCounter
{
public:
	FPSCounter();

	void handleEvent(const ph::Event&);
	void sampleFrame();
	void update();

private:
	sf::Clock mClock;
	unsigned mFPS;
	unsigned mFramesFromLastSecond;
	bool mIsActive;
};

}

