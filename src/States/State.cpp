#include "State.hpp"

using PopHead::States::State;


bool State::getHide() const
{
    return mHide;
}

bool State::getPause() const
{
    return mPause;
}

void State::setHide(bool hide)
{
    mHide = hide;
}

void State::setPause(bool pause)
{
    mPause = pause;
}
