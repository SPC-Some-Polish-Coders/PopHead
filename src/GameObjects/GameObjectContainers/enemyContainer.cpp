#include "enemyContainer.hpp" 
#include "gameData.hpp"

namespace ph {

EnemyContainer::EnemyContainer(GameData* const gameData)
	:GameObject("enemy_container")
	,mGameData(gameData)
{
}

void EnemyContainer::updateCurrent(sf::Time delta)
{
	handleDyingEnemies();
}

void EnemyContainer::handleDyingEnemies()
{
	for(Enemy* dyingEnemy : mDyingEnemies) {
		mGameData->getRenderer().removeDrawableGameObject(dyingEnemy);
		mGameData->getPhysicsEngine().removeKinematicBody(dyingEnemy->mCollisionBody);
		removeChild(dyingEnemy);
		mDyingEnemies.clear();
	}
}

}
