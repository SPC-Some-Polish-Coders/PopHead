#ifndef POPHEAD_STATES_GAMESTATE_GAMESTATE_HPP
#define POPHEAD_STATES_GAMESTATE_GAMESTATE_HPP

#include "States/state.hpp"

namespace PopHead{
namespace States{


class GameState : public State
{
public:
    GameState(Base::GameData *const);

    void input() override;
    void update(sf::Time delta) override;
};


}}

#endif // !POPHEAD_STATES_GAMESTATE_GAMESTATE_HPP
