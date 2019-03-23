#ifndef POPHEAD_STATES_GREENSTATE_GREENSTATE_HPP
#define POPHEAD_STATES_GREENSTATE_GREENSTATE_HPP

#include "States/state.hpp"

namespace PopHead{
namespace States{


class GreenState : public State
{
public:
    GreenState(Base::GameData *const gameData);

    void input() override;
    void update(sf::Time delta) override;
};


}}

#endif // POPHEAD_STATES_GREENSTATE_GREENSTATE_HPP
