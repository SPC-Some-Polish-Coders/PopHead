#include "sceneManager.hpp"
#include "sceneParser.hpp"
#include "scene.hpp"
#include "Gui/xmlGuiParser.hpp"
#include "Map/xmlMapParser.hpp"
#include "Resources/xmlResourceParser.hpp"
#include "Audio/xmlAudioParser.hpp"
#include "GameObjects/tiledGameObjectsParser.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"

namespace ph {

SceneManager::SceneManager()
	:mScene(nullptr)
	,mGameData(nullptr)
	,mIsReplacing(false)
	,mIsPopping(false)
	,mHasPlayerPosition(false)
	,mLastPlayerStatus()
{
}

void SceneManager::changingScenesProcess()
{
	if (mIsPopping)
		popAction();

	if (mIsReplacing)
		replaceAction();
}

bool SceneManager::hasPlayerPosition() const
{
	return mHasPlayerPosition;
}

const sf::Vector2f& SceneManager::getPlayerPosition() const
{
	return mPlayerPosition;
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

	if (mCurrentSceneFile == mFileOfSceneToMake)
	{
		mScene.reset(new Scene());
		SceneParser<XmlGuiParser, XmlMapParser, TiledGameObjectsParser, XmlResourceParser, XmlAudioParser>
			sceneParser(mGameData, mScene->getRoot(), mScene->getCutSceneManager(), mFileOfSceneToMake);

		if (mGameData->getAIManager().isPlayerOnScene())
			mScene->setPlayerStatus(mLastPlayerStatus);
	}
	else if (mScene && mGameData->getAIManager().isPlayerOnScene())
	{
		mLastPlayerStatus = mScene->getPlayerStatus();
		mScene.reset(new Scene());
		SceneParser<XmlGuiParser, XmlMapParser, TiledGameObjectsParser, XmlResourceParser, XmlAudioParser>
			sceneParser(mGameData, mScene->getRoot(), mScene->getCutSceneManager(), mFileOfSceneToMake);

		if (mGameData->getAIManager().isPlayerOnScene())
			mScene->setPlayerStatus(mLastPlayerStatus);
	}
	else  // there was not a scene before
	{
		mScene.reset(new Scene());
		SceneParser<XmlGuiParser, XmlMapParser, TiledGameObjectsParser, XmlResourceParser, XmlAudioParser> 
			sceneParser(mGameData, mScene->getRoot(), mScene->getCutSceneManager(), mFileOfSceneToMake);
	}

	PH_LOG_INFO("The scene was replaced by new scene (" + mFileOfSceneToMake + ").");
	mIsReplacing = false;
	mCurrentSceneFile = std::move(mFileOfSceneToMake);
}

void SceneManager::handleEvent(const ph::Event& e)
{
	mScene->handleEvent(e);
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

	if (mCurrentSceneFile != mFileOfSceneToMake)
		mHasPlayerPosition = false;
}

void SceneManager::replaceScene(const std::string& sceneSourceCodeFilePath, const sf::Vector2f& playerPosition)
{
	mFileOfSceneToMake = sceneSourceCodeFilePath;
	mIsReplacing = true;
	mHasPlayerPosition = true;
	mPlayerPosition = playerPosition;
}

void SceneManager::popScene()
{
    mIsPopping = true;
}

}