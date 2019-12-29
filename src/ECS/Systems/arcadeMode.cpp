#include "arcadeMode.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "GUI/gui.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/aiComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph::system {

ArcadeMode::ArcadeMode(entt::registry& registry, GUI& gui, MusicPlayer& musicPlayer)
	:System(registry)
	,mGui(gui)
	,mMusicPlayer(musicPlayer)
	,mNumberOfSpawnersOnTheMap(getNumberOfSpawners())
{
}

void ArcadeMode::update(float dt)
{
	mTimeFromStart += dt;

	updateGuiCounters();

	// TODO: Why can't it be in constructor
	// init bullets
	if(!mMadeInit) {
		auto players = mRegistry.view<component::Player, component::GunAttacker>();
		players.each([](const component::Player, component::GunAttacker& gunAttacker) {
			gunAttacker.bullets = 350;
		});
		mMadeInit = true;
	}

	// before first wave
	if(!mHasStartedFirstWave) {
		mTimeBeforeStartingFirstWave -= dt;

		mMusicPlayer.playFromMusicState("break");

		auto* canvas = mGui.getInterface("nextWaveInfo")->getWidget("canvas");
		auto* timeToNextWave = dynamic_cast<TextWidget*>(canvas->getWidget("counters")->getWidget("timeToNextWave"));
		std::string timeLeft = std::to_string(static_cast<int>(mTimeBeforeStartingFirstWave));
		timeToNextWave->setString("Start in " + timeLeft  + " seconds!");

		if(mTimeBeforeStartingFirstWave <= 0.f) {
			mMusicPlayer.playFromMusicState("wave");
			mGui.getInterface("nextWaveInfo")->hide();
			mGui.getInterface("arcadeCounters")->show();
			createNextWave();
			mHasStartedFirstWave = true;
		}
	}

	// spawn enemies after new wave
	if(mShouldSpawnEnemies) {
		
	}

	// update enemies counter
	auto zombies = mRegistry.view<component::Zombie>();
	mEnemiesCounter = zombies.size();
	
	// start or end break time
	if(mTimeFromStart > 10.f && !mIsBreakTime && mEnemiesCounter <= 5)
		startBreakTime();
	else if(mIsBreakTime && mBreakClock.getElapsedTime().asSeconds() > 20){
		endBreakTime();
		createNextWave();
	}
}

void ArcadeMode::createNextWave()
{
	++mCurrentWave;
	setNextWaveNumbers();
	mIsBreakTime = false;
}

void ArcadeMode::setNextWaveNumbers()
{
	switch(mCurrentWave)
	{
		case 1: {
			mSlowZombiesToSpawnPerSpawner = 2;
			mNormalZombiesToSpawnPerSpawner = 0;
		} break;

		case 2: {
			mSlowZombiesToSpawnPerSpawner = 3;
			mNormalZombiesToSpawnPerSpawner = 1;
		} break;

		case 3: {
			mSlowZombiesToSpawnPerSpawner = 5;
			mNormalZombiesToSpawnPerSpawner = 1;
		} break;

		case 4: {
			mSlowZombiesToSpawnPerSpawner = 3;
			mNormalZombiesToSpawnPerSpawner = 3;
		} break;

		case 5: {
			mSlowZombiesToSpawnPerSpawner = 2;
			mNormalZombiesToSpawnPerSpawner = 4;
		} break;

		case 6: {
			mSlowZombiesToSpawnPerSpawner = 4;
			mNormalZombiesToSpawnPerSpawner = 4;
		} break;

		case 7: {
			mSlowZombiesToSpawnPerSpawner = 7;
			mNormalZombiesToSpawnPerSpawner = 3;
		} break;

		case 8: {
			mSlowZombiesToSpawnPerSpawner = 4;
			mNormalZombiesToSpawnPerSpawner = 6;
		} break;

		case 9: {
			mSlowZombiesToSpawnPerSpawner = 8;
			mNormalZombiesToSpawnPerSpawner = 7;
		} break;

		case 10: {
			mSlowZombiesToSpawnPerSpawner = 10;
			mNormalZombiesToSpawnPerSpawner = 10;
		} break;

		default: {
			PH_EXIT_GAME("It's impossible that someone is that good! \nCritical error!");
			break;
		}
	}
}

void ArcadeMode::startBreakTime()
{
	// spawn items
	//auto* invisibleObjects = mRoot->getChild("LAYER_invisibleObjects");
	//auto& gameObjects = invisibleObjects->getChildren();
	//for(const auto& gameObject : gameObjects)
	//	if(gameObject->getName().find("lootSpawner") != std::string::npos)
	//		dynamic_cast<LootSpawner*>(gameObject.get())->spawnLoot();

	mIsBreakTime = true;
	mBreakClock.restart();

	auto* nextWaveInfo = mGui.getInterface("nextWaveInfo");
	nextWaveInfo->show();
	auto* arcadeInterface = mGui.getInterface("arcadeCounters");
	arcadeInterface->hide();

	mMusicPlayer.playFromMusicState("break");
}

void ArcadeMode::endBreakTime()
{
	mIsBreakTime = false;

	auto* nextWaveInfo = mGui.getInterface("nextWaveInfo");
	nextWaveInfo->hide();
	auto* arcadeInterface = mGui.getInterface("arcadeCounters");
	arcadeInterface->show();

	mMusicPlayer.playFromMusicState("wave");
}

void ArcadeMode::updateGuiCounters()
{
	if(mIsBreakTime) {
		auto* arcadeInterface2 = mGui.getInterface("nextWaveInfo");
		auto* counters = arcadeInterface2->getWidget("canvas")->getWidget("counters");
		auto* timeToNextWave = dynamic_cast<TextWidget*>(counters->getWidget("timeToNextWave"));
		int secondsUntilTheEndOfBreak = static_cast<int>(20.f - mBreakClock.getElapsedTime().asSeconds());
		timeToNextWave->setString("Time to next wave: " + addZero(secondsUntilTheEndOfBreak));
	}
	else {
		auto* arcadeInterface = mGui.getInterface("arcadeCounters");
		auto* counters = arcadeInterface->getWidget("canvas")->getWidget("counters");

		auto* waveCounter = dynamic_cast<TextWidget*>(counters->getWidget("waveCounter"));
		waveCounter->setString("Wave: " + addZero(mCurrentWave));
		auto* enemiesCounter = dynamic_cast<TextWidget*>(counters->getWidget("enemiesCounter"));
		enemiesCounter->setString("Enemies: " + addZero(mEnemiesCounter));
	}
}

int ArcadeMode::getNumberOfSpawners()
{
	//auto* invisibleObjects = mRoot->getChild("LAYER_invisibleObjects");
	//auto& gameObjects = invisibleObjects->getChildren();
	int counter = 0;
	//for(const auto& gameObject : gameObjects)
		//if(gameObject->getName().find("arcadeSpawner") != std::string::npos)
			//++counter;
	return counter;
}

std::string ArcadeMode::addZero(int number)
{
	if(number < 10)
		return "0" + std::to_string(number);
	else
		return std::to_string(number);
}

}

