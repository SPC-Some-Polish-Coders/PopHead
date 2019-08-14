#pragma once 
 
#include "GameObjects/DrawableGameObjects/Characters/enemy.hpp"
#include <vector>

namespace ph { 

class EnemyContainer : public GameObject
{
public:
	EnemyContainer(GameData* const);

	void addEnemyToDie(Enemy* enemy) { mDyingEnemies.emplace_back(enemy); }
	void updateCurrent(sf::Time delta) override;

private:
	void handleDyingEnemies();

private:
	std::vector<Enemy*> mDyingEnemies;
	GameData* const mGameData;
};

} 
