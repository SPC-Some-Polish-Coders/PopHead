#include "sceneManager.hpp"
#include "sceneParser.hpp"
#include "scene.hpp"
#include "GUI/xmlGuiParser.hpp"
#include "Map/xmlMapParser.hpp"
#include "Audio/xmlAudioParser.hpp"
#include "Logs/logs.hpp"
#include "gameData.hpp"
#include "ECS/entitiesParser.hpp"

namespace ph {

SceneManager::SceneManager()
	:mScene(nullptr)
	,mGameData(nullptr)
	,mIsReplacing(false)
	,mIsPopping(false)
	,mHasPlayerPosition(false)
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
		mGameData->getGui().clearGUI();
		mScene = nullptr;
		PH_LOG_INFO("The scene was popped.");
	}
	mIsPopping = false;
}

void SceneManager::replaceAction()
{
	mGameData->getGui().clearGUI();

	if (mCurrentSceneFile == mFileOfSceneToMake && mHasPlayerPosition)
		mScene->setPlayerPosition(mPlayerPosition);
	else
	{
		bool thereIsPlayerStatus = mScene && mGameData->getAIManager().isPlayerOnScene();
		if (thereIsPlayerStatus)
			mLastPlayerStatus = mScene->getPlayerStatus();
		
		mScene.reset(new Scene(mGameData->getMusicPlayer(), mGameData->getSoundPlayer(),
			mGameData->getAIManager(), mGameData->getTerminal(), mGameData->getSceneManager(), mGameData->getGui(), *mTilesetTexture));
		SceneParser<XmlGuiParser, XmlMapParser, XmlAudioParser, EntitiesParser>
			sceneParser(mGameData, mScene->getCutSceneManager(), mEntitiesTemplateStorage, mScene->getRegistry(), mFileOfSceneToMake, mGameData->getTextures());

		if (mGameData->getAIManager().isPlayerOnScene())
		{
			mScene->setPlayerStatus(mLastPlayerStatus);

			if (mHasPlayerPosition)
				mScene->setPlayerPosition(mPlayerPosition);
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
	if(mScene != nullptr)
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
