#include "gameObjectLayers.hpp" 
#include "Physics/physicsEngine.hpp"
#include "GameObjects/DrawableGameObjects/deadCharacter.hpp"
#include "GameObjects/DrawableGameObjects/character.hpp"

namespace ph {

StandingGameObjectsLayer::StandingGameObjectsLayer(PhysicsEngine& physicsEngine)
	:GameObject("LAYER_standingObjects")
	,mPhysicsEngine(physicsEngine)
{
}

void StandingGameObjectsLayer::addCharacterToDie(Character* character)
{
	mDyingCharacters.emplace_back(character);
}

void StandingGameObjectsLayer::updateCurrent(sf::Time delta)
{
	handleDyingCharaters();
}

void StandingGameObjectsLayer::handleDyingCharaters()
{
	for(Character* dyingCharacter : mDyingCharacters) {
		dyingCharacter->dropItems();
		auto* lyingObjectsLayer = mRoot->getChild("LAYER_lyingObjects");
		auto deadCharacter = std::make_unique<DeadCharacter>(
			dyingCharacter->getTexture(),
			dyingCharacter->getAnimation().getCurrentTextureRect(),
			"dying" + dyingCharacter->getName()
		);
		deadCharacter->setPosition(dyingCharacter->getPosition());
		lyingObjectsLayer->addChild(std::move(deadCharacter));
		removeChild(dyingCharacter);
	}
	mDyingCharacters.clear();
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
