#include "pch.hpp"
#include "sceneManager.hpp"
#include "sceneParser.hpp"
#include "scene.hpp"
#include "GUI/gui.hpp"
#include "GUI/xmlGuiParser.hpp"
#include "ECS/xmlMapParser.hpp"
#include "Audio/xmlAudioParser.hpp"
#include "ECS/entitiesParser.hpp"
#include "ECS/tiledParser.hpp"
#include "ECS/Components/charactersComponents.hpp"
#include "ECS/Components/physicsComponents.hpp"

namespace ph {

SceneManager::SceneManager()
	:mScene(Null)
	,mThreadPool(0)
	,mAIManager(Null)
	,mIsReplacing(false)
	,mIsPopping(false)
	,mHasPlayerPositionForNextScene(false)
	,mLastPlayerStatus()
	,mTilesetTexture(Null)
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

const Vec2& SceneManager::getPlayerPositionForNextScene() const
{
	return mPlayerPositionForNextScene;
}

void SceneManager::popAction()
{
	if (mScene == Null)
	{
		PH_LOG_WARNING("You are trying to pop scene but there is no scene to pop.");
	}
	else 
	{
		GUI::clear();
		mScene = Null;
		PH_LOG_INFO("The scene was popped, scene file: " + mCurrentSceneFilePath);
	}
	mIsPopping = false;
}

void SceneManager::replaceAction()
{
	GUI::clear();

	bool thereIsPlayerStatus = mScene && mAIManager->isPlayerOnScene();
	if (thereIsPlayerStatus)
		mLastPlayerStatus = mScene->getPlayerStatus();
	
	mScene.reset(new Scene(*mAIManager, *this, *mTilesetTexture, mThreadPool, mEntitiesTemplateStorage, mWindow));
	PH_LOG_INFO("The scene was replaced by new scene (" + mFilePathOfSceneToMake + ").");

	parseScene(mEntitiesTemplateStorage, mScene->getRegistry(), mFilePathOfSceneToMake,
			   mScene->getSystemsQueue(), *mAIManager, *this, mTilesetTexture);

	if(mAIManager->isPlayerOnScene()) 
	{
		mScene->setPlayerStatus(mLastPlayerStatus);
		if(mHasPlayerPositionForNextScene)
			mScene->setPlayerPosition(mPlayerPositionForNextScene);
	}

	if (mHasPlayerPositionForNextScene)
	{
		mScene->setPlayerPosition(mPlayerPositionForNextScene);
	}

	mIsReplacing = false;
	mCurrentSceneFilePath = std::move(mFilePathOfSceneToMake);
}

void SceneManager::handleEvent(sf::Event e)
{
	mScene->handleEvent(e);

	#ifndef PH_DISTRIBUTION
	if(e.type == sf::Event::KeyPressed && e.key.alt && e.key.code == sf::Keyboard::M)
	{
		Vec2 playerPos = nullVec2; 
		mScene->getRegistry().view<component::Player, component::BodyRect>().each([&]
		(auto, auto& playerBody)
		{
			playerPos = playerBody.pos;
		});

		if(playerPos == nullVec2)
			replaceScene(mCurrentSceneFilePath);
		else
			replaceScene(mCurrentSceneFilePath, playerPos);
	}
	#endif
}

void SceneManager::update(float dt)
{
	PH_ASSERT_UNEXPECTED_SITUATION(mScene != Null, "There is no active scene.");
	mScene->update(dt);
}

void SceneManager::init(AIManager* aiManager, sf::Window* window)
{
	mAIManager = aiManager;
	mWindow = window;
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

void SceneManager::replaceScene(const std::string& sceneSourceCodeFilePath, Vec2 playerPosition)
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
