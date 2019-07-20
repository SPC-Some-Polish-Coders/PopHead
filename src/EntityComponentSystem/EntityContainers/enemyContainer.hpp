#pragma once 
 
#include "EntityComponentSystem/entity.hpp"
#include "EntityComponentSystem/Objects/Characters/enemy.hpp"
#include <vector>

namespace ph { 

class EnemyContainer : public Entity
{
public:
	EnemyContainer(GameData* const);

	void update(sf::Time delta) override;
	void addEnemyToDie(Enemy* enemy) { mDyingEnemies.emplace_back(enemy); }

private:
	std::vector<Enemy*> mDyingEnemies;
	GameData* const mGameData;
};

} 
