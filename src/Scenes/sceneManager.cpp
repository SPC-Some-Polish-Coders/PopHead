#include "sceneManager.hpp"
#include "scene.hpp"
#include "Utilities/debug.hpp"
#include "gameData.hpp"

namespace ph {

SceneManager::SceneManager()
	:mScene(nullptr)
	,mGameData(nullptr)
	,mIsReplacing(false)
	,mIsPopping(false)
{
}

void SceneManager::changingScenesProcess()
{
	if (mIsPopping)
		popAction();

	if (mIsReplacing)
		replaceAction();
}

void SceneManager::popAction()
{
	if (mScene == nullptr)
		PH_LOG(LogType::Warning, "You are trying to pop scene but there is no scene to pop.");
	else {
		mGameData->getRenderer().clear();
		mGameData->getPhysicsEngine().clear();
		mScene = nullptr;
		PH_LOG(LogType::Info, "The scene was popped.");
	}
	mIsPopping = false;
}

void SceneManager::replaceAction()
{
	mGameData->getRenderer().clear();
	mGameData->getPhysicsEngine().clear();
	mScene = std::make_unique<Scene>(mGameData, mSceneToMakeSourceCodeFilepath);
	PH_LOG(LogType::Info, "The scene was replaced by new scene (" + mSceneToMakeSourceCodeFilepath + ").");
	mIsReplacing = false;
}

void SceneManager::input()
{
	if(mScene != nullptr)
		mScene->input();
}

void SceneManager::update(sf::Time delta)
{
	if(mScene != nullptr)
		mScene->update(delta);
}

void SceneManager::replaceScene(const std::string& sceneSourceCodeFilePath)
{
	mSceneToMakeSourceCodeFilepath = sceneSourceCodeFilePath;
	mIsReplacing = true;
}

void SceneManager::popScene()
{
    mIsPopping = true;
}

}