#include "scene.hpp"
#include "gameData.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

Scene::Scene()
	:mRoot(std::make_unique<GameObject>("root"))
	,mPause(false)
{
}

void Scene::input()
{
	mRoot->input();
}

void Scene::update(sf::Time delta)
{
	if(!mPause)
		mRoot->update(delta);
}

}