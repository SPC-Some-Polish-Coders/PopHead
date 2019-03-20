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
        mPendingStates.push_back(std::move(state));
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
        mPendingStates.push_back(std::move(state));
        mIsReplacing = true;
    }
    else{
        /// log - ERROR | States | couldn't replace state because another state is pushing
    }
}

void StateMachine::popState()
{

}

void StateMachine::clearStates()
{

}

unsigned int StateMachine::getStatesAmount() const
{

}

bool StateMachine::getHideInStateNr(unsigned int nrOfState) const
{

}

bool StateMachine::getPauseInStateNr(unsigned int nrOfState) const
{

}

void StateMachine::setHideInStateNr(unsigned int nrOfState, bool hide)
{

}

void StateMachine::setPauseInStateNr(unsigned int nrOfState, bool pause)
{

}

