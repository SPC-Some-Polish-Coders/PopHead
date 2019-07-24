#include "GameObjects/NotDrawableGameObjects/spawner.hpp"
#include "GameObjects/DrawableGameObjects/Characters/Enemies/zombie.hpp"
#include "Utilities/spawn.hpp"
#include <iostream>

namespace ph {

Spawner::Spawner(GameData* const gameData, const std::string spawnerName,  const ObjectType objectType,
		const sf::Time spawnFrequency, const sf::Vector2f position)
	:GameObject(spawnerName)
	,mGameData(gameData)
	,mObjectType(objectType)
	,mSpawnFrequency(spawnFrequency)
	,mPosition(position)
{
}

void Spawner::spawnObject()
{
	Spawn(mGameData, mObjectType, mPosition);
	mTimeFromLastSpawn.restart();
}

void Spawner::update(const sf::Time delta)
{
	if (mTimeFromLastSpawn.getElapsedTime().asSeconds() > mSpawnFrequency.asSeconds())
		spawnObject();
}

}