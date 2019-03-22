#include "State.hpp"

using PopHead::States::State;

State::State( Base::GameData* const gameData )
        : mGameData { gameData }
        // , World::Entity mRoot;
        , mHide { false }
        , mPause { false }
{
}

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
