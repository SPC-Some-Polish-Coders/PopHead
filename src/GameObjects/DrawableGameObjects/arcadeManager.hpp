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
	void updateClocks(const sf::Time delta);
	void updateWave();
	void updateEnemies();
	bool shouldCreateNewWave();
	void createNextWave();
	void invokeSpawners();
	void updateCounters();
	int getNumberOfSpawners();

	void startBreakTime();
	void endBreakTime();

	void setSpawnNumbers();
	void setTimeForTheNextWave();

	bool isInteger(float number);
	std::string addZero(int number);
	auto getSpawners()->std::vector<ArcadeSpawner*>;

private:
	GUI& mGui;
	sf::Clock mArcadeClock;
	sf::Clock mTimeInTheCurrentWave;
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