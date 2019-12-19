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
		//mGameData->getPhysicsEngine().clear();
		mGameData->getGui().clearGUI();
		mScene = nullptr;
		PH_LOG_INFO("The scene was popped.");
	}
	mIsPopping = false;
}

void SceneManager::replaceAction()
{
	//mGameData->getPhysicsEngine().clear();
	mGameData->getGui().clearGUI();

	if (mCurrentSceneFile == mFileOfSceneToMake)
	{
		mScene.reset(new Scene(mGameData->getMusicPlayer(), mGameData->getSoundPlayer(),
			mGameData->getAIManager(), mGameData->getTerminal(), mGameData->getSceneManager(), mGameData->getGui(), *mTilesetTexture));
		SceneParser<XmlGuiParser, XmlMapParser/*, TiledGameObjectsParser*/, XmlAudioParser, EntitiesParser>
			sceneParser(mGameData/*, mScene->getRoot()*/, mScene->getCutSceneManager(), mEntitiesTemplateStorage, mScene->getRegistry(), mFileOfSceneToMake, mGameData->getTextures());

		//if (mGameData->getAIManager().isPlayerOnScene())
			//mScene->setPlayerStatus(mLastPlayerStatus);
	}
	//else if (mScene && mGameData->getAIManager().isPlayerOnScene())
	//{
	//	//mLastPlayerStatus = mScene->getPlayerStatus();
	//	mScene.reset(new Scene(mGameData->getRenderWindow()));
	//	SceneParser<XmlGuiParser, XmlMapParser, TiledGameObjectsParser, XmlResourceParser, XmlAudioParser>
	//		sceneParser(mGameData/*, mScene->getRoot()*/, mScene->getCutSceneManager(), mFileOfSceneToMake);

	//	//if (mGameData->getAIManager().isPlayerOnScene())
	//		//mScene->setPlayerStatus(mLastPlayerStatus);
	//}
	else  // there wasn't a scene before
	{
		mScene.reset(new Scene(mGameData->getMusicPlayer(), mGameData->getSoundPlayer(), mGameData->getAIManager(),
			mGameData->getTerminal(), mGameData->getSceneManager(), mGameData->getGui(), *mTilesetTexture));
		SceneParser<XmlGuiParser, XmlMapParser/*, TiledGameObjectsParser*/, XmlAudioParser, EntitiesParser>
			sceneParser(mGameData/*, mScene->getRoot()*/, mScene->getCutSceneManager(), mEntitiesTemplateStorage, mScene->getRegistry(), mFileOfSceneToMake, mGameData->getTextures());
	}

	PH_LOG_INFO("The scene was replaced by new scene (" + mFileOfSceneToMake + ").");
	mIsReplacing = false;
	mCurrentSceneFile = std::move(mFileOfSceneToMake);
}

void SceneManager::handleEvent(const ph::Event& e)
{
	mScene->handleEvent(e);
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
