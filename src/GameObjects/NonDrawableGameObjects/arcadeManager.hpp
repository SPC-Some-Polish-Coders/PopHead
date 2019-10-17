#pragma once

#include "GameObjects/gameObject.hpp"

#include <map>

namespace ph {

class GUI;
class Widget;
class ArcadeSpawner;

class ArcadeManager : public GameObject
{
public:
	ArcadeManager(GUI& gui);
	void updateCurrent(const sf::Time delta) override;

private:
	void init();

	void updateEnemiesCounter();
	void updateWave();
	void updateCounters();

	std::string getArcadeClockValues();
	std::string getWaveClockValues();

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
	sf::Clock mBreakClock;
	sf::Time mTimeFromStart;
	int mNumberOfSpawnersOnTheMap;
	int mEnemiesToSpawn;
	int mSlowZombiesToSpawnPerSpawner;
	int mNormalZombiesToSpawnPerSpawner;
	int mEnemiesCounter;
	int mCurrentWave;
	bool mIsBreakTime;
	bool mMadeInit;
};

}
