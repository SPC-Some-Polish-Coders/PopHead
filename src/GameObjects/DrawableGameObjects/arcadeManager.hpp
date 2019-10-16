#pragma once

#include "GameObjects/gameObject.hpp"

#include <map>

namespace ph {

class GUI;
class ArcadeSpawner;

class ArcadeManager : public GameObject
{
public:
	ArcadeManager(GUI& gui);
	void updateCurrent(const sf::Time delta) override;

private:
	void updateClocks();
	void updateArcadeClock();
	void updateWaveClock();
	void updateEnemies();
	void updateWave();
	void updateCounters();

	bool shouldCreateNewWave();
	void createNextWave();
	void invokeSpawners();

	void startBreakTime();
	void endBreakTime();

	void setSpawnNumbers();
	void setTimeForTheNextWave();

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
	int mEnemiesToSpawnPerSpawner;
	int mEnemiesLeft;
	int mCurrentWave;
	bool mBreakTime;
};

}