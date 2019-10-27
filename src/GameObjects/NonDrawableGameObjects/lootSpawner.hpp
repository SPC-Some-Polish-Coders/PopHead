#pragma once

#include "GameObjects/gameObject.hpp"

namespace ph {

enum class LootType{ Medkit, Bullets };

class GameObject;
class GameData;

class LootSpawner : public GameObject
{
public:
	LootSpawner(LootType type, GameObject* itemsContainer, GameData* gameData);

	void spawnLoot();

private:
	GameData* mGameData;
	GameObject* mItemsContainer;
	LootType mType;
};

}
