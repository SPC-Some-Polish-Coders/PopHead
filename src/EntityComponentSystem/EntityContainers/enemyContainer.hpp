#pragma once 
 
#include "EntityComponentSystem/entity.hpp"
#include "EntityComponentSystem/Objects/Characters/enemy.hpp"
#include <vector>

namespace ph { 

class EnemyContainer : public Entity
{
public:
	EnemyContainer(GameData* const);

	void addEnemyToDie(Enemy* enemy) { mDyingEnemies.emplace_back(enemy); }
	void update(sf::Time delta) override;

private:
	void handleDyingEnemies();

private:
	std::vector<Enemy*> mDyingEnemies;
	GameData* const mGameData;
};

} 
