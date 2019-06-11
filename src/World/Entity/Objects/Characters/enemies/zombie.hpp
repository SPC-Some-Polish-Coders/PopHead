#ifndef POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTERS_ENEMIES_ZOMBIE_H_
#define POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTERS_ENEMIES_ZOMBIE_H_

#include "World/Entity/Objects/Characters/enemy.hpp"

namespace ph {

class Zombie : public Enemy
{
public:
	Zombie(GameData*);

	void update(sf::Time delta) override;

private:
	sf::Clock timeFromLastGrowl;
};

}

#endif // !POPHEAD_WORLD_ENTITY_OBJECTS_CHARACTERS_ENEMIES_ZOMBIE_H_

