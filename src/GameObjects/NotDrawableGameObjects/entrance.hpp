#pragma once

#include"GameObjects/gameObject.hpp"
#include"SFML/Graphics.hpp"

namespace ph {

class SceneManager;

class Entrance : public GameObject
{
public:
	Entrance(SceneManager&, const std::string filepath, const std::string name,
		const sf::Vector2f area, const sf::Vector2f position);

	void updateCurrent(const sf::Time delta) override;

private:
	SceneManager& mSceneManager;
	sf::RectangleShape mEntranceArea;
	const std::string mFilepath;
};

}
