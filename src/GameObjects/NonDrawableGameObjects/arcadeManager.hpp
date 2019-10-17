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

	void updateEnemies();
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
	sf::Clock mArcadeClock;
	sf::Clock mTimeInCurrentPart;
	sf::Time mTimeForCurrentWave;
	sf::Time mTimeForBreak;
	int mNumberOfSpawnersOnTheMap;
	int mEnemiesToSpawn;
	int mSlowZombiesToSpawnPerSpawner;
	int mNormalZombiesToSpawnPerSpawner;
	int mEnemiesLeft;
	int mCurrentWave;
	bool mBreakTime;
	bool mMadeInit;
};

}
