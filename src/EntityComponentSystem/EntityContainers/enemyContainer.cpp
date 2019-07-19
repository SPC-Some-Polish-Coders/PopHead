#include "enemyContainer.hpp" 

namespace ph {

EnemyContainer::EnemyContainer()
	:Entity("enemy_container")
{
}

void EnemyContainer::update(sf::Time delta)
{
	for(auto it = mChildren.begin(); it != mChildren.end(); ++it)
		(*it)->update(delta);

	for(Enemy* dyingEnemy : mDyingEnemies) {
		removeChild(dyingEnemy);
		mDyingEnemies.clear();
	}
}

}
