#pragma once

#include "ECS/system.hpp"

namespace ph {
	class GUI;
	class AIManager;
	class MusicPlayer;
	class EntitiesTemplateStorage;
}

namespace ph::system {

class ArcadeMode : public System
{
public:
	ArcadeMode(entt::registry&, GUI&, AIManager&, MusicPlayer&, EntitiesTemplateStorage&);
	~ArcadeMode();

	void update(float dt) override;

	static bool isActive() { return sIsActive; }

private:
	void updateGuiCounters();
	void createNextWave();
	void startBreakTime();
	void endBreakTime();
	std::string addZero(int number);
	int getNumberOfSpawners();
	void createNormalZombie(sf::Vector2f position);
	void createSlowZombie(sf::Vector2f position);

private:
	GUI& mGui;
	AIManager& mAIManager;
	MusicPlayer& mMusicPlayer;
	EntitiesTemplateStorage& mTemplateStorage;
	sf::Clock mBreakClock;
	float mTimeFromStart = 0.f;
	float mTimeBeforeStartingFirstWave = 10.f;
	int mNumberOfSpawnersOnTheMap;
	int mEnemiesToSpawn = 0;
	int mSlowZombiesToSpawnPerSpawner = 0;
	int mNormalZombiesToSpawnPerSpawner = 0;
	int mEnemiesCounter = 0;
	int mCurrentWave = 0;
	bool mIsBreakTime = false;
	bool mMadeInit = false;
	bool mHasStartedFirstWave = false;
	bool mHasWon = false;
	bool mShouldSpawnEnemies = false;

	inline static bool sIsActive = false;
};

}

