#pragma once

#include "efficiencyDisplayer.hpp"
#include "Events/event.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

class GameData;

class EfficiencyRegister
{
public:
	EfficiencyRegister();
	void init(GameData* const gameData);

	void handleEvent(const ph::Event&);
	void update();

	void registerDrawCall();

	auto getDisplayer() -> EfficiencyDisplayer& { return mEfficiencyDisplayer; }

private:
	EfficiencyDisplayer mEfficiencyDisplayer;
	sf::Clock mClock;
	GameData* mGameData;
	unsigned mFramesPerSecond;
	unsigned mDrawCallPerFrame;
	unsigned mFramesFromLastSecond;
	bool mIsActive;
};

}
