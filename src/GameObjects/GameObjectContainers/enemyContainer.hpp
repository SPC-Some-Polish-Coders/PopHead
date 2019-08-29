#pragma once 
 
#include "GameObjects/DrawableGameObjects/Characters/enemy.hpp"
#include <vector>

namespace ph { 

class PhysicsEngine;

class EnemyContainer : public GameObject
{
public:
	EnemyContainer(PhysicsEngine& physicsEngine);

	void addEnemyToDie(Enemy* enemy) { mDyingEnemies.emplace_back(enemy); }
	void updateCurrent(sf::Time delta) override;

private:
	void handleDyingEnemies();

private:
	std::vector<Enemy*> mDyingEnemies;
	PhysicsEngine& mPhysicsEngine;
};

} 
