#include "sceneManager.hpp"
#include "sceneParser.hpp"
#include "scene.hpp"
#include "Gui/xmlGuiParser.hpp"
#include "Map/xmlMapParser.hpp"
#include "Resources/xmlResourceParser.hpp"
#include "Audio/Music/xmlAudioParser.hpp"
#include "GameObjects/tiledGameObjectsParser.hpp"
#include "Logs/logs.hpp"
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
		PH_LOG_WARNING("You are trying to pop scene but there is no scene to pop.");
	else {
		mGameData->getPhysicsEngine().clear();
		mGameData->getGui().clearGUI();
		mScene = nullptr;
		PH_LOG_INFO("The scene was popped.");
	}
	mIsPopping = false;
}

void SceneManager::replaceAction()
{
	mGameData->getPhysicsEngine().clear();
	mGameData->getGui().clearGUI();
	mScene.reset(new Scene());
	SceneParser<XmlGuiParser, XmlMapParser, TiledGameObjectsParser, XmlResourceParser, XmlAudioParser> 
		sceneParser(mGameData, mScene->getRoot(), mScene->getCutSceneManager(), mFileOfSceneToMake);
	PH_LOG_INFO("The scene was replaced by new scene (" + mFileOfSceneToMake + ").");
	mIsReplacing = false;
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