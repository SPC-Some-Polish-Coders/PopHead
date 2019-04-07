#include "States/stateMachine.hpp"

#include <iostream>

#include "GameState/gameState.hpp"

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
            /// log - WARNING | States | you are trying to clear active states, but there are no states in vector
        }
        else{
            mActiveStates.clear();
            /// log - GOOD | States | vector of active states was cleared
        }
        mIsClearing = false;
    }

    if(mIsRemoving)
    {
        if(mActiveStates.empty()){
            /// log - WARNING | States | you are trying to pop state, but there are no states in vector
        }
        else{
            mActiveStates.pop_back();
            /// log - GOOD | States | the state in the back of the vector of active states was popped (deleted)
        }
        mIsRemoving = false;
    }

    if(mIsAdding)
    {
        while(!mPendingStates.empty()){
            mActiveStates.emplace_back(std::move(mPendingStates.front()));
            mPendingStates.pop_front();
            /// log - GOOD | States | the state in the back of the vector was replaced by new state
        }
        mIsAdding = false;
    }

    if(mIsReplacing)
    {
        mActiveStates.emplace_back(std::move(mPendingStates.back()));
        mPendingStates.clear();
        /// log - GOOD | States | the new state was pushed into back of the vector
        mIsReplacing = false;
    }
}

void StateMachine::input()
{
    if(mActiveStates.empty()){
        //it's temporary. Later we'll change it to be handle by LogManager.
        std::cout<<"ERROR | States | cannot execute input because there are no States on the vector"<<std::endl;
    }
    else{
        mActiveStates.back()->input();
    }
}

void StateMachine::update(sf::Time delta)
{
    if(mActiveStates.empty()){
        //it's temporary. Later we'll change it to be handle by LogManager.
        std::cout<<"ERROR | States | cannot execute update because there are no States on the vector"<<std::endl;
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
        /// log - ERROR | States | couldn't push state because another state is replacing
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
        /// log - ERROR | States | couldn't replace state because another state is pushing
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

unsigned int StateMachine::getStatesAmount() const
{
    return mActiveStates.size();
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

void StateMachine::setGameData( PopHead::Base::GameData* const ptr )
{
    mGameData = ptr;
}

