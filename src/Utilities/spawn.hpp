#pragma once

#include "SFML/Graphics.hpp"
#include <string>

namespace ph {

class GameData;
class GameObject;

enum class ObjectType
{
	Zombie, Npc
};

class Spawn
{
public:
	Spawn(GameData* const, const ObjectType, const sf::Vector2f);

private:
	void spawnObject();

	template<typename T>
	void spawn();
	template<typename T>
	void spawnEnemy();
	
	auto getRoot() -> GameObject &;

private:
	GameData* const mGameData;
	const sf::Vector2f mSpawnPosition;
	const ObjectType mObjectType;

};

}
#include "spawn.inl"
