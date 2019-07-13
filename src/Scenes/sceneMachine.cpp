#include "sceneMachine.hpp"
#include "scene.hpp"
#include "Utilities/debug.hpp"
#include "gameData.hpp"

namespace ph {

SceneMachine::SceneMachine()
	:mScene(nullptr)
	,mGameData(nullptr)
	,mIsReplacing(false)
	,mIsPopping(false)
{
}

void SceneMachine::changingScenesProcess()
{
	if (mIsPopping)
		popAction();

	if (mIsReplacing)
		replaceAction();
}

void SceneMachine::popAction()
{
	if (mScene == nullptr)
		PH_LOG(LogType::Warning, "You are trying to pop scene but there is no scene to pop.");
	else {
		mScene = nullptr;
		PH_LOG(LogType::Info, "The scene was popped.");
	}
	mIsPopping = false;
}

void SceneMachine::replaceAction()
{
	mGameData->getPhysicsEngine().clear();
	mScene = std::make_unique<Scene>(mGameData, mSceneToMakeSourceCodeFilepath);
	PH_LOG(LogType::Info, "The scene was replaced by new scene (" + mSceneToMakeSourceCodeFilepath + ").");
	mIsReplacing = false;
}

void SceneMachine::input()
{
	if(mScene != nullptr)
		mScene->input();
}

void SceneMachine::update(sf::Time delta)
{
	if(mScene != nullptr)
		mScene->update(delta);
}

void SceneMachine::replaceScene(const std::string& sceneSourceCodeFilePath)
{
	mSceneToMakeSourceCodeFilepath = sceneSourceCodeFilePath;
	mIsReplacing = true;
}

void SceneMachine::popScene()
{
    mIsPopping = true;
}

}