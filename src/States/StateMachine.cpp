#include "States/StateMachine.hpp"

using PopHead::States::StateMachine;


StateMachine::StateMachine()
:mIsAdding(false)
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
    }

    if(mIsAdding)
    {
        while(!mPendingStates.empty()){
            mActiveStates.emplace_back(std::move(mPendingStates.front()));
            mPendingStates.pop_front();
            /// log - GOOD | States | the state in the back of the vector was replaced by new state
        }
    }

    if(mIsReplacing)
    {
        mActiveStates.emplace_back(std::move(mPendingStates.back()));
        mPendingStates.clear();
        /// log - GOOD | States | the new state was pushed into back of the vector
    }
}

void StateMachine::input()
{

}

void StateMachine::update(sf::Time delta)
{

}

void StateMachine::pushState(StatePtr state)
{
    if(mIsReplacing == false){
        mPendingStates.emplace_back(std::move(state));
        mIsAdding = true;
    }
    else{
        /// log - ERROR | States | couldn't push state because another state is replacing
    }
}

void StateMachine::replaceState(StatePtr state)
{
    if(mIsAdding == false){
        mPendingStates.clear();
        mPendingStates.emplace_back(std::move(state));
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

