#pragma once

#include "efficiencyDisplayer.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

class GameData;

class EfficiencyRegister
{
public:
	EfficiencyRegister();
	void init(GameData* const gameData);

	void update();

	void registerDrawCall() { ++mDrawCallPerFrame; }

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
