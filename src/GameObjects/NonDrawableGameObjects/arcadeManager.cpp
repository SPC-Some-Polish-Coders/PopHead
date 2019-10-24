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
	:GameObject("arcadeManager")
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
	,mHasWon(false)
{
	mIsActive = true;
}

ArcadeManager::~ArcadeManager()
{
	mIsActive = false;
}

void ArcadeManager::updateCurrent(const sf::Time delta)
{
	if (mHasWon && mBreakClock.getElapsedTime().asSeconds() > 10.f)
		endWinInscript();

	if (!mHasStarted)
	{
		mTimeBeforeStart -= delta;
		mMusicPlayer.playFromMusicState("break");
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
	auto* canvas = mGui.getInterface("nextWaveInfo")->getWidget("canvas");
	auto* timeToNextWave = dynamic_cast<TextWidget*>(canvas->getWidget("counters")->getWidget("timeToNextWave"));
	std::string timeLeft = std::to_string(static_cast<int>(mTimeBeforeStart.asSeconds()));
	timeToNextWave->setString("Start in " + timeLeft  + " seconds!");
	if (mTimeBeforeStart <= sf::Time::Zero)
		startArcadeMode();
}

void ArcadeManager::startArcadeMode()
{
	mMusicPlayer.playFromMusicState("wave");
	mGui.getInterface("nextWaveInfo")->hide();
	mGui.getInterface("arcadeCounters")->show();
	mHasStarted = true;
	createNextWave();
}

void ArcadeManager::init()
{
	auto* standingObjects = mRoot->getChild("LAYER_standingObjects");
	auto* player = dynamic_cast<Player*>(standingObjects->getChild("player"));
	player->setNumOfBullets(350u);

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
	if (mTimeFromStart.asSeconds() > 10 && !mIsBreakTime && mEnemiesCounter <= 5) {
		if (mPGAMode && mCurrentWave == 5)
			handleWin();
		else
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

void ArcadeManager::handleWin()
{
	mHasWon = true;
	auto* winScreen = mGui.getInterface("winScreen");
	auto* totalTime = dynamic_cast<TextWidget*>(winScreen->getWidget("canvas")->getWidget("totalTimeText"));
	int minutes = mTimeFromStart.asSeconds() /60;
	int seconds = mTimeFromStart.asSeconds() - minutes*60;
	totalTime->setString("Total time: " + addZero(minutes) + ":" + addZero(seconds));
	winScreen->show();
	startBreakTime();
}

void ArcadeManager::endWinInscript()
{
	auto* winScreen = mGui.getInterface("winScreen");
	winScreen->hide();
}

void ArcadeManager::setNextWaveNumbers()
{
	switch(mCurrentWave)
	{
		case 1:{
			mSlowZombiesToSpawnPerSpawner = 2;
			mNormalZombiesToSpawnPerSpawner = 0;
		}break;

		case 2: {
			mSlowZombiesToSpawnPerSpawner = 3;
			mNormalZombiesToSpawnPerSpawner = 1;
		}break;
		
		case 3: {
			mSlowZombiesToSpawnPerSpawner = 5;
			mNormalZombiesToSpawnPerSpawner = 1;
		}break;
		
		case 4: {
			mSlowZombiesToSpawnPerSpawner = 3;
			mNormalZombiesToSpawnPerSpawner = 3;
		}break;

		case 5: {
			mSlowZombiesToSpawnPerSpawner = 2;
			mNormalZombiesToSpawnPerSpawner = 4;
		}break;

		case 6: {
			mSlowZombiesToSpawnPerSpawner = 4;
			mNormalZombiesToSpawnPerSpawner = 4;
		}break;

		case 7: {
			mSlowZombiesToSpawnPerSpawner = 7;
			mNormalZombiesToSpawnPerSpawner = 3;
		}break;

		case 8: {
			mSlowZombiesToSpawnPerSpawner = 4;
			mNormalZombiesToSpawnPerSpawner = 6;
		}break;

		case 9: {
			mSlowZombiesToSpawnPerSpawner = 8;
			mNormalZombiesToSpawnPerSpawner = 7;
		}break;
		
		case 10: {
			mSlowZombiesToSpawnPerSpawner = 10;
			mNormalZombiesToSpawnPerSpawner = 10;
		}break;

		default: {
			PH_EXIT_GAME("It's impossible that someone is that good! \nCritical error!");
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

	auto* nextWaveInfo = mGui.getInterface("nextWaveInfo");
	nextWaveInfo->show();
	auto* arcadeInterface = mGui.getInterface("arcadeCounters");
	arcadeInterface->hide();

	mMusicPlayer.playFromMusicState("break");
}

void ArcadeManager::endBreakTime()
{
	mIsBreakTime = false;

	auto* nextWaveInfo = mGui.getInterface("nextWaveInfo");
	nextWaveInfo->hide();
	auto* arcadeInterface = mGui.getInterface("arcadeCounters");
	arcadeInterface->show();

	mMusicPlayer.playFromMusicState("wave");
}

void ArcadeManager::updateCounters()
{
	if (mIsBreakTime) {
		auto* arcadeInterface2 = mGui.getInterface("nextWaveInfo");
		auto* counters = arcadeInterface2->getWidget("canvas")->getWidget("counters");
		auto* timeToNextWave = dynamic_cast<TextWidget*>(counters->getWidget("timeToNextWave"));
		timeToNextWave->setString(getTimeToNextWaveInfo());
	}
	else {
		auto* arcadeInterface = mGui.getInterface("arcadeCounters");
		auto* counters = arcadeInterface->getWidget("canvas")->getWidget("counters");

		auto* waveCounter = dynamic_cast<TextWidget*>(counters->getWidget("waveCounter"));
		waveCounter->setString("Wave: " + addZero(mCurrentWave));
		auto * enemiesCounter = dynamic_cast<TextWidget*>(counters->getWidget("enemiesCounter"));
		enemiesCounter->setString("Enemies: " + addZero(mEnemiesCounter));
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

std::string ArcadeManager::getTimeToNextWaveInfo()
{
	int secondsUntilTheEndOfBreak = static_cast<int>(20.f - mBreakClock.getElapsedTime().asSeconds());
	return std::string("Time to next wave: " + addZero(secondsUntilTheEndOfBreak));
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
