#include "enemyContainer.hpp" 

namespace ph {

EnemyContainer::EnemyContainer(Renderer* const rendererPointer)
	:Entity("enemy_container")
	,mRednererPointer(rendererPointer)
{
}

void EnemyContainer::update(sf::Time delta)
{
	for(auto it = mChildren.begin(); it != mChildren.end(); ++it)
		(*it)->update(delta);

	for(Enemy* dyingEnemy : mDyingEnemies) {
		mRednererPointer->removeObject(dyingEnemy);
		removeChild(dyingEnemy);
		mDyingEnemies.clear();
	}
}

}
