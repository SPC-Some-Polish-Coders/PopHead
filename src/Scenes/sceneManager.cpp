#include "sceneManager.hpp"
#include "scene.hpp"
#include "sceneParser.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

#include "Gui/xmlGuiParser.hpp"
#include "Map/xmlMapParser.hpp"
#include "GameObjects/xmlGameObjectsParser.hpp"
#include "Resources/xmlResourceParser.hpp"

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
		PH_LOG_WARNING("You are trying to pop scene but there is no scene to pop.");
	else {
		mGameData->getRenderer().clear();
		mGameData->getPhysicsEngine().clear();
		mScene = nullptr;
		PH_LOG_INFO("The scene was popped.");
	}
	mIsPopping = false;
}

void SceneManager::replaceAction()
{
	mGameData->getRenderer().clear();
	mGameData->getPhysicsEngine().clear();
	mScene.reset(new Scene());
	SceneParser<XmlGuiParser, XmlMapParser, XmlGameObjectsParser, XmlResourceParser> 
		sceneParser(mGameData, mScene->getRoot(), mFileOfSceneToMake);
	PH_LOG_INFO("The scene was replaced by new scene (" + mFileOfSceneToMake + ").");
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
	mFileOfSceneToMake = sceneSourceCodeFilePath;
	mIsReplacing = true;
}

void SceneManager::popScene()
{
    mIsPopping = true;
}

}