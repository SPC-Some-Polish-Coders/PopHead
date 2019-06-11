#include "States/state.hpp"

#include "World/Entity/entityType.hpp"

using ph::States::State;
using ph::World::EntityType;

State::State( ph::Base::GameData* const gameData )
        : mGameData { gameData }
        , mRoot { EntityType::none, gameData, "root" }
        , mHide { false }
        , mPause { false }
{
}
