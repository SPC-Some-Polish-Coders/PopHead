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
	mTimeForBreak(sf::seconds(20.f))
{
	createNextWave();
}

void ArcadeManager::updateCurrent(const sf::Time delta)
{
	updateClocks();
	updateEnemies();
	updateWave();
	updateCounters();
}

void ArcadeManager::updateClocks()
{
	updateArcadeClock();
	if (mBreakTime)
		updateWaveClock();	
}

void ArcadeManager::updateArcadeClock()
{
	auto* arcadeInterface = mGui.getInterface("arcadeCounters");
	auto* counters = arcadeInterface->getWidget("canvas")->getWidget("counters");
	float elapsedTime = mArcadeClock.getElapsedTime().asSeconds();
	int elapsedTimeSeconds = static_cast<int>(elapsedTime);
	int elapsedTimeMinutes = static_cast<int>(elapsedTimeSeconds / 60);
	if (elapsedTimeSeconds >= 60) elapsedTimeSeconds -= elapsedTimeMinutes * 60;

	auto * arcadeClock = dynamic_cast<TextWidget*>(counters->getWidget("arcadeClock"));
	arcadeClock->setString("Time - " + addZero(elapsedTimeMinutes) + ":" + addZero(elapsedTimeSeconds));
}

void ArcadeManager::updateWaveClock()
{
	auto* arcadeInformations = mGui.getInterface("arcadeInformations");
	auto* informations = arcadeInformations->getWidget("canvas");
	auto* waveClock = dynamic_cast<TextWidget*>(informations->getWidget("nextWaveInfo"));
	int secondsUntilTheEnd = static_cast<int>(mTimeForBreak.asSeconds() - mTimeInCurrentPart.getElapsedTime().asSeconds());
	waveClock->setString("Next wave in " + addZero(secondsUntilTheEnd) + " seconds!");
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
	if (!mBreakTime && mTimeInCurrentPart.getElapsedTime().asSeconds() > mTimeForCurrentWave.asSeconds())
	{
		mTimeInCurrentPart.restart();
		startBreakTime();
	}

	else if(shouldCreateNewWave())
	{
		mTimeInCurrentPart.restart();
		endBreakTime();
		createNextWave();
	}
}

bool ArcadeManager::shouldCreateNewWave()
{
	if (mBreakTime)
		if (mTimeInCurrentPart.getElapsedTime().asSeconds() > mTimeForBreak.asSeconds())
			return true;
	else
		if (mTimeInCurrentPart.getElapsedTime().asSeconds() > mTimeForCurrentWave.asSeconds())
			return true;
	return false;
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
	mEnemiesToSpawn = (enemiesPerSpawner - rest > rest / 2 ? numberOfEnemies - rest : numberOfEnemies + (enemiesPerSpawner - rest));
	mEnemiesToSpawnPerSpawner = mEnemiesToSpawn / mNumberOfSpawnersOnTheMap;
}

void ArcadeManager::setTimeForTheNextWave()
{
	mTimeForCurrentWave = sf::seconds(mEnemiesToSpawn / 0.65) + sf::seconds(10.f);
}

void ArcadeManager::invokeSpawners()
{
	std::vector<ArcadeSpawner*> arcadeSpawners = getSpawners();
	sf::Time timeBetweenSpawns = sf::seconds((mTimeForCurrentWave.asSeconds() / mEnemiesToSpawnPerSpawner));
	for (unsigned i = 0; i < arcadeSpawners.size(); ++i)
		arcadeSpawners[i]->invokeSpawner(timeBetweenSpawns, mEnemiesToSpawnPerSpawner);
}

void ArcadeManager::startBreakTime()
{
	mBreakTime = true;
	auto* arcadeInterface = mGui.getInterface("arcadeInformations");
	arcadeInterface->show();
}

void ArcadeManager::endBreakTime()
{
	mBreakTime = false;
	auto* arcadeInterface = mGui.getInterface("arcadeInformations");
	arcadeInterface->show();
}

void ArcadeManager::updateCounters()
{
	auto* arcadeInterface = mGui.getInterface("arcadeCounters");
	auto* counters = arcadeInterface->getWidget("canvas")->getWidget("counters");

	auto* waveCounter = dynamic_cast<TextWidget*>(counters->getWidget("waveCounter"));
	waveCounter->setString("Wave: " + addZero(mCurrentWave));
	auto * enemiesCounter = dynamic_cast<TextWidget*>(counters->getWidget("enemiesCounter"));
	enemiesCounter->setString("Enemies: " + addZero(mEnemiesLeft));
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

std::string ArcadeManager::addZero(int number)
{
	if (number < 10)
		return "0" + std::to_string(number);
	else
		return std::to_string(number);
}

}