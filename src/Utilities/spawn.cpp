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
		spawnEnemy<Zombie>();
		return;
	case ObjectType::Npc:
		spawn<Npc>();
		return;
	}
}

auto Spawn::getRoot() -> GameObject&
{
	auto& scene = mGameData->getSceneManager().getScene();
	return scene.getRoot();
}

}
