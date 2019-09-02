#pragma once

#include "GameObjects/gameObject.hpp"
#include "Utilities/spawn.hpp"

namespace ph {

class GameData;

class Spawner : public GameObject
{
public:
	Spawner(GameData* const, const std::string&, const ObjectType objectType, const sf::Time, const sf::Vector2f);
	
private:
	void updateCurrent(const sf::Time delta) override;
	void spawnObject();

private:
	sf::Clock mTimeFromLastSpawn;
	const sf::Time mSpawnFrequency;
	const sf::Vector2f mPosition;
	GameData* const mGameData;
	const ObjectType mObjectType;
};

}