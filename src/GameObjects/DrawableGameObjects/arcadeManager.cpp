#include "GameObjects/DrawableGameObjects/arcadeManager.hpp"
#include "GameObjects/NonDrawableGameObjects/arcadeSpawner.hpp"
#include "Gui/gui.hpp"
#include "Gui/interface.hpp"
#include <math.h>

namespace ph {

ArcadeManager::ArcadeManager(GUI& gui)
	:GameObject("arcadeTimer")
	,mGui(gui),
	mEnemiesToSpawn(0),
	mEnemiesToSpawnPerSpawner(0),
	mTimeForCurrentWave(sf::Time::Zero),
	mCurrentWave(0),
	mEnemiesLeft(0),
	mNumberOfSpawnersOnTheMap(getNumberOfSpawners()),
	mBreakTime(false),
	mTimeForBreak(sf::seconds(10.f))
{
	auto* arcadeInterface = mGui.getInterface("arcadeInformations");
	auto* counters = arcadeInterface->getWidget("canvas");
	counters->getWidget("breakInfo")->hide();
	counters->getWidget("nextWaveInfo")->hide();
	createNextWave();
}

int ArcadeManager::getNumberOfSpawners()
{
	auto* invisibleObjects = mRoot->getChild("LAYER_invisibleObjects");
	auto& gameObjects = invisibleObjects->getChildren();
	int counter = 0;
	for (const auto& gameObject : gameObjects)
		if (gameObject->getName().find("arcadeSpawner") != std::string::npos)
			++counter;
	return counter;
}

void ArcadeManager::updateCurrent(const sf::Time delta)
{
	updateClocks(delta);
	updateEnemies();
	updateWave();
	updateCounters();
}

void ArcadeManager::updateClocks(const sf::Time delta)
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
	auto* standingObjects = mRoot->getChild("LAYER_standingObjects");
	auto& gameObjects = standingObjects->getChildren();
	mEnemiesLeft = 0;
	for (const auto& gameObject : gameObjects)
		if (gameObject->getName().find("zombie") != std::string::npos)
			++mEnemiesLeft;
}

void ArcadeManager::updateWave()
{
	if (!mBreakTime && mTimeInTheCurrentWave.getElapsedTime().asSeconds() > mTimeForCurrentWave.asSeconds())
	{
		mTimeInTheCurrentWave.restart();
		startBreakTime();
	}

	if (shouldCreateNewWave())
	{
		endBreakTime();
		mTimeInTheCurrentWave.restart();
		createNextWave();
	}
}

void ArcadeManager::startBreakTime()
{
	mBreakTime = true;
	auto* arcadeInterface = mGui.getInterface("arcadeInformations");
	auto* counters = arcadeInterface->getWidget("canvas");
	counters->getWidget("breakInfo")->show();
	counters->getWidget("nextWaveInfo")->show();
}

void ArcadeManager::endBreakTime()
{
	mBreakTime = false;
	auto* arcadeInterface = mGui.getInterface("arcadeInformations");
	auto* counters = arcadeInterface->getWidget("canvas");
	counters->getWidget("breakInfo")->hide();
	counters->getWidget("nextWaveInfo")->hide();
}

bool ArcadeManager::shouldCreateNewWave()
{
	bool boolean = false;
	if (mBreakTime)
	{
		if (mTimeInTheCurrentWave.getElapsedTime().asSeconds() > mTimeForBreak.asSeconds())
			boolean = true;
	}
	else
		if (mTimeInTheCurrentWave.getElapsedTime().asSeconds() > mTimeForCurrentWave.asSeconds())
			boolean = true;

	return boolean;
}

void ArcadeManager::createNextWave()
{
	++mCurrentWave;
	setSpawnNumbers();
	setTimeForTheNextWave();
	invokeSpawners();
}

void ArcadeManager::setSpawnNumbers()
{
	int numberOfEnemies = mNumberOfSpawnersOnTheMap * mCurrentWave * 1.2;
	int enemiesPerSpawner = numberOfEnemies / mNumberOfSpawnersOnTheMap;
	int rest = numberOfEnemies - enemiesPerSpawner * mNumberOfSpawnersOnTheMap;
	mEnemiesToSpawn = (enemiesPerSpawner - rest > rest / 2 ?  numberOfEnemies - rest : numberOfEnemies + (enemiesPerSpawner - rest));
	mEnemiesToSpawnPerSpawner = mEnemiesToSpawn / mNumberOfSpawnersOnTheMap;
}

void ArcadeManager::setTimeForTheNextWave()
{
	mTimeForCurrentWave = sf::seconds(mEnemiesToSpawn / 0.65)+sf::seconds(10.f);
}

void ArcadeManager::invokeSpawners()
{
	std::vector<ArcadeSpawner*> arcadeSpawners = getSpawners();
	sf::Time timeBetweenSpawns = sf::seconds((mTimeForCurrentWave.asSeconds()/mEnemiesToSpawnPerSpawner));
	for (unsigned i = 0; i < arcadeSpawners.size(); ++i)
		arcadeSpawners[i]->invokeSpawner(timeBetweenSpawns, mEnemiesToSpawnPerSpawner);
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

auto ArcadeManager::getSpawners() -> std::vector<ArcadeSpawner*>
{
	auto* invisibleObjects = mRoot->getChild("LAYER_invisibleObjects");
	auto& gameObjects = invisibleObjects->getChildren();
	std::vector<ArcadeSpawner*> arcadeSpawners;
	for (const auto& gameObject : gameObjects)
		if (gameObject->getName().find("arcadeSpawner") != std::string::npos)
			arcadeSpawners.emplace_back(dynamic_cast<ArcadeSpawner*>(gameObject.get()));
	return arcadeSpawners;
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