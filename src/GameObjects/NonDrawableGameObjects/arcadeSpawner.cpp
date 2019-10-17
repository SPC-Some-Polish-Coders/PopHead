#include "GameObjects/DrawableGameObjects/Characters/Enemies/zombie.hpp"
#include "GameObjects/NonDrawableGameObjects/arcadeSpawner.hpp"
#include "Utilities/spawn.hpp"
#include "Utilities/random.hpp"

namespace ph {

	ArcadeSpawner::ArcadeSpawner(GameData* const gameData, const ObjectType objectType, const sf::Vector2f position)
		:GameObject("arcadeSpawner")
		,mPosition(position)
		,mGameData(gameData)
		,mObjectType(objectType)
		,mNumberOfSlowZombiesToSpawn(0)
		,mNumberOfNormalZombiesToSpawn(0)
		,mShouldSpawn(false)
	{
	}

	void ArcadeSpawner::updateCurrent(const sf::Time delta)
	{
		if(mShouldSpawn)
		{
			// TODO_arc: Change sf::Clock to sf::Time in mTimeFromLastSpawn
			// TODO_arc: Devide it to smaller functions

			if(mTimeFromLastSpawn.getElapsedTime().asSeconds() > 0.3)
			{
				if (mNumberOfNormalZombiesToSpawn > 0 && mNumberOfSlowZombiesToSpawn > 0)
				{
					int ran = Random::generateNumber(0, 5);
					if(ran == 0)
					{
						// Spawn normal zombie
						Spawn(mGameData, ObjectType::Zombie, getSpawnPosition());
						mTimeFromLastSpawn.restart();
						--mNumberOfNormalZombiesToSpawn;
					}
					else {
						// Spawn slow zombie
						Spawn(mGameData, ObjectType::SlowZombie, getSpawnPosition());
						mTimeFromLastSpawn.restart();
						--mNumberOfSlowZombiesToSpawn;
					}
				}
				else if(mNumberOfSlowZombiesToSpawn > 0)
				{
					// Spawn slow zombie
					Spawn(mGameData, ObjectType::SlowZombie, getSpawnPosition());
					mTimeFromLastSpawn.restart();
					--mNumberOfSlowZombiesToSpawn;
				}
				else if(mNumberOfNormalZombiesToSpawn > 0) {
					// Spawn normal zombie
					Spawn(mGameData, ObjectType::Zombie, getSpawnPosition());
					mTimeFromLastSpawn.restart();
					--mNumberOfNormalZombiesToSpawn;
				}
				else
					mShouldSpawn = false;
			}
		}
	}

	void ArcadeSpawner::invokeSpawner(const int numberOfSlowZombiesToSpawn, const int numberOfNormalZombiesToSpawn)
	{
		mShouldSpawn = true;
		mNumberOfSlowZombiesToSpawn = numberOfSlowZombiesToSpawn;
		mNumberOfNormalZombiesToSpawn = numberOfNormalZombiesToSpawn;
	}

	void ArcadeSpawner::shouldSpawn(bool shouldSpawn)
	{
		mShouldSpawn = shouldSpawn;
	}

	void ArcadeSpawner::resetSpawner()
	{
		mNumberOfSlowZombiesToSpawn = 0;
		mNumberOfNormalZombiesToSpawn = 0;
		mShouldSpawn = false;
	}

	sf::Vector2f ArcadeSpawner::getSpawnPosition() const
	{
		float x = Random::generateNumber(mPosition.x-15, mPosition.x+15);
		float y = Random::generateNumber(mPosition.y-15, mPosition.y+15);
		return sf::Vector2f(x, y);
	}
}
