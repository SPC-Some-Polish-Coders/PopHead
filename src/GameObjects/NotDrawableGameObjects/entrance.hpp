#pragma once

#include"GameObjects/gameObject.hpp"
#include"SFML/Graphics.hpp"

namespace ph {

class GameData;

class Entrance : public GameObject
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
