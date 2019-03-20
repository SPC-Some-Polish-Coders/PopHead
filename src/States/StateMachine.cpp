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

}

void StateMachine::popState()
{

}

void StateMachine::replaceState(StatePtr replacer)
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

