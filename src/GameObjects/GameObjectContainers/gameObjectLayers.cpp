#include "gameObjectLayers.hpp" 
#include "Physics/physicsEngine.hpp"
#include "GameObjects/DrawableGameObjects/deadCharacter.hpp"
#include "GameObjects/DrawableGameObjects/Characters/enemy.hpp"

namespace ph {

StandingGameObjectsLayer::StandingGameObjectsLayer(PhysicsEngine& physicsEngine)
	:GameObject("LAYER_standingObjects")
	,mPhysicsEngine(physicsEngine)
{
}

void StandingGameObjectsLayer::updateCurrent(sf::Time delta)
{
	handleDyingEnemies();
}

void StandingGameObjectsLayer::handleDyingEnemies()
{
	for(Enemy* dyingEnemy : mDyingEnemies) {
		auto& lyingObjectsLayer = mRoot->getChild("LAYER_lyingObjects");
		auto deadCharacter = std::make_unique<DeadCharacter>(dyingEnemy->getSprite());
		deadCharacter->setPosition(dyingEnemy->getPosition());
		lyingObjectsLayer.addChild(std::move(deadCharacter));
		removeChild(dyingEnemy);
	}
	mDyingEnemies.clear();
}

LyingGameObjectsLayer::LyingGameObjectsLayer()
	:GameObject("LAYER_lyingObjects")
{
}

void LyingGameObjectsLayer::removeDeadEnemy(DeadCharacter* enemy)
{
	mEnemiesToRemove.emplace_back(enemy);
}

void LyingGameObjectsLayer::updateCurrent(sf::Time delta)
{
	removeEnemies();
}

void LyingGameObjectsLayer::removeEnemies()
{
	for(DeadCharacter* enemy : mEnemiesToRemove)
		removeChild(enemy);

	mEnemiesToRemove.clear();
}

}
