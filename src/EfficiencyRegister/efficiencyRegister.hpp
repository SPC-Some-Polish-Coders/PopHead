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

	void input();
	void update();

	void registerRenderCall() { ++mRenderCallPerFrame; }

	auto getDisplayer() -> EfficiencyDisplayer& { return mEfficiencyDisplayer; }

private:
	EfficiencyDisplayer mEfficiencyDisplayer;
	sf::Clock mClock;
	GameData* mGameData;
	unsigned mFramesPerSecond;
	unsigned mRenderCallPerFrame;
	unsigned mFramesFromLastSecond;
	bool mIsActive;
	};

};