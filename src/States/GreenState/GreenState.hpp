#ifndef POPHEAD_STATES_GREENSTATE_GREENSTATE_HPP
#define POPHEAD_STATES_GREENSTATE_GREENSTATE_HPP

#include "States/State.hpp"

namespace PopHead{
namespace States{


class GreenState : public State
{
public:
    GreenState(Base::GameData *gameData);

    void input() override;
    void update(sf::Time delta) override;

private:
    Base::GameData *gameData;
};


}}

#endif // POPHEAD_STATES_GREENSTATE_GREENSTATE_HPP
