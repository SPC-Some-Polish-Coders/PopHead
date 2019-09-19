#include "scene.hpp"
#include "cutScene.hpp"
#include "gameData.hpp"
#include "GameObjects/DrawableGameObjects/Characters/player.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

Scene::Scene()
	:mCutSceneManager()
	,mRoot(std::make_unique<GameObject>("root"))
	,mPause(false)
{
	GameObject::setRoot(mRoot.get());
}

void Scene::handleEvent(const ph::Event& e)
{
	mRoot->handleEvent(e);
}

void Scene::update(sf::Time delta)
{
 	if(mCutSceneManager.isCutSceneActive())
		mCutSceneManager.updateCutScene(delta);

	if(!mPause)
		mRoot->update(delta);
}

void Scene::setPlayerStatus(const PlayerStatus& status)
{
	auto& player = getPlayer();
	player.setHp(status.mHealthPoints);
	player.setNumOfBullets(status.mNumOfBullets);
}

PlayerStatus Scene::getPlayerStatus() const
{
	auto& player = getPlayer();
	PlayerStatus status;
	status.mHealthPoints = player.getHp();
	status.mNumOfBullets = player.getNumOfBullets();
	return status;
}

Player& Scene::getPlayer() const
{
	auto playerPointer = dynamic_cast<Player*>(mRoot->getChild("LAYER_standingObjects")->getChild("player"));
	return *playerPointer;
}

}