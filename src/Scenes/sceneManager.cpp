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
	,mAIManager(nullptr)
	,mTextures(nullptr)
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
		PH_LOG_INFO("The scene was popped.");
	}
	mIsPopping = false;
}

void SceneManager::replaceAction()
{
	GUI::clear();

	if(mCurrentSceneFilePath == mFilePathOfSceneToMake && mHasPlayerPositionForNextScene)
		mScene->setPlayerPosition(mPlayerPositionForNextScene);
	else {
		bool thereIsPlayerStatus = mScene && mAIManager->isPlayerOnScene();
		if (thereIsPlayerStatus)
			mLastPlayerStatus = mScene->getPlayerStatus();
		
		mScene.reset(new Scene(*mAIManager, *this, *mTilesetTexture));

		parseScene(mScene->getCutSceneManager(), mEntitiesTemplateStorage, mScene->getRegistry(), mFilePathOfSceneToMake,
		           *mTextures, mScene->getSystemsQueue(), *mAIManager, *this);

		if(mAIManager->isPlayerOnScene()) {
			mScene->setPlayerStatus(mLastPlayerStatus);
			if(mHasPlayerPositionForNextScene)
				mScene->setPlayerPosition(mPlayerPositionForNextScene);
		}
	}

	PH_LOG_INFO("The scene was replaced by new scene (" + mFilePathOfSceneToMake + ").");
	mIsReplacing = false;
	mCurrentSceneFilePath = std::move(mFilePathOfSceneToMake);
}

void SceneManager::handleEvent(const Event& e)
{
	if (auto* event = std::get_if<ActionEvent>(&e))
		mScene->handleEvent(*event);
}

void SceneManager::update(float dt)
{
	PH_ASSERT_UNEXPECTED_SITUATION(mScene != nullptr, "There is no active scene");
	mScene->update(dt);
}

void SceneManager::init(TextureHolder* textures, AIManager* aiManager)
{
	mAIManager = aiManager;
	mTextures = textures;
	textures->load("textures/map/extrudedTileset.png");
	mTilesetTexture = &textures->get("textures/map/extrudedTileset.png");
}

void SceneManager::replaceScene(const std::string& sceneSourceCodeFilePath)
{
	mFilePathOfSceneToMake = sceneSourceCodeFilePath;
	mIsReplacing = true;
	mHasPlayerPositionForNextScene = false;
}

void SceneManager::replaceScene(const std::string& sceneSourceCodeFilePath, const sf::Vector2f& playerPosition)
{
	mFilePathOfSceneToMake = sceneSourceCodeFilePath;
	mIsReplacing = true;
	mHasPlayerPositionForNextScene = true;
	mPlayerPositionForNextScene = playerPosition;
}

void SceneManager::popScene()
{
    mIsPopping = true;
}

}
