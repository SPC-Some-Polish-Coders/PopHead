#pragma once

#include "ECS/system.hpp"

namespace ph {
	class GUI;
	class AIManager;
	class EntitiesTemplateStorage;
}

namespace ph::system {

class ArcadeMode : public System
{
public:
	ArcadeMode(entt::registry&, GUI&, AIManager&, EntitiesTemplateStorage&);

	void update(float dt) override;

	static bool isActive() { return sIsActive; }

private:
	void updateGuiCounters();
	void createNextWave();
	void startBreakTime();
	void endBreakTime();
	std::string addZero(int number);
	void createNormalZombie(sf::Vector2f position);
	void createSlowZombie(sf::Vector2f position);

private:
	GUI& mGui;
	AIManager& mAIManager;
	EntitiesTemplateStorage& mTemplateStorage;
	float mTimeFromStart = 0.f;
	float mTimeFromBreakTimeStart = 0.f;;
	float mTimeBeforeStartingFirstWave = 5.f;
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

