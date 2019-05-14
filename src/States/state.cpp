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
