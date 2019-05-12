#include "States/state.hpp"

#include "World/Entity/entityType.hpp"

using PopHead::States::State;
using PopHead::World::EntityType;

State::State( PopHead::Base::GameData* const gameData )
        : mGameData { gameData }
        , mRoot { EntityType::none, gameData, "root" }
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
