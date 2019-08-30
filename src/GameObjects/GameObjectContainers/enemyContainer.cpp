#include "enemyContainer.hpp" 
#include "Physics/physicsEngine.hpp"
#include "GameObjects/DrawableGameObjects/deadCharacter.hpp"

namespace ph {

EnemyContainer::EnemyContainer(PhysicsEngine& physicsEngine)
	:GameObject("enemy_container")
	,mPhysicsEngine(physicsEngine)
{
}

void EnemyContainer::updateCurrent(sf::Time delta)
{
	handleDyingEnemies();
}

void EnemyContainer::handleDyingEnemies()
{
	for(Enemy* dyingEnemy : mDyingEnemies) {
		auto& deadEnemyContainer = mRoot->getChild("LAYER_lyingOnGroundObjects").getChild("dead_enemy_container");
		auto deadCharacter = std::make_unique<DeadCharacter>(dyingEnemy->getSprite());
		deadCharacter->setPosition(dyingEnemy->getPosition());
		deadEnemyContainer.addChild(std::move(deadCharacter));
		removeChild(dyingEnemy);
	}
	mDyingEnemies.clear();
}

DeadEnemyContainer::DeadEnemyContainer()
	:GameObject("dead_enemy_container")
{
}

void DeadEnemyContainer::removeDeadEnemy(DeadCharacter* enemy)
{
	mEnemiesToRemove.emplace_back(enemy);
}

void DeadEnemyContainer::updateCurrent(sf::Time delta)
{
	removeEnemies();
}

void DeadEnemyContainer::removeEnemies()
{
	for(DeadCharacter* enemy : mEnemiesToRemove)
		removeChild(enemy);

	mEnemiesToRemove.clear();
}

}
