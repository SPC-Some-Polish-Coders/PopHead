#include "entrance.hpp"
#include "Utilities/math.hpp"
#include "Scenes/sceneManager.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include "Logs/logs.hpp"

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

void Entrance::updateCurrent(const sf::Time delta)
{
	auto &root = getParent();
	try {
		auto& player = dynamic_cast<Player&>(root.getChild("player"));
		if (Math::areTheyOverlapping(player.getSprite().getGlobalBounds(), mEntranceArea.getGlobalBounds()))
			mSceneManager.replaceScene(mFilepath);
	}
	catch(const std::exception&) {}
}

}