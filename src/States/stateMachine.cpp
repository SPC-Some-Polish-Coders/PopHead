#include "States/stateMachine.hpp"

#include <iostream>

#include "GameState/gameState.hpp"
#include "Logs/logger.hpp"

using PopHead::States::StateMachine;
using PopHead::States::State;

#include "state.hpp"

StateMachine::StateMachine()
:mGameData(nullptr)
,mIsAdding(false)
,mIsReplacing(false)
,mIsRemoving(false)
,mIsClearing(false)
{
}

void StateMachine::changingStatesProcess()
{
    if(mIsClearing)
    {
        if(mActiveStates.empty()){
			PH_LOG(LogType::WARNING, "You are trying to clear active states, but there are no states in vector.");
        }
        else{
            mActiveStates.clear();
			PH_LOG(LogType::INFO, "Vector of active states was cleared.");
        }
        mIsClearing = false;
    }

    if(mIsRemoving)
    {
        if(mActiveStates.empty()){
			PH_LOG(LogType::WARNING, "You are trying to pop state, but there are no states in vector.");
        }
        else{
            mActiveStates.pop_back();
			PH_LOG(LogType::INFO, "The state in the back of the vector of active states was popped (deleted).");
        }
        mIsRemoving = false;
    }

    if(mIsAdding)
    {
        while(!mPendingStates.empty()){
            mActiveStates.emplace_back(std::move(mPendingStates.front()));
            mPendingStates.pop_front();
			PH_LOG(LogType::INFO, "The state in the back of the vector was replaced by new state.");
        }
        mIsAdding = false;
    }

    if(mIsReplacing)
    {
        mActiveStates.emplace_back(std::move(mPendingStates.back()));
        mPendingStates.clear();
		PH_LOG(LogType::INFO, "The new state was pushed into back of the vector.");
        mIsReplacing = false;
    }
}

void StateMachine::input()
{
    if(mActiveStates.empty()){
		PH_LOG(LogType::ERROR, "Cannot execute input because there are no States on the vector.");
    }
    else{
        mActiveStates.back()->input();
    }
}

void StateMachine::update(sf::Time delta)
{
    if(mActiveStates.empty()){
		PH_LOG(LogType::ERROR, "Cannot execute update because there are no States on the vector.");
    }
    else{
        mActiveStates.back()->update(delta);

        #if 0
        for(auto state& : mActiveStates){
            if(!state->getPause())
                state->update(delta);
        }
        #endif // 0
    }
}

void StateMachine::pushState(PopHead::States::StateID id)
{
    if(mIsReplacing == false){
        mPendingStates.emplace_back( std::move(getStatePtr(id)) );
        mIsAdding = true;
    }
    else{
		PH_LOG(LogType::ERROR, "Couldn't push state because another state is replacing.");
    }
}

void StateMachine::replaceState(PopHead::States::StateID id)
{
    if(mIsAdding == false){
        mPendingStates.clear();
        mPendingStates.emplace_back( std::move(getStatePtr(id)) );
        mIsReplacing = true;
    }
    else{
		PH_LOG(LogType::ERROR, "Couldn't replace state because another state is pushing.");
    }
}

void StateMachine::popState()
{
    mIsRemoving = true;
}

void StateMachine::clearStates()
{
    mIsClearing = true;
}

bool StateMachine::getHideInStateNr(unsigned int nrOfState) const
{
    return mActiveStates[ mActiveStates.size() - nrOfState - 1 ]->getHide();
}

bool StateMachine::getPauseInStateNr(unsigned int nrOfState) const
{
    return mActiveStates[ mActiveStates.size() - nrOfState - 1 ]->getPause();
}

void StateMachine::setHideInStateNr(unsigned int nrOfState, bool hide)
{
    mActiveStates[ mActiveStates.size() - nrOfState - 1 ]->setHide(hide);
}

void StateMachine::setPauseInStateNr(unsigned int nrOfState, bool pause)
{
    mActiveStates[ mActiveStates.size() - nrOfState - 1 ]->setPause(pause);
}

auto StateMachine::getStatePtr(PopHead::States::StateID id) const -> std::unique_ptr<State>
{
    switch(id)
    {
	case PopHead::States::StateID::GameState:{
            return StatePtr(new PopHead::States::GameState(mGameData));
        }
    }
}

