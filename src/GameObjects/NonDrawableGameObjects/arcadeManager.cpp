#include "GameObjects/NonDrawableGameObjects/arcadeManager.hpp"
#include "GameObjects/NonDrawableGameObjects/arcadeSpawner.hpp"
#include "GameObjects/NonDrawableGameObjects/lootSpawner.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "Gui/gui.hpp"
#include "Gui/interface.hpp"
#include <math.h>

namespace ph {

ArcadeManager::ArcadeManager(GUI& gui, MusicPlayer& musicPlayer)
	:GameObject("arcadeTimer")
	,mGui(gui)
	,mMusicPlayer(musicPlayer)
	,mTimeFromStart(sf::Time::Zero)
	,mTimeBeforeStart(sf::seconds(10.f))
	,mEnemiesToSpawn(0)
	,mSlowZombiesToSpawnPerSpawner(0)
	,mNormalZombiesToSpawnPerSpawner(0)
	,mCurrentWave(0)
	,mEnemiesCounter(0)
	,mNumberOfSpawnersOnTheMap(getNumberOfSpawners())
	,mIsBreakTime(false)
	,mMadeInit(false)
	,mHasStarted(false)
{
	mIsActive = true;
}

ArcadeManager::~ArcadeManager()
{
	mIsActive = false;
}

void ArcadeManager::updateCurrent(const sf::Time delta)
{
	if (!mHasStarted)
	{
		mTimeBeforeStart -= delta;
		updateStartTimeCounter();
		return;
	}

	if(!mMadeInit)
		init();

	mTimeFromStart += delta;

	updateEnemiesCounter();
	updateWave();
	updateCounters();
}

void ArcadeManager::updateStartTimeCounter()
{
	auto* startTimeInterface = mGui.getInterface("startTime");
	auto* canvas = startTimeInterface->getWidget("canvas");
	auto* startWaveInfo = dynamic_cast<TextWidget*>(canvas->getWidget("startWaveInfo"));
	std::string timeLeft = std::to_string(static_cast<int>(mTimeBeforeStart.asSeconds()));
	startWaveInfo->setString("Start in " + timeLeft  + " seconds!");
	if (mTimeBeforeStart <= sf::Time::Zero)
		startArcadeMode();
}

void ArcadeManager::startArcadeMode()
{
	auto* startTimeInterface = mGui.getInterface("startTime");
	startTimeInterface->hide();
	mHasStarted = true;
	createNextWave();
}

void ArcadeManager::init()
{
	auto* standingObjects = mRoot->getChild("LAYER_standingObjects");
	auto* player = dynamic_cast<Player*>(standingObjects->getChild("player"));
	player->setNumOfBullets(200u);

	mMadeInit = true;
}

void ArcadeManager::updateEnemiesCounter()
{
	auto* standingObjects = mRoot->getChild("LAYER_standingObjects");
	auto& gameObjects = standingObjects->getChildren();

	mEnemiesCounter = 0;
	for (const auto& gameObject : gameObjects)
		if (gameObject->getName().find("ombie") != std::string::npos)
			++mEnemiesCounter;
	
	// NOTE: "ombie" fits to slowZombie and zombie
}

void ArcadeManager::updateWave()
{
	if(mTimeFromStart.asSeconds() > 10 && !mIsBreakTime && mEnemiesCounter <= 5) {
		startBreakTime();
	}
	else if(shouldCreateNewWave()) {
		endBreakTime();
		createNextWave();
	}
}

bool ArcadeManager::shouldCreateNewWave()
{
	return mIsBreakTime && mBreakClock.getElapsedTime().asSeconds() > 20;
}

void ArcadeManager::createNextWave()
{
	++mCurrentWave;
	setNextWaveNumbers();
	invokeSpawners();
	mIsBreakTime = false;
}

void ArcadeManager::setNextWaveNumbers()
{
	switch(mCurrentWave)
	{
		case 1:{
			mSlowZombiesToSpawnPerSpawner = 4;
			mNormalZombiesToSpawnPerSpawner = 0;
		}break;

		case 2: {
			mSlowZombiesToSpawnPerSpawner = 4;
			mNormalZombiesToSpawnPerSpawner = 2;
		}break;
		
		case 3: {
			mSlowZombiesToSpawnPerSpawner = 7;
			mNormalZombiesToSpawnPerSpawner = 1;
		}break;
		
		case 4: {
			mSlowZombiesToSpawnPerSpawner = 4;
			mNormalZombiesToSpawnPerSpawner = 6;
		}break;

		case 5: {
			mSlowZombiesToSpawnPerSpawner = 1;
			mNormalZombiesToSpawnPerSpawner = 14;
		}break;

		default: {
			// TODO_arc: Add more waves
			break;
		}
	}
}

void ArcadeManager::invokeSpawners()
{
	std::vector<ArcadeSpawner*> arcadeSpawners = getSpawners();
	for (auto* spawner : arcadeSpawners)
		spawner->invokeSpawner(mSlowZombiesToSpawnPerSpawner, mNormalZombiesToSpawnPerSpawner);
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

void ArcadeManager::startBreakTime()
{
	// spawn items
	auto* invisibleObjects = mRoot->getChild("LAYER_invisibleObjects");
	auto& gameObjects = invisibleObjects->getChildren();
	for(const auto& gameObject : gameObjects)
		if(gameObject->getName().find("lootSpawner") != std::string::npos)
			dynamic_cast<LootSpawner*>(gameObject.get())->spawnLoot();

	mIsBreakTime = true;
	mBreakClock.restart();
	auto* arcadeInterface = mGui.getInterface("arcadeInformations");
	arcadeInterface->show();
	mMusicPlayer.playFromMusicState("break");
}

void ArcadeManager::endBreakTime()
{
	mIsBreakTime = false;
	auto* arcadeInterface = mGui.getInterface("arcadeInformations");
	arcadeInterface->hide();
	mMusicPlayer.playFromMusicState("wave");
}

void ArcadeManager::updateCounters()
{
	auto* arcadeInterface = mGui.getInterface("arcadeCounters");
	auto* counters = arcadeInterface->getWidget("canvas")->getWidget("counters");

	auto* arcadeClock = dynamic_cast<TextWidget*>(counters->getWidget("arcadeClock"));
	arcadeClock->setString(getArcadeClockValues());
	auto* waveCounter = dynamic_cast<TextWidget*>(counters->getWidget("waveCounter"));
	waveCounter->setString("Wave: " + addZero(mCurrentWave));
	auto * enemiesCounter = dynamic_cast<TextWidget*>(counters->getWidget("enemiesCounter"));
	enemiesCounter->setString("Enemies: " + addZero(mEnemiesCounter));

	if (mIsBreakTime)
	{
		auto* arcadeInterface2 = mGui.getInterface("arcadeInformations");
		auto* information = arcadeInterface2->getWidget("canvas");
		auto* waveClock = dynamic_cast<TextWidget*>(information->getWidget("nextWaveInfo"));
		waveClock->setString(getWaveClockValues());
	}
}

std::string ArcadeManager::getArcadeClockValues()
{
	float elapsedTime = mTimeFromStart.asSeconds();
	int elapsedTimeSeconds = static_cast<int>(elapsedTime);
	int elapsedTimeMinutes = static_cast<int>(elapsedTimeSeconds / 60);
	if (elapsedTimeSeconds >= 60) elapsedTimeSeconds -= elapsedTimeMinutes * 60;
	return std::string("Time: " + addZero(elapsedTimeMinutes) + ":" + addZero(elapsedTimeSeconds));
}

std::string ArcadeManager::getWaveClockValues()
{
	int secondsUntilTheEndOfBreak = static_cast<int>(20.f - mBreakClock.getElapsedTime().asSeconds());
	return std::string("Next wave in " + addZero(secondsUntilTheEndOfBreak) + " seconds!");
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

std::string ArcadeManager::addZero(int number)
{
	if (number < 10)
		return "0" + std::to_string(number);
	else
		return std::to_string(number);
}

}
