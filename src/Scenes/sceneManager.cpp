#include "sceneManager.hpp"
#include "sceneParser.hpp"
#include "scene.hpp"
#include "GUI/xmlGuiParser.hpp"
#include "ECS/xmlMapParser.hpp"
#include "Audio/xmlAudioParser.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"
#include "ECS/entitiesParser.hpp"
#include "ECS/tiledParser.hpp"

namespace ph {

SceneManager::SceneManager()
	:mScene(nullptr)
	,mThreadPool(2)
	,mGameData(nullptr)
	,mIsReplacing(false)
	,mIsPopping(false)
	,mHasPlayerPositionForNextScene(false)
	,mLastPlayerStatus()
	,mTilesetTexture(nullptr)
{
}

void SceneManager::changingScenesProcess()
{
	if (mIsPopping)
		popAction();

	if (mIsReplacing)
		replaceAction();
}

bool SceneManager::hasPlayerPositionForNextScene() const
{
	return mHasPlayerPositionForNextScene;
}

const sf::Vector2f& SceneManager::getPlayerPositionForNextScene() const
{
	return mPlayerPositionForNextScene;
}

void SceneManager::popAction()
{
	if (mScene == nullptr)
		PH_LOG_WARNING("You are trying to pop scene but there is no scene to pop.");
	else {
		mGameData->getGui().clearGUI();
		mScene = nullptr;
		PH_LOG_INFO("The scene was popped.");
	}
	mIsPopping = false;
}

void SceneManager::replaceAction()
{
	mGameData->getGui().clearGUI();

	if(mCurrentSceneFile == mFileOfSceneToMake && mHasPlayerPositionForNextScene)
		mScene->setPlayerPosition(mPlayerPositionForNextScene);
	else {
		bool thereIsPlayerStatus = mScene && mGameData->getAIManager().isPlayerOnScene();
		if (thereIsPlayerStatus)
			mLastPlayerStatus = mScene->getPlayerStatus();
		
		mScene.reset(new Scene(mGameData->getMusicPlayer(), mGameData->getSoundPlayer(), mGameData->getAIManager(),
			                   mGameData->getTerminal(), *this, mGameData->getGui(), *mTilesetTexture));

		parseScene(mGameData, mScene->getCutSceneManager(), mEntitiesTemplateStorage, mScene->getRegistry(), mFileOfSceneToMake,
		           mGameData->getTextures(), mScene->getSystemsQueue(), mGameData->getGui(), mGameData->getMusicPlayer(), mGameData->getAIManager());

		if(mGameData->getAIManager().isPlayerOnScene()) {
			mScene->setPlayerStatus(mLastPlayerStatus);
			if(mHasPlayerPositionForNextScene)
				mScene->setPlayerPosition(mPlayerPositionForNextScene);
		}
	}

	PH_LOG_INFO("The scene was replaced by new scene (" + mFileOfSceneToMake + ").");
	mIsReplacing = false;
	mCurrentSceneFile = std::move(mFileOfSceneToMake);
}

void SceneManager::handleEvent(const Event& e)
{
	if (auto* event = std::get_if<ActionEvent>(&e))
		mScene->handleEvent(*event);
}

void SceneManager::update(sf::Time dt)
{
	PH_ASSERT_UNEXPECTED_SITUATION(mScene != nullptr, "There is no active scene");
	mScene->update(dt);
}

void SceneManager::setGameData(GameData* const gameData)
{
	mGameData = gameData;
	gameData->getTextures().load("textures/map/extrudedTileset.png");
	mTilesetTexture = &gameData->getTextures().get("textures/map/extrudedTileset.png");
}

void SceneManager::replaceScene(const std::string& sceneSourceCodeFilePath)
{
	mFileOfSceneToMake = sceneSourceCodeFilePath;
	mIsReplacing = true;
	mHasPlayerPositionForNextScene = false;
}

void SceneManager::replaceScene(const std::string& sceneSourceCodeFilePath, const sf::Vector2f& playerPosition)
{
	mFileOfSceneToMake = sceneSourceCodeFilePath;
	mIsReplacing = true;
	mHasPlayerPositionForNextScene = true;
	mPlayerPositionForNextScene = playerPosition;
}

void SceneManager::popScene()
{
    mIsPopping = true;
}

}
