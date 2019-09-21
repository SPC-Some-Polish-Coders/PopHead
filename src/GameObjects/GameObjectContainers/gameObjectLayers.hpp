#pragma once

#include "GameObjects/gameObject.hpp"

namespace ph {

class PhysicsEngine;
class Character;
class DeadCharacter;

class StandingGameObjectsLayer : public GameObject
{
public:
	StandingGameObjectsLayer(PhysicsEngine& physicsEngine);

	void addCharacterToDie(Character* character);
	void updateCurrent(sf::Time delta) override;

private:
	void handleDyingCharaters();

private:
	std::vector<Character*> mDyingCharacters;
	PhysicsEngine& mPhysicsEngine;
};

class LyingGameObjectsLayer : public GameObject
{
public:
	LyingGameObjectsLayer();
	void removeDeadEnemy(DeadCharacter* enemy);
	void updateCurrent(sf::Time delta) override;

private:
	void removeEnemies();

private:
	std::vector<DeadCharacter*> mEnemiesToRemove;
};

}
