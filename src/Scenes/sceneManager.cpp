#include "sceneManager.hpp"
#include "sceneParser.hpp"
#include "scene.hpp"
#include "GUI/xmlGuiParser.hpp"
#include "ECS/xmlMapParser.hpp"
#include "Audio/xmlAudioParser.hpp"
#include "Logs/logs.hpp"
#include "ECS/entitiesParser.hpp"
#include "ECS/tiledParser.hpp"

namespace ph {

SceneManager::SceneManager()
	:mScene(nullptr)
	,mThreadPool(0)
	,mAIManager(nullptr)
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
		GUI::clear();
		mScene = nullptr;
		PH_LOG_INFO("The scene was popped, scene file: " + mCurrentSceneFilePath);
	}
	mIsPopping = false;
}

void SceneManager::replaceAction()
{
	GUI::clear();

	if (mCurrentSceneFilePath == mFilePathOfSceneToMake && mHasPlayerPositionForNextScene)
	{
		mScene->setPlayerPosition(mPlayerPositionForNextScene);
		PH_LOG_INFO("Player position changed, new position: " + std::to_string(mPlayerPositionForNextScene.x) + ',' + std::to_string(mPlayerPositionForNextScene.y));
	}
	else {
		bool thereIsPlayerStatus = mScene && mAIManager->isPlayerOnScene();
		if (thereIsPlayerStatus)
			mLastPlayerStatus = mScene->getPlayerStatus();
		
		mScene.reset(new Scene(*mAIManager, *this, *mTilesetTexture, mThreadPool));
		PH_LOG_INFO("The scene was replaced by new scene (" + mFilePathOfSceneToMake + ").");

		parseScene(mScene->getCutSceneManager(), mEntitiesTemplateStorage, mScene->getRegistry(), mFilePathOfSceneToMake,
		           mScene->getSystemsQueue(), *mAIManager, *this);

		if(mAIManager->isPlayerOnScene()) {
			mScene->setPlayerStatus(mLastPlayerStatus);
			if(mHasPlayerPositionForNextScene)
				mScene->setPlayerPosition(mPlayerPositionForNextScene);
		}
	}

	mIsReplacing = false;
	mCurrentSceneFilePath = std::move(mFilePathOfSceneToMake);
}

void SceneManager::handleEvent(sf::Event e)
{
	mScene->handleEvent(e);
}

void SceneManager::update(float dt)
{
	PH_ASSERT_UNEXPECTED_SITUATION(mScene != nullptr, "There is no active scene.");
	mScene->update(dt);
}

void SceneManager::init(AIManager* aiManager)
{
	mAIManager = aiManager;
	loadTexture("textures/map/extrudedTileset.png", true);
	mTilesetTexture = &getTexture("textures/map/extrudedTileset.png");
}

void SceneManager::replaceScene(const std::string& sceneSourceCodeFilePath)
{
	PH_LOG_INFO("Scene replacement requested, scene file: " + sceneSourceCodeFilePath);
	mFilePathOfSceneToMake = sceneSourceCodeFilePath;
	mIsReplacing = true;
	mHasPlayerPositionForNextScene = false;
}

void SceneManager::replaceScene(const std::string& sceneSourceCodeFilePath, const sf::Vector2f& playerPosition)
{
	PH_LOG_INFO("Scene replacement requested, scene file: " + sceneSourceCodeFilePath + 
				", player position: " + std::to_string(playerPosition.x) + ',' + std::to_string(playerPosition.y));
	mFilePathOfSceneToMake = sceneSourceCodeFilePath;
	mIsReplacing = true;
	mHasPlayerPositionForNextScene = true;
	mPlayerPositionForNextScene = playerPosition;
}

void SceneManager::popScene()
{
	PH_LOG_INFO("Scene popping requested.");
    mIsPopping = true;
}

}
