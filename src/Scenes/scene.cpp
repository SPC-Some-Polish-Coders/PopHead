#include "scene.hpp"
#include "gameData.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

Scene::Scene()
	:mRoot(std::make_unique<GameObject>("root"))
	,mHide(false)
	,mPause(false)
{
}

void Scene::input()
{
	mRoot->input();
}

void Scene::update(sf::Time delta)
{
	mRoot->update(delta);
}

}