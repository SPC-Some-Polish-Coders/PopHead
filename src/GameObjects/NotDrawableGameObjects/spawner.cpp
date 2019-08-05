#include "GameObjects/DrawableGameObjects/Characters/Enemies/zombie.hpp"
#include "GameObjects/NotDrawableGameObjects/spawner.hpp"
#include "Utilities/spawn.hpp"

namespace ph {

Spawner::Spawner(GameData* const gameData, const std::string& spawnerName, const ObjectType objectType,
		const sf::Time spawnFrequency, const sf::Vector2f position)
	:GameObject(spawnerName)
	,mSpawnFrequency(spawnFrequency)
	,mPosition(position)
	,mGameData(gameData)
	,mObjectType(objectType)
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