#include "enemyContainer.hpp" 
#include "Physics/physicsEngine.hpp"

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
		mPhysicsEngine.removeKinematicBody(dyingEnemy->mCollisionBody);
		removeChild(dyingEnemy);
		mDyingEnemies.clear();
	}
}

}
