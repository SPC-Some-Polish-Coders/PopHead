#include "GameObjects/DrawableGameObjects/arcadeManager.hpp"
#include "GameObjects/NonDrawableGameObjects/arcadeSpawner.hpp"
#include "Gui/gui.hpp"
#include "Gui/interface.hpp"
#include <math.h>

namespace ph {

ArcadeManager::ArcadeManager(GUI& gui)
	:GameObject("arcadeTimer")
	,mGui(gui)
	, mWavesMap{	{0, {0, sf::Time(sf::seconds(10.f))}},
					{1, {10, sf::Time(sf::seconds(20.f))}},
					{2, {15, sf::Time(sf::seconds(25.f))}},
					{3, {25, sf::Time(sf::seconds(40.f))}}, 
					{4, {30, sf::Time(sf::seconds(60.f))}}, 
					{5, {45, sf::Time(sf::seconds(80.f))}}, 
	},
	mCurrentWave(0),
	mEnemiesLeft(0)
{
}

void ArcadeManager::updateCurrent(const sf::Time delta)
{
	updateClock();
	updateEnemies();
	updateWave();
	updateCounters();
}

void ArcadeManager::updateClock()
{
	auto* arcadeInterface = mGui.getInterface("arcadeCounters");
	auto* counters = arcadeInterface->getWidget("canvas")->getWidget("counters");
	float elapsedTime = mArcadeClock.getElapsedTime().asSeconds();
	int elapsedTimeSeconds = static_cast<int>(elapsedTime);
	int elapsedTimeMiliseconds = static_cast<int>((elapsedTime - elapsedTimeSeconds) * 100);
	int elapsedTimeMinutes = static_cast<int>(elapsedTimeSeconds/60);
	if (elapsedTimeSeconds >= 60) elapsedTimeSeconds -= elapsedTimeMinutes*60;

	auto * arcadeClock = dynamic_cast<TextWidget*>(counters->getWidget("arcadeClock"));
	arcadeClock->setString("Time - " + addZero(elapsedTimeMinutes) + ":" + addZero(elapsedTimeSeconds) + ":" + addZero(elapsedTimeMiliseconds));
}

void ArcadeManager::updateEnemies()
{
	auto& root = getParent().getParent();
	auto* standingObjects = root.getChild("LAYER_standingObjects");
	auto& gameObjects = standingObjects->getChildren();
	mEnemiesLeft = 0;
	for (const auto& gameObject : gameObjects)
		if (gameObject->getName().find("zombie") != std::string::npos)
			++mEnemiesLeft;
}

void ArcadeManager::updateWave()
{
	if (shouldCreateNewWave())
	{
		++mCurrentWave;
		mTimeOnCurrentWave.restart();
		invokeSpawners();
	}
}

bool ArcadeManager::shouldCreateNewWave()
{
	if (mTimeOnCurrentWave.getElapsedTime().asSeconds() > mWavesMap.at(mCurrentWave).second.asSeconds())
		return true;
	else
		return false;
}

void ArcadeManager::invokeSpawners()
{
	auto& root = getParent().getParent();
	auto* invisibleObjects = root.getChild("LAYER_invisibleObjects");
	auto& gameObjects = invisibleObjects->getChildren();
	std::vector<ArcadeSpawner*> arcadeSpawners;
	for (const auto& gameObject : gameObjects)
		if (gameObject->getName().find("arcadeSpawner") != std::string::npos)
			arcadeSpawners.emplace_back(dynamic_cast<ArcadeSpawner*>(gameObject.get()));

	int numberOfSpawners = arcadeSpawners.size();
	numberOfSpawners = getNumberOfUsedSpawners(numberOfSpawners);
	int numberOfSpawnsPerSpawner = mWavesMap.at(mCurrentWave).first / numberOfSpawners;
	sf::Time timeBetweenSpawns = sf::seconds((mWavesMap.at(mCurrentWave).second.asSeconds()) / numberOfSpawnsPerSpawner);
	for (int i = 0; i < numberOfSpawners; ++i)
		arcadeSpawners[i]->invokeSpawner(timeBetweenSpawns, numberOfSpawnsPerSpawner);
}

int ArcadeManager::getNumberOfUsedSpawners(float numberOfSpawners)
{
	if (mWavesMap.at(mCurrentWave).first < numberOfSpawners)
		numberOfSpawners = mWavesMap.at(mCurrentWave).first;
	 float numberOfSpawnsPerSpawner = mWavesMap.at(mCurrentWave).first / numberOfSpawners;
	 while (!isInteger(numberOfSpawnsPerSpawner))
	 {
		--numberOfSpawners;
		numberOfSpawnsPerSpawner = mWavesMap.at(mCurrentWave).first / numberOfSpawners;
	 }
	 return static_cast<int>(numberOfSpawners);
}

void ArcadeManager::updateCounters()
{
	auto* arcadeInterface = mGui.getInterface("arcadeCounters");
	auto* counters = arcadeInterface->getWidget("canvas")->getWidget("counters");

	auto* waveCounter = dynamic_cast<TextWidget*>(counters->getWidget("waveCounter"));
	waveCounter->setString("Wave: " + addZero(mCurrentWave));
	auto* enemiesCounter = dynamic_cast<TextWidget*>(counters->getWidget("enemiesCounter"));
	enemiesCounter->setString("Enemies: " + addZero(mEnemiesLeft));
}

bool ArcadeManager::isInteger(float number)
{
	return std::floor(number) == number;
}

std::string ArcadeManager::addZero(int number)
{
	if (number < 10)
		return "0" + std::to_string(number);
	else
		return std::to_string(number);
}

}