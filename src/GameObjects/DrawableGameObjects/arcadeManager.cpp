#include "GameObjects/DrawableGameObjects/arcadeManager.hpp"
#include "Gui/gui.hpp"
#include "Gui/interface.hpp"
#include <math.h>

namespace ph {

ArcadeManager::ArcadeManager(GUI& gui)
	:GameObject("arcadeTimer")
	,mGui(gui)
	, mWavesMap{	{1, {10, sf::Time(sf::seconds(20.f))}},
					{2, {25, sf::Time(sf::seconds(30.f))}},
					{3, {50, sf::Time(sf::seconds(40.f))}} 
	},
	mCurrentWave(1),
	mEnemiesLeft(1)
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

	auto * arcadeClock = dynamic_cast<TextWidget*>(counters->getWidget("arcadeClock"));
	if (elapsedTimeMiliseconds < 10 || elapsedTimeSeconds < 10)
		arcadeClock->setString("Time - 0" + std::to_string(elapsedTimeSeconds) + ":0" + std::to_string(elapsedTimeMiliseconds));
	if (elapsedTimeSeconds < 10)
		arcadeClock->setString("Time - 0" + std::to_string(elapsedTimeSeconds) + ":" + std::to_string(elapsedTimeMiliseconds));
	else if (elapsedTimeMiliseconds < 10)
		arcadeClock->setString("Time - " + std::to_string(elapsedTimeSeconds) + ":0" + std::to_string(elapsedTimeMiliseconds));
	else
		arcadeClock->setString("Time - " + std::to_string(elapsedTimeSeconds) + ":" + std::to_string(elapsedTimeMiliseconds));
}

bool ArcadeManager::shouldCreateNewWave()
{
	if (mTimeOnCurrentWave.getElapsedTime().asSeconds() > mWavesMap.at(mCurrentWave).second.asSeconds() || mEnemiesLeft == 0)
		return true;
	else
		return false;
}

void ArcadeManager::updateWave()
{
	if (shouldCreateNewWave())
	{
		++mCurrentWave;
		mTimeOnCurrentWave.restart();
		//invokeSpawners();
		//randomDrop();
	}
}

void ArcadeManager::updateEnemies()
{
	auto& root = getParent().getParent();
	auto* standingObjects = root.getChild("LAYER_standingObjects");
	auto& gameObjects = standingObjects->getChildren();

	mEnemiesLeft = 0;
	for (const auto& gameObject : gameObjects)
		if (gameObject->getName()==("zombie") != std::string::npos)
			++mEnemiesLeft;
}

void ArcadeManager::updateCounters()
{
	auto* arcadeInterface = mGui.getInterface("arcadeCounters");
	auto* counters = arcadeInterface->getWidget("canvas")->getWidget("counters");

	auto* waveCounter = dynamic_cast<TextWidget*>(counters->getWidget("waveCounter"));
	if (mCurrentWave < 10)
		waveCounter->setString("Wave: 0" + std::to_string(mCurrentWave));
	else 
		waveCounter->setString("Wave: " + std::to_string(mCurrentWave));

	auto* enemiesCounter = dynamic_cast<TextWidget*>(counters->getWidget("enemiesCounter"));
	if(mEnemiesLeft < 10)
		enemiesCounter->setString("Enemies: 0" + std::to_string(mEnemiesLeft));
	else
		enemiesCounter->setString("Enemies: " + std::to_string(mEnemiesLeft));
}

}