#pragma once 

#include <SFML/Graphics.hpp>
#include <vector>

namespace ph { 

class Zombie;

enum class Direction;

class AIManager
{
public:
	AIManager();

	Direction whereShouldIgo(const Zombie*);
	void removeOpponentFromManagedObjects(const Zombie*);

private:
	std::vector<sf::FloatRect> mOpponentsPositions;
	std::vector<std::vector<bool>> mStaticObjects;
};

} 
