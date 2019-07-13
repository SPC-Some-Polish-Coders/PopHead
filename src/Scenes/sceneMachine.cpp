#include "sceneMachine.hpp"
#include "scene.hpp"
#include "Utilities/debug.hpp"
#include "gameData.hpp"

namespace ph {

SceneMachine::SceneMachine()
	:mGameData(nullptr)
	,mIsPushing(false)
	,mIsReplacing(false)
	,mIsPopping(false)
	,mIsClearing(false)
{
}

void SceneMachine::changingScenesProcess()
{
	if (mIsClearing)
		clearAction();

	if (mIsPopping)
		popAction();

	if (mIsPushing)
		pushAction();

	if (mIsReplacing)
		replaceAction();
}

void SceneMachine::clearAction()
{
	if (mActiveScenes.empty())
		PH_LOG(LogType::Warning, "You are trying to clear active scenes, but there are no scenes in vector.");
	else {
		mActiveScenes.clear();
		mGameData->getPhysicsEngine().clear();
		PH_LOG(LogType::Info, "Vector of active scenes was cleared.");
	}
	mIsClearing = false;
}

void SceneMachine::popAction()
{
	if (mActiveScenes.empty())
		PH_LOG(LogType::Warning, "You are trying to pop scene, but there are no scenes in vector.");
	else {
		mActiveScenes.pop_back();
		PH_LOG(LogType::Info, "The scene in the back of the vector of active scenes was popped (deleted).");
	}
	mIsPopping = false;
}

void SceneMachine::pushAction()
{
	while (!mPendingScenesSourceCodePaths.empty()) {
		mActiveScenes.emplace_back(mGameData, mPendingScenesSourceCodePaths.front());
		mPendingScenesSourceCodePaths.pop_front();
		PH_LOG(LogType::Info, "The new scene was pushed into back of the vector.");
	}
	mIsPushing = false;
}

void SceneMachine::replaceAction()
{
	mGameData->getPhysicsEngine().clear();
	mActiveScenes.emplace_back(mGameData, mPendingScenesSourceCodePaths.front());
	mPendingScenesSourceCodePaths.clear();
	PH_LOG(LogType::Info, "The scene in the back of the vector was replaced by new scene.");
	mIsReplacing = false;
}

void SceneMachine::input()
{
    if(mActiveScenes.empty())
		PH_LOG(LogType::Error, "Cannot execute input because there are no scene on the vector.");
    else
        mActiveScenes.back().input();
}

void SceneMachine::update(sf::Time delta)
{
    if(mActiveScenes.empty())
		PH_LOG(LogType::Error, "Cannot execute update because there are no scenes on the vector.");
    else
        mActiveScenes.back().update(delta);
}

void SceneMachine::pushScene(const std::string& sceneSourceCodeFilePath)
{
    if(mIsReplacing == false){
		mPendingScenesSourceCodePaths.emplace_back(sceneSourceCodeFilePath);
        mIsPushing = true;
    }
    else
		PH_LOG(LogType::Error, "Couldn't push scene because another scene is replacing.");
}

void SceneMachine::replaceScene(const std::string& sceneSourceCodeFilePath)
{
    if(mIsPushing == false) {
		mPendingScenesSourceCodePaths.clear();
		mPendingScenesSourceCodePaths.emplace_back(sceneSourceCodeFilePath);
        mIsReplacing = true;
    }
    else
		PH_LOG(LogType::Error, "Couldn't replace scene because another scene is pushing.");
}

void SceneMachine::popScene()
{
    mIsPopping = true;
}

void SceneMachine::clearScenes()
{
    mIsClearing = true;
}

bool SceneMachine::getHideInSceneNr(unsigned int nrOfScene) const
{
    return mActiveScenes[ mActiveScenes.size() - nrOfScene - 1 ].getHide();
}

bool SceneMachine::getPauseInSceneNr(unsigned int nrOfScene) const
{
    return mActiveScenes[ mActiveScenes.size() - nrOfScene - 1 ].getPause();
}

void SceneMachine::setHideInSceneNr(unsigned int nrOfScene, bool hide)
{
    mActiveScenes[ mActiveScenes.size() - nrOfScene - 1 ].setHide(hide);
}

void SceneMachine::setPauseInSceneNr(unsigned int nrOfScene, bool pause)
{
    mActiveScenes[ mActiveScenes.size() - nrOfScene - 1 ].setPause(pause);
}

}