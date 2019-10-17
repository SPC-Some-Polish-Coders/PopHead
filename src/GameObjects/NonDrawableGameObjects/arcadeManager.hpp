#pragma once

#include "GameObjects/gameObject.hpp"

#include <map>

namespace ph {

class GUI;
class Widget;
class MusicPlayer;
class ArcadeSpawner;

class ArcadeManager : public GameObject
{
public:
	ArcadeManager(GUI& gui, MusicPlayer&);
	~ArcadeManager();

	void updateCurrent(const sf::Time delta) override;

	static bool isActive() { return mIsActive; }

private:
	void init();
	void startArcadeMode();
	void updateStartTimeCounter();

	void updateEnemiesCounter();
	void updateWave();
	void updateCounters();

	std::string getArcadeClockValues();
	std::string getTimeToNextWaveInfo();

	bool shouldCreateNewWave();
	void createNextWave();
	void invokeSpawners();

	void startBreakTime();
	void endBreakTime();

	void setNextWaveNumbers();

	std::string addZero(int number);
	int getNumberOfSpawners();
	auto getSpawners()->std::vector<ArcadeSpawner*>;

private:
	GUI& mGui;
	MusicPlayer& mMusicPlayer;
	sf::Clock mBreakClock;
	sf::Time mTimeFromStart;
	sf::Time mTimeBeforeStart;
	int mNumberOfSpawnersOnTheMap;
	int mEnemiesToSpawn;
	int mSlowZombiesToSpawnPerSpawner;
	int mNormalZombiesToSpawnPerSpawner;
	int mEnemiesCounter;
	int mCurrentWave;
	bool mIsBreakTime;
	bool mMadeInit;
	bool mHasStarted;

	inline static bool mIsActive = false;
};

}
