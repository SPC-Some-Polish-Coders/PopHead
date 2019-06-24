#pragma once

#include "efficencyDisplayer.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

class GameData;

class EfficencyRegister
{
public:
	EfficencyRegister();
	void init(GameData* const gameData);

	void input();
	void update();

	void registerRenderCall() { ++mRenderCallPerFrame; }

	auto getDisplayer() -> EfficencyDisplayer& { return mEfficencyDisplayer; }

private:
	EfficencyDisplayer mEfficencyDisplayer;
	sf::Clock mClock;
	GameData* mGameData;
	unsigned mFramesPerSecond;
	unsigned mRenderCallPerFrame;
	unsigned mFramesFromLastSecond;
	bool mIsActive;
	};

};