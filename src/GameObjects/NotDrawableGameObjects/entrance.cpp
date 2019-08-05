#include "entrance.hpp"
#include "Utilities/math.hpp"
#include "Scenes/sceneManager.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"

namespace ph{

Entrance::Entrance(SceneManager& sceneManager, const std::string filepath, const std::string name,
	const sf::Vector2f size, const sf::Vector2f position)
	:GameObject(name)
	,mSceneManager(sceneManager)
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
		mSceneManager.replaceScene(mFilepath);
}

}