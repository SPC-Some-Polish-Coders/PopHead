#include "States/stateMachine.hpp"

#include <iostream>

#include "GameState/gameState.hpp"
#include "Utilities/debug.hpp"

#include "state.hpp"

ph::StateMachine::StateMachine()
	:mGameData(nullptr)
	,mIsPushing(false)
	,mIsReplacing(false)
	,mIsPopping(false)
	,mIsClearing(false)
{
}

void ph::StateMachine::changingStatesProcess()
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

void ph::StateMachine::clearAction()
{
	if (mActiveStates.empty())
		PH_LOG(LogType::Warning, "You are trying to clear active states, but there are no states in vector.");
	else {
		mActiveStates.clear();
		PH_LOG(LogType::Info, "Vector of active states was cleared.");
	}
	mIsClearing = false;
}

void ph::StateMachine::popAction()
{
	if (mActiveStates.empty())
		PH_LOG(LogType::Warning, "You are trying to pop state, but there are no states in vector.");
	else {
		mActiveStates.pop_back();
		PH_LOG(LogType::Info, "The state in the back of the vector of active states was popped (deleted).");
	}
	mIsPopping = false;
}

void ph::StateMachine::pushAction()
{
	while (!mPendingStates.empty()) {
		mActiveStates.emplace_back(std::move(mPendingStates.front()));
		mPendingStates.pop_front();
		PH_LOG(LogType::Info, "The new state was pushed into back of the vector.");
	}
	mIsPushing = false;
}

void ph::StateMachine::replaceAction()
{
	mActiveStates.emplace_back(std::move(mPendingStates.back()));
	mPendingStates.clear();
	PH_LOG(LogType::Info, "The state in the back of the vector was replaced by new state.");
	mIsReplacing = false;
}

void ph::StateMachine::input()
{
    if(mActiveStates.empty())
		PH_LOG(LogType::Error, "Cannot execute input because there are no States on the vector.");
    else
        mActiveStates.back()->input();
}

void ph::StateMachine::update(sf::Time delta)
{
    if(mActiveStates.empty())
		PH_LOG(LogType::Error, "Cannot execute update because there are no States on the vector.");
    else
        mActiveStates.back()->update(delta);
}

void ph::StateMachine::pushState(ph::StateID id)
{
    if(mIsReplacing == false){
        mPendingStates.emplace_back( std::move(getStatePtr(id)) );
        mIsPushing = true;
    }
    else
		PH_LOG(LogType::Error, "Couldn't push state because another state is replacing.");
}

void ph::StateMachine::replaceState(ph::StateID id)
{
    if(mIsPushing == false){
        mPendingStates.clear();
        mPendingStates.emplace_back( std::move(getStatePtr(id)) );
        mIsReplacing = true;
    }
    else
		PH_LOG(LogType::Error, "Couldn't replace state because another state is pushing.");
}

void ph::StateMachine::popState()
{
    mIsPopping = true;
}

void ph::StateMachine::clearStates()
{
    mIsClearing = true;
}

bool ph::StateMachine::getHideInStateNr(unsigned int nrOfState) const
{
    return mActiveStates[ mActiveStates.size() - nrOfState - 1 ]->getHide();
}

bool ph::StateMachine::getPauseInStateNr(unsigned int nrOfState) const
{
    return mActiveStates[ mActiveStates.size() - nrOfState - 1 ]->getPause();
}

void ph::StateMachine::setHideInStateNr(unsigned int nrOfState, bool hide)
{
    mActiveStates[ mActiveStates.size() - nrOfState - 1 ]->setHide(hide);
}

void ph::StateMachine::setPauseInStateNr(unsigned int nrOfState, bool pause)
{
    mActiveStates[ mActiveStates.size() - nrOfState - 1 ]->setPause(pause);
}

auto ph::StateMachine::getStatePtr(ph::StateID id) const -> std::unique_ptr<State>
{
    switch(id)
    {
	case StateID::GameState:
		return StatePtr(new GameState(mGameData));
	default:
		PH_LOG(LogType::Error, "This state was't implemented yet.");
    }
}

