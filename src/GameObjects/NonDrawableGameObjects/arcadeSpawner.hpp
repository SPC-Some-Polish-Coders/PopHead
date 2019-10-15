#pragma once

#include "GameObjects/gameObject.hpp"
#include "Utilities/spawn.hpp"

namespace ph {

class GameData;

class ArcadeSpawner : public GameObject
{
public:
	ArcadeSpawner(GameData* const, const ObjectType objectType, const sf::Vector2f);
	void invokeSpawner(const sf::Time timeBetweenSpawns, const int quantityOfEnemiesToSpawn);
	void shouldSpawn(bool shouldSpawn);
	void resetSpawner();

private:
	void updateCurrent(const sf::Time delta) override;
	void spawnObject();
	sf::Vector2f getSpawnPosition() const;

private:
	GameData* const mGameData;
	const ObjectType mObjectType;
	const sf::Vector2f mPosition;
	sf::Clock mTimeFromLastSpawn;
	sf::Time mSpawnFrequency;
	int mQuantityOfEnemiesToSpawn;
	bool mShouldSpawn;
};

}