#include "entrance.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "Utilities/math.hpp"
#include "gameData.hpp"

namespace ph{

Entrance::Entrance(GameData* const gameData, const std::string filepath, const std::string name,
	const sf::Vector2f size, const sf::Vector2f position)
	:GameObject(name)
	,mGameData(gameData)
	,mEntranceArea(size)
	,mFilepath(filepath)
{
	mEntranceArea.setPosition(position);
}

void Entrance::update(const sf::Time delta)
{
	auto &root = getParent();
	auto& player = dynamic_cast<Player&>(root.getChild("player"));
	if (Math::areTheyOverlapping(player.getSprite().getGlobalBounds(), mEntranceArea.getGlobalBounds()))
		mGameData->getSceneMachine().replaceScene(mFilepath);
}

}