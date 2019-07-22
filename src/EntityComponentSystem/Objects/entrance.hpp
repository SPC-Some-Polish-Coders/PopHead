#pragma once

#include"EntityComponentSystem/entity.hpp"
#include"SFML/Graphics.hpp"

namespace ph {

class GameData;

class Entrance : public Entity
{
public:
	Entrance(GameData* const, const std::string filepath, const std::string name, 
		const sf::Vector2f area, const sf::Vector2f position);

	void update(const sf::Time delta) override;

private:
	GameData* mGameData;
	sf::RectangleShape mEntranceArea;
	const std::string mFilepath;
};

}
