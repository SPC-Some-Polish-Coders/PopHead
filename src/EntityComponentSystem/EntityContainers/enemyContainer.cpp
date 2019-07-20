#include "enemyContainer.hpp" 
#include "gameData.hpp"

namespace ph {

EnemyContainer::EnemyContainer(GameData* const gameData)
	:Entity("enemy_container")
	,mGameData(gameData)
{
}

void EnemyContainer::update(sf::Time delta)
{
	for(auto it = mChildren.begin(); it != mChildren.end(); ++it)
		(*it)->update(delta);

	for(Enemy* dyingEnemy : mDyingEnemies) {
		mGameData->getRenderer().removeObject(dyingEnemy);
		mGameData->getPhysicsEngine().removeKinematicBody(dyingEnemy->mCollisionBody);
		removeChild(dyingEnemy);
		mDyingEnemies.clear();
	}
}

}
