#include "GameObjects/DrawableGameObjects/Characters/Enemies/zombie.hpp"
#include "GameObjects/NonDrawableGameObjects/arcadeSpawner.hpp"
#include "Utilities/spawn.hpp"
#include "Utilities/random.hpp"

namespace ph {

	ArcadeSpawner::ArcadeSpawner(GameData* const gameData, const ObjectType objectType, const sf::Vector2f position)
		:GameObject("arcadeSpawner")
		,mSpawnFrequency(sf::Time::Zero)
		,mPosition(position)
		,mGameData(gameData)
		,mObjectType(objectType)
		,mQuantityOfEnemiesToSpawn(0)
		,mShouldSpawn(false)
	{
	}

	void ArcadeSpawner::updateCurrent(const sf::Time delta)
	{
		if (mShouldSpawn)
		{
			if (mTimeFromLastSpawn.getElapsedTime().asSeconds() > mSpawnFrequency.asSeconds())
			{
				if (mQuantityOfEnemiesToSpawn != 0)
				{
					spawnObject();
					--mQuantityOfEnemiesToSpawn;
				}
				else
					mShouldSpawn = false;
			}
		}
	}

	void ArcadeSpawner::invokeSpawner(const sf::Time timeBetweenSpawns, const int quantityOfEnemiesToSpawn)
	{
		mShouldSpawn = true;
		mSpawnFrequency = sf::seconds(Random::generateNumber(0.f, timeBetweenSpawns.asSeconds()));
		mQuantityOfEnemiesToSpawn = quantityOfEnemiesToSpawn;
	}

	void ArcadeSpawner::shouldSpawn(bool shouldSpawn)
	{
		mShouldSpawn = shouldSpawn;
	}

	void ArcadeSpawner::resetSpawner()
	{
		mQuantityOfEnemiesToSpawn = 0;
		mSpawnFrequency = sf::Time::Zero;
		mShouldSpawn = false;
	}

	void ArcadeSpawner::spawnObject()
	{
		Spawn(mGameData, mObjectType, getSpawnPosition());
		mTimeFromLastSpawn.restart();
	}

	sf::Vector2f ArcadeSpawner::getSpawnPosition() const
	{
		float x = Random::generateNumber(mPosition.x-15, mPosition.x+15);
		float y = Random::generateNumber(mPosition.y-15, mPosition.y+15);
		return sf::Vector2f(x, y);
	}
}