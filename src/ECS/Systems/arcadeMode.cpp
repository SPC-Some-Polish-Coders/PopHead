#include "arcadeMode.hpp"
#include "Audio/Music/musicPlayer.hpp"
#include "GUI/gui.hpp"
#include "AI/aiManager.hpp"
#include "ECS/entitiesTemplateStorage.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/objectsComponents.hpp"
#include "ECS/Components/aiComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"
#include "ECS/Components/animationComponents.hpp"
#include "ECS/Components/graphicsComponents.hpp"
#include "ECS/Components/particleComponents.hpp"
#include "ECS/Components/itemComponents.hpp"
#include "Utilities/random.hpp"

namespace ph::system {

ArcadeMode::ArcadeMode(entt::registry& registry, GUI& gui, AIManager& aiManager, MusicPlayer& musicPlayer, EntitiesTemplateStorage& templateStorage)
	:System(registry)
	,mGui(gui)
	,mAIManager(aiManager)
	,mTemplateStorage(templateStorage)
	,mMusicPlayer(musicPlayer)
	,mNumberOfSpawnersOnTheMap(getNumberOfSpawners())
{
	sIsActive = true;
	mAIManager.setAIMode(AIMode::zombieAlwaysLookForPlayer);
}

ArcadeMode::~ArcadeMode()
{
	sIsActive = false;
	mAIManager.setAIMode(AIMode::normal);
}

void ArcadeMode::update(float dt)
{
	mTimeFromStart += dt;

	updateGuiCounters();

	// TODO: Can't it be in constructor
	// init bullets
	if(!mMadeInit) {
		auto players = mRegistry.view<component::Player, component::Bullets>();
		players.each([](const component::Player, component::Bullets& bullets) {
			bullets.numOfPistolBullets = 350;
			bullets.numOfShotgunBullets = 50;
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
		auto spawners = mRegistry.view<component::ArcadeSpawner, component::BodyRect>();
		spawners.each([dt, this](component::ArcadeSpawner& arcadeModeSpawner, const component::BodyRect& spawnerBody) 
		{
			arcadeModeSpawner.timeFromLastSpawn += dt;
			if(arcadeModeSpawner.timeFromLastSpawn > 0.5f) 
			{
				arcadeModeSpawner.timeFromLastSpawn = 0.f;
				const sf::Vector2f spawnerPos = spawnerBody.rect.getTopLeft();
				auto& wave = arcadeModeSpawner.waves[mCurrentWave - 1];
				if(wave.normalZombiesToSpawn > 0 && wave.slowZombiesToSpawn > 0) {
					int ran = Random::generateNumber(0, 5);
					if(ran == 0) {
						createNormalZombie(spawnerPos);
						--wave.normalZombiesToSpawn;
					}
					else {
						createSlowZombie(spawnerPos);
						--wave.slowZombiesToSpawn;
					}
				}
				else if(wave.normalZombiesToSpawn > 0) {
					createNormalZombie(spawnerPos);
					--wave.normalZombiesToSpawn;
				}
				else if(wave.slowZombiesToSpawn > 0) {
					createSlowZombie(spawnerPos);
					--wave.slowZombiesToSpawn;
				}
				else
					mShouldSpawnEnemies = false;
			}
		});
	}

	// update enemies counter
	auto zombies = mRegistry.view<component::Zombie>();
	mEnemiesCounter = zombies.size();
	
	// break time
	mTimeFromBreakTimeStart += dt;
	if(mTimeFromStart > 15.f && !mIsBreakTime && !mShouldSpawnEnemies && mEnemiesCounter <= 5)
		startBreakTime();
	else if(mIsBreakTime && mTimeFromBreakTimeStart > 20.f){
		endBreakTime();
		createNextWave();
	}
}

void ArcadeMode::createNextWave()
{
	++mCurrentWave;
	mShouldSpawnEnemies = true;
	mIsBreakTime = false;
}

void ArcadeMode::startBreakTime()
{
	auto lootSpawners = mRegistry.view<component::LootSpawner, component::BodyRect>();
	lootSpawners.each([this](const component::LootSpawner lootSpawner, const component::BodyRect& lootSpawnerBody) 
	{
		switch(lootSpawner.type) 
		{
			case component::LootSpawner::Medkit: {
				auto medkitEntity = mTemplateStorage.createCopy("Medkit", mRegistry);
				auto& body = mRegistry.get<component::BodyRect>(medkitEntity);
				body.rect.setPosition(lootSpawnerBody.rect.getTopLeft());
			} break;
			case component::LootSpawner::Bullets: {
				auto bulletBoxEntity = mTemplateStorage.createCopy("BulletBox", mRegistry);
				auto& [bullets, body] = mRegistry.get<component::Bullets, component::BodyRect>(bulletBoxEntity);
				body.rect.setPosition(lootSpawnerBody.rect.getTopLeft());
				bullets.numOfPistolBullets = 10 * Random::generateNumber(2, 10);
				bullets.numOfShotgunBullets = 10 * Random::generateNumber(2, 10);
			} break;
		}
	});

	mIsBreakTime = true;
	mTimeFromBreakTimeStart = 0.f;

	mGui.showInterface("nextWaveInfo");
	mGui.hideInterface("arcadeCounters");

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
		int secondsUntilTheEndOfBreak = static_cast<int>(20.f - mTimeFromBreakTimeStart);
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

void ArcadeMode::createNormalZombie(sf::Vector2f position)
{
	auto zombie = mTemplateStorage.createCopy("Zombie", mRegistry);
	auto& body = mRegistry.get<component::BodyRect>(zombie);
	body.rect.setPosition(position);
}

void ArcadeMode::createSlowZombie(sf::Vector2f position)
{
	auto slowZombie = mTemplateStorage.createCopy("SlowZombie", mRegistry);
	auto& body = mRegistry.get<component::BodyRect>(slowZombie);
	body.rect.setPosition(position);
}

std::string ArcadeMode::addZero(int number)
{
	if(number < 10)
		return "0" + std::to_string(number);
	else
		return std::to_string(number);
}

}

