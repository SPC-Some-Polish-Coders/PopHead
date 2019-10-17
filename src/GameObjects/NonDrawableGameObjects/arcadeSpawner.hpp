#pragma once

#include "GameObjects/gameObject.hpp"
#include "Utilities/spawn.hpp"

namespace ph {

class GameData;

class ArcadeSpawner : public GameObject
{
public:
	ArcadeSpawner(GameData* const, const ObjectType objectType, const sf::Vector2f);
	void invokeSpawner(const int numberOfSlowZombiesToSpawn, const int numberOfNormalZombiesToSpawn);
	void shouldSpawn(bool shouldSpawn);
	void resetSpawner();

private:
	void updateCurrent(const sf::Time delta) override;
	sf::Vector2f getSpawnPosition() const;

private:
	GameData* const mGameData;
	const ObjectType mObjectType;
	const sf::Vector2f mPosition;
	sf::Clock mTimeFromLastSpawn;
	int mNumberOfSlowZombiesToSpawn;
	int mNumberOfNormalZombiesToSpawn;
	bool mShouldSpawn;
};

}