#include "States/state.hpp"

#include "World/Entity/entityType.hpp"

namespace ph {

State::State(GameData* const gameData)
        :mGameData { gameData }
        ,mRoot { EntityType::none, gameData, "root" }
        ,mHide { false }
        ,mPause { false }
{
}

}