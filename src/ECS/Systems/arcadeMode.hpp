
// NOTE: 
// For now we don't have arcade mode in the game
// Probably it will stay that way

/*

#pragma once

#include "ECS/system.hpp"

namespace ph {
	class AIManager;
	class EntitiesTemplateStorage;
}

namespace ph::system {

class ArcadeMode : public System
{
public:
	ArcadeMode(entt::registry&, AIManager&, EntitiesTemplateStorage&);

	void update(float dt) override;

	static bool isActive() { return sIsActive; }

private:
	void updateGuiCounters();
	void createNextWave();
	void startBreakTime();
	void endBreakTime();
	std::string addZero(i32 number);
	void createNormalZombie(Vec2 position);
	void createSlowZombie(Vec2 position);

private:
	AIManager& mAIManager;
	EntitiesTemplateStorage& mTemplateStorage;
	float mTimeFromStart = 0.f;
	float mTimeFromBreakTimeStart = 0.f;;
	float mTimeBeforeStartingFirstWave = 5.f;
	i32 mEnemiesToSpawn = 0;
	i32 mSlowZombiesToSpawnPerSpawner = 0;
	i32 mNormalZombiesToSpawnPerSpawner = 0;
	i32 mEnemiesCounter = 0;
	i32 mCurrentWave = 0;
	bool mIsBreakTime = false;
	bool mMadeInit = false;
	bool mHasStartedFirstWave = false;
	bool mHasWon = false;
	bool mShouldSpawnEnemies = false;

	inline static bool sIsActive = false;
};

}

*/
