#pragma once

#include "GameObjects/gameObject.hpp"

namespace ph {

class PhysicsEngine;
class Enemy;
class DeadCharacter;

class StandingGameObjectsLayer : public GameObject
{
public:
	StandingGameObjectsLayer(PhysicsEngine& physicsEngine);

	void addEnemyToDie(Enemy* enemy) { mDyingEnemies.emplace_back(enemy); }
	void updateCurrent(sf::Time delta) override;

private:
	void handleDyingEnemies();

private:
	std::vector<Enemy*> mDyingEnemies;
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