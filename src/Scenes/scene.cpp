#include "scene.hpp"
#include "cutScene.hpp"
#include "gameData.hpp"
#include <SFML/Graphics.hpp>

namespace ph {

Scene::Scene()
	:mCutSceneManager()
	,mRoot(std::make_unique<GameObject>("root"))
	,mPause(false)
{
}

void Scene::input()
{
	if(mCutSceneManager.isCutSceneActive())
		mCutSceneManager.handleCutSceneInput();

	mRoot->input();
}

void Scene::update(sf::Time delta)
{
	if(mCutSceneManager.isCutSceneActive())
		mCutSceneManager.updateCutScene(delta);

	if(!mPause)
		mRoot->update(delta);
}

}