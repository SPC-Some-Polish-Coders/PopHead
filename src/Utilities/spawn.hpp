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
	auto getRoot() -> GameObject &;

	void spawnZombie();
	void spawnNpc();

private:
	GameData* const mGameData;
	const sf::Vector2f mSpawnPosition;
	const ObjectType mObjectType;

};
}
