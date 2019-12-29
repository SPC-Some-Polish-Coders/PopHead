#pragma once

#include "ECS/system.hpp"

namespace ph {
	class GUI;
	class MusicPlayer;
}

namespace ph::system {

class ArcadeMode : public System
{
public:
	ArcadeMode(entt::registry&, GUI&, MusicPlayer&);

	void update(float dt) override;

private:
	void updateGuiCounters();

	void createNextWave();

	void startBreakTime();
	void endBreakTime();

	void setNextWaveNumbers();

	std::string addZero(int number);
	int getNumberOfSpawners();

private:
	GUI& mGui;
	MusicPlayer& mMusicPlayer;
	sf::Clock mBreakClock;
	float mTimeFromStart = 0.f;
	float mTimeBeforeStartingFirstWave = 10.f;
	int mNumberOfSpawnersOnTheMap;
	int mEnemiesToSpawn = 0;
	int mSlowZombiesToSpawnPerSpawner = 0;
	int mNormalZombiesToSpawnPerSpawner = 0;
	int mEnemiesCounter = 0;
	int mCurrentWave = 0;
	bool mIsBreakTime = false;
	bool mMadeInit = false;
	bool mHasStartedFirstWave = false;
	bool mHasWon = false;
	bool mShouldSpawnEnemies = false;

	inline static bool mIsActive = false;
};

}

