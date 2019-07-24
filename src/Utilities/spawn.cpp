#include "gameData.hpp"
#include "Utilities/spawn.hpp"
#include "GameObjects/DrawableGameObjects/Characters/Enemies/zombie.hpp"
#include "GameObjects/DrawableGameObjects/Characters/npc.hpp"

namespace ph {

Spawn::Spawn(GameData* const gameData, const ObjectType objectType, const sf::Vector2f spawnPosition)
	:mGameData(gameData)
	,mObjectType(objectType)
	,mSpawnPosition(spawnPosition)
{
	spawnObject();
}

void Spawn::spawnObject()
{
	switch (mObjectType)
	{
	case ObjectType::Zombie:
		spawnZombie();
		return;
	case ObjectType::Npc:
		spawnNpc();
		return;
	}
}

void Spawn::spawnZombie()
{
	auto& zombie = std::make_unique<Zombie>(mGameData);
	auto& root = getRoot();
	zombie->setPosition(mSpawnPosition);
	root.addChild(std::move(zombie));
}

void Spawn::spawnNpc()
{
	auto& npc = std::make_unique<Npc>(mGameData);
	auto& root = getRoot();
	npc->setPosition(mSpawnPosition);
	root.addChild(std::move(npc));
}

auto Spawn::getRoot() -> GameObject &
{
	auto& scene = mGameData->getSceneMachine().getScene();
	return scene.getRoot();
}

}
